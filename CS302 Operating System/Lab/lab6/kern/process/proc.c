#include <assert.h>
#include <elf.h>
#include <error.h>
#include <kmalloc.h>
#include <pmm.h>
#include <proc.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sync.h>
#include <trap.h>
#include <unistd.h>
#include <vmm.h>

/* ------------- process/thread mechanism design&implementation -------------
(an simplified Linux process/thread mechanism )
introduction:
  ucore implements a simple process/thread mechanism. process contains the
independent memory sapce, at least one threads for execution, the kernel
data(for management), processor state (for context switch), files(in lab6), etc.
ucore needs to manage all these details efficiently. In ucore, a thread is just
a special kind of process(share process's memory).
------------------------------
process state       :     meaning               -- reason
    PROC_UNINIT     :   uninitialized           -- alloc_proc
    PROC_SLEEPING   :   sleeping                -- try_free_pages, do_wait,
do_sleep PROC_RUNNABLE   :   runnable(maybe running) -- proc_init, wakeup_proc,
    PROC_ZOMBIE     :   almost dead             -- do_exit

-----------------------------
process state changing:

  alloc_proc                                 RUNNING
      +                                   +--<----<--+
      +                                   + proc_run +
      V                                   +-->---->--+
PROC_UNINIT -- proc_init/wakeup_proc --> PROC_RUNNABLE --
try_free_pages/do_wait/do_sleep --> PROC_SLEEPING -- A      + + |      +---
do_exit --> PROC_ZOMBIE                                +
                                           + +
                                           -----------------------wakeup_proc----------------------------------
-----------------------------
process relations
parent:           proc->parent  (proc is children)
children:         proc->cptr    (proc is parent)
older sibling:    proc->optr    (proc is younger sibling)
younger sibling:  proc->yptr    (proc is older sibling)
-----------------------------
related syscall for process:
SYS_exit        : process exit,                           -->do_exit
SYS_fork        : create child process, dup mm -->do_fork-->wakeup_proc SYS_wait
: wait process                            -->do_wait SYS_exec        : after
fork, process execute a program   -->load a program and refresh the mm SYS_clone
: create child thread                     -->do_fork-->wakeup_proc SYS_yield :
process flag itself need resecheduling, -- proc->need_sched=1, then scheduler
will rescheule this process SYS_sleep       : process sleep -->do_sleep SYS_kill
: kill process                            -->do_kill-->proc->flags |= PF_EXITING
                                                                 -->wakeup_proc-->do_wait-->do_exit
SYS_getpid      : get the process's pid

*/

// the process set's list
list_entry_t proc_list;

#define HASH_SHIFT 10
#define HASH_LIST_SIZE (1 << HASH_SHIFT)
#define pid_hashfn(x) (hash32(x, HASH_SHIFT))

// has list for process set based on pid
static list_entry_t hash_list[HASH_LIST_SIZE];

// idle proc
struct proc_struct *idleproc = NULL;
// init proc
struct proc_struct *initproc = NULL;
// current proc
struct proc_struct *current = NULL;

static int nr_process = 0;

void kernel_thread_entry(void);
void forkrets(struct trapframe *tf);
void switch_to(struct context *from, struct context *to);

// alloc_proc - alloc a proc_struct and init all fields of proc_struct
static struct proc_struct *alloc_proc(void)
{
	struct proc_struct *proc = kmalloc(sizeof(struct proc_struct));
	if (proc != NULL) {
		proc->state = PROC_UNINIT;
		proc->pid = -1;
		proc->runs = 0;
		proc->kstack = 0;
		proc->need_resched = 0;
		proc->parent = NULL;
		proc->mm = NULL;
		memset(&(proc->context), 0, sizeof(struct context));
		proc->tf = NULL;
		proc->satp = boot_satp;
		proc->flags = 0;
		memset(proc->name, 0, PROC_NAME_LEN);
		proc->wait_state = 0;
		proc->cptr = proc->optr = proc->yptr = NULL;
	}
	return proc;
}

// set_proc_name - set the name of proc
char *set_proc_name(struct proc_struct *proc, const char *name)
{
	memset(proc->name, 0, sizeof(proc->name));
	return memcpy(proc->name, name, PROC_NAME_LEN);
}

