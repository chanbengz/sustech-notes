#include <default_pmm.h>
#include <defs.h>
#include <error.h>
#include <kmalloc.h>
#include <memlayout.h>
#include <mmu.h>
#include <pmm.h>
#include <riscv.h>
#include <sbi.h>
#include <stdio.h>
#include <string.h>
#include <swap.h>
#include <sync.h>
#include <vmm.h>

// virtual address of physical page array
struct Page *pages;
// amount of physical memory (in pages)
size_t npage = 0;
// The kernel image is mapped at VA=KERNBASE and PA=info.base
uint_t va_pa_offset;
// memory starts at 0x80000000 in RISC-V
const size_t nbase = DRAM_BASE / PGSIZE;

// virtual address of boot-time page directory
pde_t *boot_pgdir = NULL;
// physical address of boot-time page directory
uintptr_t boot_satp;

// physical memory management
const struct pmm_manager *pmm_manager;

static void check_alloc_page(void);
static void check_pgdir(void);
static void check_boot_pgdir(void);

// init_pmm_manager - initialize a pmm_manager instance
static void init_pmm_manager(void)
{
	pmm_manager = &default_pmm_manager;
	cprintf("memory management: %s\n", pmm_manager->name);
	pmm_manager->init();
}

// init_memmap - call pmm->init_memmap to build Page struct for free memory
static void init_memmap(struct Page *base, size_t n)
{
	pmm_manager->init_memmap(base, n);
}

// alloc_pages - call pmm->alloc_pages to allocate a continuous n*PAGESIZE
// memory
struct Page *alloc_pages(size_t n)
{
	struct Page *page = NULL;
	bool intr_flag;

	while (1) {
		local_intr_save(intr_flag);
		{
			page = pmm_manager->alloc_pages(n);
		}
		local_intr_restore(intr_flag);

		if (page != NULL || n > 1 || swap_init_ok == 0)
			break;

		extern struct mm_struct *check_mm_struct;
		// cprintf("page %x, call swap_out in alloc_pages %d\n",page, n);
		swap_out(check_mm_struct, n, 0);
	}
	// cprintf("n %d,get page %x, No %d in alloc_pages\n",n,page,(page-pages));
	return page;
}

// free_pages - call pmm->free_pages to free a continuous n*PAGESIZE memory
void free_pages(struct Page *base, size_t n)
{
	bool intr_flag;
	local_intr_save(intr_flag);
	{
		pmm_manager->free_pages(base, n);
	}
	local_intr_restore(intr_flag);
}

// nr_free_pages - call pmm->nr_free_pages to get the size (nr*PAGESIZE)
// of current free memory
size_t nr_free_pages(void)
{
	size_t ret;
	bool intr_flag;
	local_intr_save(intr_flag);
	{
		ret = pmm_manager->nr_free_pages();
	}
	local_intr_restore(intr_flag);
	return ret;
}

/* pmm_init - initialize the physical memory management */
static void page_init(void)
{
	extern char kern_entry[];

	va_pa_offset = KERNBASE - 0x80200000;

	uint_t mem_begin = KERNEL_BEGIN_PADDR;
	uint_t mem_end = PHYSICAL_MEMORY_END - KERNEL_BEGIN_PADDR;
	uint_t mem_size = PHYSICAL_MEMORY_END;

	cprintf("physcial memory map:\n");
	cprintf("  memory: 0x%08lx, [0x%08lx, 0x%08lx].\n", mem_size, mem_begin,
		mem_end - 1);

	uint64_t maxpa = mem_end;

	if (maxpa > KERNTOP) {
		maxpa = KERNTOP;
	}

	extern char end[];

	npage = maxpa / PGSIZE;
	// BBL has put the initial page table at the first available page after the
	// kernel
	// so stay away from it by adding extra offset to end
	pages = (struct Page *)ROUNDUP((void *)end, PGSIZE);

	for (size_t i = 0; i < npage - nbase; i++) {
		SetPageReserved(pages + i);
	}

	uintptr_t freemem =
		PADDR((uintptr_t)pages + sizeof(struct Page) * (npage - nbase));

	mem_begin = ROUNDUP(freemem, PGSIZE);
	mem_end = ROUNDDOWN(mem_end, PGSIZE);
	if (freemem < mem_end) {
		init_memmap(pa2page(mem_begin), (mem_end - mem_begin) / PGSIZE);
	}
	// cprintf("vapaofset is %llu\n",va_pa_offset);
}
static void enable_paging(void)
{
	write_csr(satp, 0x8000000000000000 | (boot_satp >> RISCV_PGSHIFT));
}

