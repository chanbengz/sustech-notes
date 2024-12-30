# Compiler Principle (CS323)

Tutorial of the dragon book, with labs and projects, including lexical analysis (Regular Expression/NFA/DFA), syntax analysis (CFG), semantic analysis (AST), intermediate code generation (IR), optimization, and code generation (Assembly).

## About

- **Instructor:** `Yepang Liu (刘烨庞)`
- **Semester:** `2024 Fall`
- **Textbook:** `Compilers: Principles, Techniques, and Tools (The Dragon Book)`

## Content

- [Assignment](./Assignment): 4 theory assignment written in Typst, covering the first three topics.
- [Lab](./Lab): slides and flex/bison practices
- [Project](https://github.com/chanbengz/SUSTech_CS323_Project_Incredibuild): building toy compiler

| Lecture                            | Duration | Content                                                      |
| ---------------------------------- | -------- | ------------------------------------------------------------ |
| Introduction                       | 1 Week   | Compilation process                                          |
| Lexical Analysis                   | 3 Week   | Regular Expression<br>NFA/DFA                                |
| Syntax Analysis                    | 3 Week   | Context-Free Grammar<br>Recursive Descent Parser<br>LL(1), LR/CLR/SLR Parsers |
| Syntax-Directed Translation        | 3 Week   | SDD and Attributes<br>SDT                                    |
| Intermediate-Code Generation       | 2 Week   | Type Declaration and Checking<br>Control Flow                |
| Run-Time Environments              | 2 Week   | Storage Organization<br>Stack Space Allocation<br>Heap Management |
| Code Generation                    | 1 Week   | Addressing Mode<br>Basic Blocks<br>Register Allocation and Assignment |
| Introduction to Data-Flow Analysis | 1 Week   | Dataflow Analysis<br>Classic Dataflow Problems               |

## Lab

Lab sessions introduce tokenizer [flex](https://github.com/westes/flex) and parser generator [bison](https://www.gnu.org/software/bison/) with several practices. Also, advanced topics in lectures will continues in lab sessions.

## Project

At first, you need to investigate some aspects of programming languages, such as syntax design, features, compiler features, etc. Project section requires to implement a compiler for SUSTech Programming Language (SPL), which is a subset of C language. Also, you can choose to alternatively implement a self-design language. It's splitted into 4 phases

- Phase 1: Lexer and parser part of your compiler, which is able to parse the source codes and recognize the lexical and syntaxtic errors.
- Phase 2: Semantic analyser, needs to detect context errors, like undeclared variables, unmatched types.
- Phase 3: Intermediate generator, generates the Three Address Code (TAC) or other IRs like LLVM IR.
- Phase 4 (bonus): Code generator, generates assembly codes in some platforms (x86, MIPS, ARM, RISC-V).

Have fun with your compiler!