// get_proc_name - get the name of proc
char *get_proc_name(struct proc_struct *proc)
{
	static char name[PROC_NAME_LEN + 1];
	memset(name, 0, sizeof(name));
	return memcpy(name, proc->name, PROC_NAME_LEN);
}

// set_links - set the relation links of process
static void set_links(struct proc_struct *proc)
{
	list_add(&proc_list, &(proc->list_link));
	proc->yptr = NULL;
	if ((proc->optr = proc->parent->cptr) != NULL) {
		proc->optr->yptr = proc;
	}
	proc->parent->cptr = proc;
	nr_process++;
}

// remove_links - clean the relation links of process
static void remove_links(struct proc_struct *proc)
{
	list_del(&(proc->list_link));
	if (proc->optr != NULL) {
		proc->optr->yptr = proc->yptr;
	}
	if (proc->yptr != NULL) {
		proc->yptr->optr = proc->optr;
	} else {
		proc->parent->cptr = proc->optr;
	}
	nr_process--;
}

// get_pid - alloc a unique pid for process
static int get_pid(void)
{
	static_assert(MAX_PID > MAX_PROCESS);
	struct proc_struct *proc;
	list_entry_t *list = &proc_list, *le;
	static int next_safe = MAX_PID, last_pid = MAX_PID;
	if (++last_pid >= MAX_PID) {
		last_pid = 1;
		goto inside;
	}
	if (last_pid >= next_safe) {
inside:
		next_safe = MAX_PID;
repeat:
		le = list;
		while ((le = list_next(le)) != list) {
			proc = le2proc(le, list_link);
			if (proc->pid == last_pid) {
				if (++last_pid >= next_safe) {
					if (last_pid >= MAX_PID) {
						last_pid = 1;
					}
					next_safe = MAX_PID;
					goto repeat;
				}
			} else if (proc->pid > last_pid &&
				   next_safe > proc->pid) {
				next_safe = proc->pid;
			}
		}
	}
	return last_pid;
}

// proc_run - make process "proc" running on cpu
// NOTE: before call switch_to, should load  base addr of "proc"'s new PDT
void proc_run(struct proc_struct *proc)
{
	if (proc != current) {
		bool intr_flag;
		struct proc_struct *prev = current, *next = proc;
		local_intr_save(intr_flag);
		{
			current = proc;
			lsatp(next->satp);
			switch_to(&(prev->context), &(next->context));
		}
		local_intr_restore(intr_flag);
	}
}

// forkret -- the first kernel entry point of a new thread/process
// NOTE: the addr of forkret is setted in copy_thread function
//       after switch_to, the current proc will execute here.
static void forkret(void)
{
	forkrets(current->tf);
}

// hash_proc - add proc into proc hash_list
static void hash_proc(struct proc_struct *proc)
{
	list_add(hash_list + pid_hashfn(proc->pid), &(proc->hash_link));
}

// unhash_proc - delete proc from proc hash_list
static void unhash_proc(struct proc_struct *proc)
{
	list_del(&(proc->hash_link));
}

// find_proc - find proc frome proc hash_list according to pid
struct proc_struct *find_proc(int pid)
{
	if (0 < pid && pid < MAX_PID) {
		list_entry_t *list = hash_list + pid_hashfn(pid), *le = list;
		while ((le = list_next(le)) != list) {
			struct proc_struct *proc = le2proc(le, hash_link);
			if (proc->pid == pid) {
				return proc;
			}
		}
	}
	return NULL;
}

// kernel_thread - create a kernel thread using "fn" function
// NOTE: the contents of temp trapframe tf will be copied to
//       proc->tf in do_fork-->copy_thread function
int kernel_thread(int (*fn)(void *), void *arg, uint32_t clone_flags)
{
	struct trapframe tf;
	memset(&tf, 0, sizeof(struct trapframe));

	tf.gpr.s0 = (uintptr_t)fn;
	tf.gpr.s1 = (uintptr_t)arg;
	tf.status = (read_csr(sstatus) | SSTATUS_SPP | SSTATUS_SPIE) &
		    ~SSTATUS_SIE;
	tf.epc = (uintptr_t)kernel_thread_entry;
	return do_fork(clone_flags | CLONE_VM, 0, &tf);
}