// boot_map_segment - setup&enable the paging mechanism
// parameters
//  la:   linear address of this memory need to map (after x86 segment map)
//  size: memory size
//  pa:   physical address of this memory
//  perm: permission of this memory
static void boot_map_segment(pde_t *pgdir, uintptr_t la, size_t size,
			     uintptr_t pa, uint32_t perm)
{
	assert(PGOFF(la) == PGOFF(pa));
	size_t n = ROUNDUP(size + PGOFF(la), PGSIZE) / PGSIZE;
	la = ROUNDDOWN(la, PGSIZE);
	pa = ROUNDDOWN(pa, PGSIZE);
	for (; n > 0; n--, la += PGSIZE, pa += PGSIZE) {
		pte_t *ptep = get_pte(pgdir, la, 1);
		assert(ptep != NULL);
		*ptep = pte_create(pa >> PGSHIFT, PTE_V | perm);
	}
}

// boot_alloc_page - allocate one page using pmm->alloc_pages(1)
// return value: the kernel virtual address of this allocated page
// note: this function is used to get the memory for PDT(Page Directory
// Table)&PT(Page Table)
static void *boot_alloc_page(void)
{
	struct Page *p = alloc_page();
	if (p == NULL) {
		panic("boot_alloc_page failed.\n");
	}
	return page2kva(p);
}

// pmm_init - setup a pmm to manage physical memory, build PDT&PT to setup
// paging mechanism
//         - check the correctness of pmm & paging mechanism, print PDT&PT
void pmm_init(void)
{
	// We need to alloc/free the physical memory (granularity is 4KB or other
	// size).
	// So a framework of physical memory manager (struct pmm_manager)is defined
	// in pmm.h
	// First we should init a physical memory manager(pmm) based on the
	// framework.
	// Then pmm can alloc/free the physical memory.
	// Now the first_fit/best_fit/worst_fit/buddy_system pmm are available.
	init_pmm_manager();

	// detect physical memory space, reserve already used memory,
	// then use pmm->init_memmap to create free page list
	page_init();

	// use pmm->check to verify the correctness of the alloc/free function in a
	// pmm
	// check_alloc_page();
	// create boot_pgdir, an initial page directory(Page Directory Table, PDT)
	extern char boot_page_table_sv39[];
	boot_pgdir = (pte_t *)boot_page_table_sv39;
	boot_satp = PADDR(boot_pgdir);

	// check_pgdir();

	static_assert(KERNBASE % PTSIZE == 0 && KERNTOP % PTSIZE == 0);

	// now the basic virtual memory map(see memalyout.h) is established.
	// check the correctness of the basic virtual memory map.
	// check_boot_pgdir();

	// kmalloc_init();
}