// setup_kstack - alloc pages with size KSTACKPAGE as process kernel stack
static int setup_kstack(struct proc_struct *proc)
{
	struct Page *page = alloc_pages(KSTACKPAGE);
	if (page != NULL) {
		proc->kstack = (uintptr_t)page2kva(page);
		return 0;
	}
	return -E_NO_MEM;
}

// put_kstack - free the memory space of process kernel stack
static void put_kstack(struct proc_struct *proc)
{
	free_pages(kva2page((void *)(proc->kstack)), KSTACKPAGE);
}

// setup_pgdir - alloc one page as PDT
static int setup_pgdir(struct mm_struct *mm)
{
	struct Page *page;
	if ((page = alloc_page()) == NULL) {
		return -E_NO_MEM;
	}
	pde_t *pgdir = page2kva(page);
	memcpy(pgdir, boot_pgdir, PGSIZE);

	mm->pgdir = pgdir;
	return 0;
}

// put_pgdir - free the memory space of PDT
static void put_pgdir(struct mm_struct *mm)
{
	free_page(kva2page(mm->pgdir));
}

// copy_mm - process "proc" duplicate OR share process "current"'s mm according
// clone_flags
//         - if clone_flags & CLONE_VM, then "share" ; else "duplicate"
static int copy_mm(uint32_t clone_flags, struct proc_struct *proc)
{
	struct mm_struct *mm, *oldmm = current->mm;

	/* current is a kernel thread */
	if (oldmm == NULL) {
		return 0;
	}
	if (clone_flags & CLONE_VM) {
		mm = oldmm;
		goto good_mm;
	}
	int ret = -E_NO_MEM;
	if ((mm = mm_create()) == NULL) {
		goto bad_mm;
	}
	if (setup_pgdir(mm) != 0) {
		goto bad_pgdir_cleanup_mm;
	}
	lock_mm(oldmm);
	{
		ret = dup_mmap(mm, oldmm);
	}
	unlock_mm(oldmm);

	if (ret != 0) {
		goto bad_dup_cleanup_mmap;
	}

good_mm:
	mm_count_inc(mm);
	proc->mm = mm;
	proc->satp = PADDR(mm->pgdir);
	return 0;
bad_dup_cleanup_mmap:
	exit_mmap(mm);
	put_pgdir(mm);
bad_pgdir_cleanup_mm:
	mm_destroy(mm);
bad_mm:
	return ret;
}

// copy_thread - setup the trapframe on the  process's kernel stack top and
//             - setup the kernel entry point and stack of process
static void copy_thread(struct proc_struct *proc, uintptr_t esp,
			struct trapframe *tf)
{
	proc->tf = (struct trapframe *)(proc->kstack + KSTACKSIZE) - 1;
	*(proc->tf) = *tf;

	// Set a0 to 0 so a child process knows it's just forked
	proc->tf->gpr.a0 = 0;
	proc->tf->gpr.sp = (esp == 0) ? (uintptr_t)proc->tf - 4 : esp;

	proc->context.ra = (uintptr_t)forkret;
	proc->context.sp = (uintptr_t)(proc->tf);
}

/* do_fork -     parent process for a new child process
 * @clone_flags: used to guide how to clone the child process
 * @stack:       the parent's user stack pointer. if stack==0, It means to fork
 * a kernel thread.
 * @tf:          the trapframe info, which will be copied to child process's
 * proc->tf
 */
int do_fork(uint32_t clone_flags, uintptr_t stack, struct trapframe *tf)
{
	int ret = -E_NO_FREE_PROC;
	struct proc_struct *proc;
	if (nr_process >= MAX_PROCESS) {
		goto fork_out;
	}
	ret = -E_NO_MEM;
	//    1. call alloc_proc to allocate a proc_struct
	//    2. call setup_kstack to allocate a kernel stack for child process
	//    3. call copy_mm to dup OR share mm according clone_flag
	//    4. call copy_thread to setup tf & context in proc_struct
	//    5. insert proc_struct into hash_list && proc_list
	//    6. call wakeup_proc to make the new child process RUNNABLE
	//    7. set ret vaule using child proc's pid

	/* Some Functions
   *    set_links:  set the relation links of process.  ALSO SEE: remove_links:
   * lean the relation links of process
   *    -------------------
   *    update step 1: set child proc's parent to current process, make sure
   * current process's wait_state is 0 update step 5: insert proc_struct into
   * hash_list && proc_list, set the relation links of process
   */
	if ((proc = alloc_proc()) == NULL) {
		goto fork_out;
	}

	proc->parent = current;
	assert(current->wait_state == 0);

	if (setup_kstack(proc) != 0) {
		goto bad_fork_cleanup_proc;
	}
	if (copy_mm(clone_flags, proc) != 0) {
		goto bad_fork_cleanup_kstack;
	}
	copy_thread(proc, stack, tf);

	bool intr_flag;
	local_intr_save(intr_flag);
	{
		proc->pid = get_pid();
		hash_proc(proc);
		set_links(proc);
	}
	local_intr_restore(intr_flag);

	wakeup_proc(proc);

	ret = proc->pid;
fork_out:
	return ret;

bad_fork_cleanup_kstack:
	put_kstack(proc);
bad_fork_cleanup_proc:
	kfree(proc);
	goto fork_out;
}

// do_exit - called by sys_exit
//   1. call exit_mmap & put_pgdir & mm_destroy to free the almost all memory
//   space of process
//   2. set process' state as PROC_ZOMBIE, then call wakeup_proc(parent) to ask
//   parent reclaim itself.
//   3. call scheduler to switch to other process
int do_exit(int error_code)
{
	if (current == idleproc) {
		panic("idleproc exit.\n");
	}
	if (current == initproc) {
		panic("initproc exit.\n");
	}
	struct mm_struct *mm = current->mm;
	if (mm != NULL) {
		lsatp(boot_satp);
		if (mm_count_dec(mm) == 0) {
			exit_mmap(mm);
			put_pgdir(mm);
			mm_destroy(mm);
		}
		current->mm = NULL;
	}
	current->state = PROC_ZOMBIE;
	current->exit_code = error_code;
	bool intr_flag;
	struct proc_struct *proc;
	local_intr_save(intr_flag);
	{
		proc = current->parent;
		if (proc->wait_state == WT_CHILD) {
			wakeup_proc(proc);
		}
		while (current->cptr != NULL) {
			proc = current->cptr;
			current->cptr = proc->optr;

			proc->yptr = NULL;
			if ((proc->optr = initproc->cptr) != NULL) {
				initproc->cptr->yptr = proc;
			}
			proc->parent = initproc;
			initproc->cptr = proc;
			if (proc->state == PROC_ZOMBIE) {
				if (initproc->wait_state == WT_CHILD) {
					wakeup_proc(initproc);
				}
			}
		}
	}
	local_intr_restore(intr_flag);
	schedule();
	panic("do_exit will not return!! %d.\n", current->pid);
}

/* load_icode - load the content of binary program(ELF format) as the new
 * content of current process
 * @binary:  the memory addr of the content of binary program
 * @size:  the size of the content of binary program
 */