// get_pte - get pte and return the kernel virtual address of this pte for la
//        - if the PT contians this pte didn't exist, alloc a page for PT
// parameter:
//  pgdir:  the kernel virtual base address of PDT
//  la:     the linear address need to map
//  create: a logical value to decide if alloc a page for PT
// return vaule: the kernel virtual address of this pte
pte_t *get_pte(pde_t *pgdir, uintptr_t la, bool create)
{
	/* LAB2 EXERCISE 2: YOUR CODE
   *
   * If you need to visit a physical address, please use KADDR()
   * please read pmm.h for useful macros
   *
   * Maybe you want help comment, BELOW comments can help you finish the code
   *
   * Some Useful MACROs and DEFINEs, you can use them in below implementation.
   * MACROs or Functions:
   *   PDX(la) = the index of page directory entry of VIRTUAL ADDRESS la.
   *   KADDR(pa) : takes a physical address and returns the corresponding
   * kernel virtual address.
   *   set_page_ref(page,1) : means the page be referenced by one time
   *   page2pa(page): get the physical address of memory which this (struct
   * Page *) page  manages
   *   struct Page * alloc_page() : allocation a page
   *   memset(void *s, char c, size_t n) : sets the first n bytes of the
   * memory area pointed by s
   *                                       to the specified value c.
   * DEFINEs:
   *   PTE_P           0x001                   // page table/directory entry
   * flags bit : Present
   *   PTE_W           0x002                   // page table/directory entry
   * flags bit : Writeable
   *   PTE_U           0x004                   // page table/directory entry
   * flags bit : User can access
   */
	pde_t *pdep1 = &pgdir[PDX1(la)];
	if (!(*pdep1 & PTE_V)) {
		struct Page *page;
		if (!create || (page = alloc_page()) == NULL) {
			return NULL;
		}
		set_page_ref(page, 1);
		uintptr_t pa = page2pa(page);
		memset(KADDR(pa), 0, PGSIZE);
		*pdep1 = pte_create(page2ppn(page), PTE_U | PTE_V);
	}
	pde_t *pdep0 = &((pte_t *)KADDR(PDE_ADDR(*pdep1)))[PDX0(la)];
	if (!(*pdep0 & PTE_V)) {
		struct Page *page;
		if (!create || (page = alloc_page()) == NULL) {
			return NULL;
		}
		set_page_ref(page, 1);
		uintptr_t pa = page2pa(page);
		memset(KADDR(pa), 0, PGSIZE);
		*pdep0 = pte_create(page2ppn(page), PTE_U | PTE_V);
	}
	return &((pte_t *)KADDR(PDE_ADDR(*pdep0)))[PTX(la)];
}

// get_page - get related Page struct for linear address la using PDT pgdir
struct Page *get_page(pde_t *pgdir, uintptr_t la, pte_t **ptep_store)
{
	pte_t *ptep = get_pte(pgdir, la, 0);
	if (ptep_store != NULL) {
		*ptep_store = ptep;
	}
	if (ptep != NULL && *ptep & PTE_V) {
		return pte2page(*ptep);
	}
	return NULL;
}

// page_remove_pte - free an Page sturct which is related linear address la
//                - and clean(invalidate) pte which is related linear address la
// note: PT is changed, so the TLB need to be invalidate
static inline void page_remove_pte(pde_t *pgdir, uintptr_t la, pte_t *ptep)
{
	/* LAB2 EXERCISE 3: YOUR CODE
   *
   * Please check if ptep is valid, and tlb must be manually updated if
   * mapping is updated
   *
   * Maybe you want help comment, BELOW comments can help you finish the code
   *
   * Some Useful MACROs and DEFINEs, you can use them in below implementation.
   * MACROs or Functions:
   *   struct Page *page pte2page(*ptep): get the according page from the
   * value of a ptep
   *   free_page : free a page
   *   page_ref_dec(page) : decrease page->ref. NOTICE: ff page->ref == 0 ,
   * then this page should be free.
   *   tlb_invalidate(pde_t *pgdir, uintptr_t la) : Invalidate a TLB entry,
   * but only if the page tables being
   *                        edited are the ones currently in use by the
   * processor.
   * DEFINEs:
   *   PTE_P           0x001                   // page table/directory entry
   * flags bit : Present
   */
	if (*ptep & PTE_V) { //(1) check if this page table entry is
		struct Page *page =
			pte2page(*ptep); //(2) find corresponding page to pte
		page_ref_dec(page); //(3) decrease page reference
		if (page_ref(page) ==
		    0) { //(4) and free this page when page reference reachs 0
			free_page(page);
		}
		*ptep = 0; //(5) clear second page table entry
		tlb_invalidate(pgdir, la); //(6) flush tlb
	}
}