static int load_icode(unsigned char *binary, size_t size)
{
	if (current->mm != NULL) {
		panic("load_icode: current->mm must be empty.\n");
	}

	int ret = -E_NO_MEM;
	struct mm_struct *mm;
	//(1) create a new mm for current process
	if ((mm = mm_create()) == NULL) {
		goto bad_mm;
	}
	//(2) create a new PDT, and mm->pgdir= kernel virtual addr of PDT
	if (setup_pgdir(mm) != 0) {
		goto bad_pgdir_cleanup_mm;
	}
	//(3) copy TEXT/DATA section, build BSS parts in binary to memory space of
	//process
	struct Page *page = NULL;
	//(3.1) get the file header of the bianry program (ELF format)
	struct elfhdr *elf = (struct elfhdr *)binary;
	//(3.2) get the entry of the program section headers of the bianry program
	//(ELF format)
	struct proghdr *ph = (struct proghdr *)(binary + elf->e_phoff);
	//(3.3) This program is valid?
	if (elf->e_magic != ELF_MAGIC) {
		ret = -E_INVAL_ELF;
		goto bad_elf_cleanup_pgdir;
	}

	uint32_t vm_flags, perm;
	struct proghdr *ph_end = ph + elf->e_phnum;
	for (; ph < ph_end; ph++) {
		//(3.4) find every program section headers
		if (ph->p_type != ELF_PT_LOAD) {
			continue;
		}
		if (ph->p_filesz > ph->p_memsz) {
			ret = -E_INVAL_ELF;
			goto bad_cleanup_mmap;
		}
		if (ph->p_filesz == 0) {
			// continue ;
		}
		//(3.5) call mm_map fun to setup the new vma ( ph->p_va, ph->p_memsz)
		vm_flags = 0, perm = PTE_U | PTE_V;
		if (ph->p_flags & ELF_PF_X)
			vm_flags |= VM_EXEC;
		if (ph->p_flags & ELF_PF_W)
			vm_flags |= VM_WRITE;
		if (ph->p_flags & ELF_PF_R)
			vm_flags |= VM_READ;
		// modify the perm bits here for RISC-V
		if (vm_flags & VM_READ)
			perm |= PTE_R;
		if (vm_flags & VM_WRITE)
			perm |= (PTE_W | PTE_R);
		if (vm_flags & VM_EXEC)
			perm |= PTE_X;
		if ((ret = mm_map(mm, ph->p_va, ph->p_memsz, vm_flags, NULL)) !=
		    0) {
			goto bad_cleanup_mmap;
		}
		unsigned char *from = binary + ph->p_offset;
		size_t off, size;
		uintptr_t start = ph->p_va, end, la = ROUNDDOWN(start, PGSIZE);

		ret = -E_NO_MEM;

		//(3.6) alloc memory, and  copy the contents of every program section (from,
		//from+end) to process's memory (la, la+end)
		end = ph->p_va + ph->p_filesz;
		//(3.6.1) copy TEXT/DATA section of bianry program
		while (start < end) {
			if ((page = pgdir_alloc_page(mm->pgdir, la, perm)) ==
			    NULL) {
				goto bad_cleanup_mmap;
			}
			off = start - la, size = PGSIZE - off, la += PGSIZE;
			if (end < la) {
				size -= la - end;
			}
			memcpy(page2kva(page) + off, from, size);
			start += size, from += size;
		}

		//(3.6.2) build BSS section of binary program
		end = ph->p_va + ph->p_memsz;
		if (start < la) {
			/* ph->p_memsz == ph->p_filesz */
			if (start == end) {
				continue;
			}
			off = start + PGSIZE - la, size = PGSIZE - off;
			if (end < la) {
				size -= la - end;
			}
			memset(page2kva(page) + off, 0, size);
			start += size;
			assert((end < la && start == end) ||
			       (end >= la && start == la));
		}
		while (start < end) {
			if ((page = pgdir_alloc_page(mm->pgdir, la, perm)) ==
			    NULL) {
				goto bad_cleanup_mmap;
			}
			off = start - la, size = PGSIZE - off, la += PGSIZE;
			if (end < la) {
				size -= la - end;
			}
			memset(page2kva(page) + off, 0, size);
			start += size;
		}
	}
	//(4) build user stack memory
	vm_flags = VM_READ | VM_WRITE | VM_STACK;
	if ((ret = mm_map(mm, USTACKTOP - USTACKSIZE, USTACKSIZE, vm_flags,
			  NULL)) != 0) {
		goto bad_cleanup_mmap;
	}
	assert(pgdir_alloc_page(mm->pgdir, USTACKTOP - PGSIZE, PTE_USER) !=
	       NULL);
	assert(pgdir_alloc_page(mm->pgdir, USTACKTOP - 2 * PGSIZE, PTE_USER) !=
	       NULL);
	assert(pgdir_alloc_page(mm->pgdir, USTACKTOP - 3 * PGSIZE, PTE_USER) !=
	       NULL);
	assert(pgdir_alloc_page(mm->pgdir, USTACKTOP - 4 * PGSIZE, PTE_USER) !=
	       NULL);

	//(5) set current process's mm, sr3, and set SATP reg = physical addr of Page
	//Directory
	mm_count_inc(mm);
	current->mm = mm;
	current->satp = PADDR(mm->pgdir);
	lsatp(PADDR(mm->pgdir));

	//(6) setup trapframe for user environment
	struct trapframe *tf = current->tf;
	// Keep sstatus
	uintptr_t sstatus = tf->status;
	memset(tf, 0, sizeof(struct trapframe));
	tf->gpr.sp = USTACKTOP;
	tf->epc = elf->e_entry;
	tf->status = sstatus &
		     ~(SSTATUS_SPP | SSTATUS_SPIE); // spp位置0用户态  1内核态

	ret = 0;
out:
	return ret;
bad_cleanup_mmap:
	exit_mmap(mm);
bad_elf_cleanup_pgdir:
	put_pgdir(mm);
bad_pgdir_cleanup_mm:
	mm_destroy(mm);
bad_mm:
	goto out;
}

// do_execve - call exit_mmap(mm)&put_pgdir(mm) to reclaim memory space of
// current process
//           - call load_icode to setup new memory space accroding binary prog.
int do_execve(const char *name, size_t len, unsigned char *binary, size_t size)
{
	struct mm_struct *mm = current->mm;
	if (!user_mem_check(mm, (uintptr_t)name, len, 0)) {
		return -E_INVAL;
	}
	if (len > PROC_NAME_LEN) {
		len = PROC_NAME_LEN;
	}

	char local_name[PROC_NAME_LEN + 1];
	memset(local_name, 0, sizeof(local_name));
	memcpy(local_name, name, len);

	if (mm != NULL) {
		cputs("mm != NULL");
		lsatp(boot_satp);
		if (mm_count_dec(mm) == 0) {
			exit_mmap(mm);
			put_pgdir(mm);
			mm_destroy(mm);
		}
		current->mm = NULL;
	}
	int ret;
	if ((ret = load_icode(binary, size)) != 0) {
		goto execve_exit;
	}
	set_proc_name(current, local_name);
	return 0;

execve_exit:
	do_exit(ret);
	panic("already exit: %e.\n", ret);
}

// do_yield - ask the scheduler to reschedule
int do_yield(void)
{
	current->need_resched = 1;
	return 0;
}

// do_wait - wait one OR any children with PROC_ZOMBIE state, and free memory
// space of kernel stack
//         - proc struct of this child.
// NOTE: only after do_wait function, all resources of the child proces are
// free.
int do_wait(int pid, int *code_store)
{
	struct mm_struct *mm = current->mm;
	if (code_store != NULL) {
		if (!user_mem_check(mm, (uintptr_t)code_store, sizeof(int),
				    1)) {
			return -E_INVAL;
		}
	}

	struct proc_struct *proc;
	bool intr_flag, haskid;
repeat:
	haskid = 0;
	if (pid != 0) {
		proc = find_proc(pid);
		if (proc != NULL && proc->parent == current) {
			haskid = 1;
			if (proc->state == PROC_ZOMBIE) {
				goto found;
			}
		}
	} else {
		proc = current->cptr;
		for (; proc != NULL; proc = proc->optr) {
			haskid = 1;
			if (proc->state == PROC_ZOMBIE) {
				goto found;
			}
		}
	}
	if (haskid) {
		current->state = PROC_SLEEPING;
		current->wait_state = WT_CHILD;
		schedule();
		if (current->flags & PF_EXITING) {
			do_exit(-E_KILLED);
		}
		goto repeat;
	}
	return -E_BAD_PROC;

found:
	if (proc == idleproc || proc == initproc) {
		panic("wait idleproc or initproc.\n");
	}
	if (code_store != NULL) {
		*code_store = proc->exit_code;
	}
	local_intr_save(intr_flag);
	{
		unhash_proc(proc);
		remove_links(proc);
	}
	local_intr_restore(intr_flag);
	put_kstack(proc);
	kfree(proc);
	return 0;
}