// page_remove - free an Page which is related linear address la and has an
// validated pte
void page_remove(pde_t *pgdir, uintptr_t la)
{
	pte_t *ptep = get_pte(pgdir, la, 0);
	if (ptep != NULL) {
		page_remove_pte(pgdir, la, ptep);
	}
}

// page_insert - build the map of phy addr of an Page with the linear addr la
// paramemters:
//  pgdir: the kernel virtual base address of PDT
//  page:  the Page which need to map
//  la:    the linear address need to map
//  perm:  the permission of this Page which is setted in related pte
// return value: always 0
// note: PT is changed, so the TLB need to be invalidate
int page_insert(pde_t *pgdir, struct Page *page, uintptr_t la, uint32_t perm)
{
	pte_t *ptep = get_pte(pgdir, la, 1);
	if (ptep == NULL) {
		return -E_NO_MEM;
	}
	page_ref_inc(page);
	if (*ptep & PTE_V) {
		struct Page *p = pte2page(*ptep);
		if (p == page) {
			page_ref_dec(page);
		} else {
			page_remove_pte(pgdir, la, ptep);
		}
	}
	*ptep = pte_create(page2ppn(page), PTE_V | perm);
	tlb_invalidate(pgdir, la);
	return 0;
}

// invalidate a TLB entry, but only if the page tables being
// edited are the ones currently in use by the processor.
void tlb_invalidate(pde_t *pgdir, uintptr_t la)
{
	// flush_tlb();
	// The flush_tlb flush the entire TLB, is there any better way?
	asm volatile("sfence.vma %0" : : "r"(la));
}

// pgdir_alloc_page - call alloc_page & page_insert functions to
//                  - allocate a page size memory & setup an addr map
//                  - pa<->la with linear address la and the PDT pgdir
struct Page *pgdir_alloc_page(pde_t *pgdir, uintptr_t la, uint32_t perm)
{
	struct Page *page = alloc_page();
	if (page != NULL) {
		if (page_insert(pgdir, page, la, perm) != 0) {
			free_page(page);
			return NULL;
		}
		if (swap_init_ok) {
			swap_map_swappable(check_mm_struct, la, page, 0);
			page->pra_vaddr = la;
			assert(page_ref(page) == 1);
			// cprintf("get No. %d  page: pra_vaddr %x, pra_link.prev %x,
			// pra_link_next %x in pgdir_alloc_page\n", (page-pages),
			// page->pra_vaddr,page->pra_page_link.prev,
			// page->pra_page_link.next);
		}
	}

	return page;
}

static void check_alloc_page(void)
{
	pmm_manager->check();
	cprintf("check_alloc_page() succeeded!\n");
}

static void check_pgdir(void)
{
	// assert(npage <= KMEMSIZE / PGSIZE);
	// The memory starts at 2GB in RISC-V
	// so npage is always larger than KMEMSIZE / PGSIZE
	assert(npage <= KERNTOP / PGSIZE);
	assert(boot_pgdir != NULL && (uint32_t)PGOFF(boot_pgdir) == 0);
	assert(get_page(boot_pgdir, 0x0, NULL) == NULL);

	struct Page *p1, *p2;
	p1 = alloc_page();
	assert(page_insert(boot_pgdir, p1, 0x0, 0) == 0);

	pte_t *ptep;
	assert((ptep = get_pte(boot_pgdir, 0x0, 0)) != NULL);
	assert(pte2page(*ptep) == p1);
	assert(page_ref(p1) == 1);

	ptep = (pte_t *)KADDR(PDE_ADDR(boot_pgdir[0]));
	ptep = (pte_t *)KADDR(PDE_ADDR(ptep[0])) + 1;
	assert(get_pte(boot_pgdir, PGSIZE, 0) == ptep);

	p2 = alloc_page();
	assert(page_insert(boot_pgdir, p2, PGSIZE, PTE_U | PTE_W) == 0);
	assert((ptep = get_pte(boot_pgdir, PGSIZE, 0)) != NULL);
	assert(*ptep & PTE_U);
	assert(*ptep & PTE_W);
	assert(boot_pgdir[0] & PTE_U);
	assert(page_ref(p2) == 1);

	assert(page_insert(boot_pgdir, p1, PGSIZE, 0) == 0);
	assert(page_ref(p1) == 2);
	assert(page_ref(p2) == 0);
	assert((ptep = get_pte(boot_pgdir, PGSIZE, 0)) != NULL);
	assert(pte2page(*ptep) == p1);
	assert((*ptep & PTE_U) == 0);

	page_remove(boot_pgdir, 0x0);
	assert(page_ref(p1) == 1);
	assert(page_ref(p2) == 0);

	page_remove(boot_pgdir, PGSIZE);
	assert(page_ref(p1) == 0);
	assert(page_ref(p2) == 0);

	assert(page_ref(pde2page(boot_pgdir[0])) == 1);
	free_page(pde2page(boot_pgdir[0]));
	boot_pgdir[0] = 0;

	cprintf("check_pgdir() succeeded!\n");
}