// do_kill - kill process with pid by set this process's flags with PF_EXITING
int do_kill(int pid)
{
	struct proc_struct *proc;
	if ((proc = find_proc(pid)) != NULL) {
		if (!(proc->flags & PF_EXITING)) {
			proc->flags |= PF_EXITING;
			if (proc->wait_state & WT_INTERRUPTED) {
				wakeup_proc(proc);
			}
			return 0;
		}
		return -E_KILLED;
	}
	return -E_INVAL;
}

// kernel_execve - do SYS_exec syscall to exec a user program called by
// user_main kernel_thread
static int kernel_execve(const char *name, unsigned char *binary, size_t size)
{
	int64_t ret = 0, len = strlen(name);
	asm volatile("li a0, %1\n"
		     "lw a1, %2\n"
		     "lw a2, %3\n"
		     "lw a3, %4\n"
		     "lw a4, %5\n"
		     "li a7, 10\n"
		     "ebreak\n"
		     "sw a0, %0\n"
		     : "=m"(ret)
		     : "i"(SYS_exec), "m"(name), "m"(len), "m"(binary),
		       "m"(size)
		     : "memory");
	// cprintf("ret = %d\n", ret);
	return ret;
}

#define __KERNEL_EXECVE(name, binary, size)                          \
	({                                                           \
		cprintf("kernel_execve: pid = %d, name = \"%s\".\n", \
			current->pid, name);                         \
		kernel_execve(name, binary, (size_t)(size));         \
	})

#define KERNEL_EXECVE(x)                                                   \
	({                                                                 \
		extern unsigned char _binary_obj___user_##x##_out_start[], \
			_binary_obj___user_##x##_out_size[];               \
		__KERNEL_EXECVE(#x, _binary_obj___user_##x##_out_start,    \
				_binary_obj___user_##x##_out_size);        \
	})

// user_main - kernel thread used to exec a user program
static int user_main(void *arg)
{
	KERNEL_EXECVE(hello); // exec
	panic("user_main execve failed.\n");
}

// init_main - the second kernel thread used to create user_main kernel threads
static int init_main(void *arg)
{
	size_t nr_free_pages_store = nr_free_pages();
	// size_t kernel_allocated_store = kallocated();

	int pid = kernel_thread(user_main, NULL, 0); // 2 user_main
	if (pid <= 0) {
		panic("create user_main failed.\n");
	}

	while (do_wait(0, NULL) == 0) {
		schedule();
	}

	cprintf("all user-mode processes have quit.\n");
	assert(initproc->cptr == NULL && initproc->yptr == NULL &&
	       initproc->optr == NULL);
	assert(nr_process == 2);
	assert(list_next(&proc_list) == &(initproc->list_link));
	assert(list_prev(&proc_list) == &(initproc->list_link));

	cprintf("init check memory pass.\n");
	return 0;
}

// proc_init - set up the first kernel thread idleproc "idle" by itself and
//           - create the second kernel thread init_main
void proc_init(void)
{
	int i;

	list_init(&proc_list);
	for (i = 0; i < HASH_LIST_SIZE; i++) {
		list_init(hash_list + i);
	}

	if ((idleproc = alloc_proc()) == NULL) {
		panic("cannot alloc idleproc.\n");
	}

	idleproc->pid = 0;
	idleproc->state = PROC_RUNNABLE;
	idleproc->kstack = (uintptr_t)bootstack;
	idleproc->need_resched = 1;
	set_proc_name(idleproc, "idle");
	nr_process++;

	current = idleproc;

	int pid = kernel_thread(init_main, NULL, 0);
	if (pid <= 0) {
		panic("create init_main failed.\n");
	}

	initproc = find_proc(pid);
	set_proc_name(initproc, "init");

	assert(idleproc != NULL && idleproc->pid == 0);
	assert(initproc != NULL && initproc->pid == 1);
}

// cpu_idle - at the end of kern_init, the first kernel thread idleproc will do
// below works
void cpu_idle(void)
{
	while (1) {
		if (current->need_resched) {
			schedule();
		}
	}
}