static void check_boot_pgdir(void)
{
	pte_t *ptep;
	int i;
	for (i = ROUNDDOWN(KERNBASE, PGSIZE); i < npage * PGSIZE; i += PGSIZE) {
		assert((ptep = get_pte(boot_pgdir, (uintptr_t)KADDR(i), 0)) !=
		       NULL);
		assert(PTE_ADDR(*ptep) == i);
	}

	assert(boot_pgdir[0] == 0);

	struct Page *p;
	p = alloc_page();
	assert(page_insert(boot_pgdir, p, 0x100, PTE_W | PTE_R) == 0);
	assert(page_ref(p) == 1);
	assert(page_insert(boot_pgdir, p, 0x100 + PGSIZE, PTE_W | PTE_R) == 0);
	assert(page_ref(p) == 2);

	const char *str = "ucore: Hello world!!";
	strcpy((void *)0x100, str);
	assert(strcmp((void *)0x100, (void *)(0x100 + PGSIZE)) == 0);

	*(char *)(page2kva(p) + 0x100) = '\0';
	assert(strlen((const char *)0x100) == 0);

	free_page(p);
	free_page(pde2page(boot_pgdir[0]));
	boot_pgdir[0] = 0;

	cprintf("check_boot_pgdir() succeeded!\n");
}

// perm2str - use string 'u,r,w,-' to present the permission
static const char *perm2str(int perm)
{
	static char str[4];
	str[0] = (perm & PTE_U) ? 'u' : '-';
	str[1] = 'r';
	str[2] = (perm & PTE_W) ? 'w' : '-';
	str[3] = '\0';
	return str;
}

// get_pgtable_items - In [left, right] range of PDT or PT, find a continuous
// linear addr space
//                  - (left_store*X_SIZE~right_store*X_SIZE) for PDT or PT
//                  - X_SIZE=PTSIZE=4M, if PDT; X_SIZE=PGSIZE=4K, if PT
// paramemters:
//  left:        no use ???
//  right:       the high side of table's range
//  start:       the low side of table's range
//  table:       the beginning addr of table
//  left_store:  the pointer of the high side of table's next range
//  right_store: the pointer of the low side of table's next range
//  return value: 0 - not a invalid item range, perm - a valid item range with
//  perm permission
static int get_pgtable_items(size_t left, size_t right, size_t start,
			     uintptr_t *table, size_t *left_store,
			     size_t *right_store)
{
	if (start >= right) {
		return 0;
	}
	while (start < right && !(table[start] & PTE_V)) {
		start++;
	}
	if (start < right) {
		if (left_store != NULL) {
			*left_store = start;
		}
		int perm = (table[start++] & PTE_USER);
		while (start < right && (table[start] & PTE_USER) == perm) {
			start++;
		}
		if (right_store != NULL) {
			*right_store = start;
		}
		return perm;
	}
	return 0;
}
