<h1 align="center">Terminology</h1>

**Please familiarize yourself with the following terms. It really helps**

## C Programming Language

### C is a general-purpose, procedural programming language developed in the early 1970s by Dennis Ritchie at Bell Labs. It is widely used for system programming, embedded programming, and application development

1. Compiler:
A compiler is a software tool that translates the high-level C code written by the programmer into machine code or intermediate code that can be executed by the computer's processor.

2. Variables:
Variables are used to store data in memory during the execution of a C program. They have a name, a data type, and a value that can be modified during runtime.

3. data Types:
In C, data types determine the type of data that a variable can hold, such as integers, floating-point numbers, characters, etc.

4. Functions:
Functions are blocks of code that perform specific tasks. They help modularize the code and improve code reusability.

5. Arrays:
An array is a collection of elements of the same data type stored in contiguous memory locations. It allows efficient access to multiple values using an index.

6. Pointers:
Pointers are variables that store memory addresses. They are extensively used for dynamic memory allocation and manipulating data directly in memory.

7. Control Structures:
Control structures such as if-else, switch-case, loops (for, while, do-while) control the flow of execution in a C program based on certain conditions.

8. Header Files:
Header files in C contain function prototypes, constants, and definitions that are used across multiple source files.

9. Structures:
Structures, also known as structs, are user-defined data types that group together related data elements under a single name.

10. File I/O:
File Input/Output functions in C allow reading from and writing to files on the disk, enabling data persistence beyond program execution.

11. Preprocessor Directives:
Preprocessor directives are instructions to the compiler that start with a hash (#) symbol, used to perform text manipulation before actual compilation.

12. Recursion:
Recursion is a programming technique where a function calls itself to solve a problem. It is useful for solving problems that can be broken down into smaller instances of the same problem.

13. Memory Management:
Memory management in C involves dynamically allocating and releasing memory using functions like malloc, calloc, realloc, and free.

14. Bitwise Operators:
Bitwise operators (AND, OR, XOR, etc.) manipulate individual bits of data, often used for low-level operations and optimizing code.

15. Command-Line Arguments:
Command-line arguments are parameters passed to a C program when it is executed, allowing external input to affect program behavior.

16. Standard Library:
The Standard Library in C provides a set of built-in functions and macros that are part of the C language specification and accessible through standard header files like <stdio.h>, < >, etc.

17. Macro:
A macro is a preprocessor directive that defines a code snippet or value, which is replaced directly in the code during compilation.

18. Enumerations (enums):
Enums are user-defined data types used to create named constants, providing more readability and maintainability to the code.

19. Type Casting:
Type casting is the process of converting one data type to another, either implicitly or explicitly, to perform certain operations.

## Assembly language

# Assembly language is a low-level programming language that uses mnemonic codes (abbreviations) to represent instructions that can be executed directly by a computer's CPU. It provides a more human-readable representation of machine code

1. Instruction: An instruction in assembly language represents a single operation that the CPU can execute. It includes operations such as arithmetic, logical, control flow, and data movement.

2. Opcode: The opcode (operation code) is a numerical value or mnemonic that identifies a specific instruction in assembly language. It tells the CPU which operation to perform.
3. Operand: An operand is a value or memory address on which an instruction operates. It can be a register, immediate value, or memory location.
4. Register: Registers are small, fast storage locations within the CPU used to hold data during program execution. They have specific purposes, such as storing intermediate results and function arguments.
5. Memory Addressing Modes: Memory addressing modes define how the CPU accesses operands in memory. Common modes include direct, indirect, indexed, and immediate addressing.
6. Label: A label is a symbolic name given to a memory address or instruction in assembly language. It simplifies the process of referring to memory locations and enables easy control flow.
7. Assembler: An assembler is a program that translates assembly language code into machine code (binary code) that the CPU can understand and execute.
8. Macro: A macro in assembly language is a named sequence of instructions that can be expanded inline wherever the macro is called. It simplifies repetitive code and enhances code reusability.
9. Directive: A directive in assembly language provides instructions to the assembler rather than the CPU. Directives are used to define constants, allocate memory, and include libraries.
10. Interrupt: An interrupt is a signal generated by hardware or software to temporarily halt the CPU's current execution and handle an event or request. Interrupts are used for multitasking and I/O operations.
11. Stack: The stack is a region of memory used for managing function calls and local variables. It follows the Last-In-First-Out (LIFO) principle and is crucial for managing program flow.
12. Flags: Flags are individual bits in the CPU's status register that reflect the outcome of certain arithmetic and logical operations. They are used to determine conditional branching in program flow.
13. Assembler Directives: Assembler directives are commands used in assembly language to provide instructions to the assembler. Examples include ORG (origin), DB (define byte), and EQU (equivalent).
14. LinkerThe linker is a program that combines multiple object files and libraries to generate an executable file. It resolves references between files and prepares the final program for execution.
15. Machine Code: Machine code is the binary representation of instructions that can be directly executed by the CPU. It is specific to the CPU architecture and is not human-readable.

## Operating System Development

### Operating System(OS)

An operating system is a software that manages computer hardware and provides services for computer programs. It acts as an intermediary between applications and hardware, handling tasks like memory management, process scheduling, and file system operations.

1. Kernel: The kernel is the core component of an operating system that directly interacts with the hardware. It manages resources, handles system calls, and provides essential services for other parts of the OS and user applications.
2. System Call: A system call is a mechanism that allows user-level programs to request services from the operating system. It provides an interface for applications to access low-level functionalities like I/O, memory allocation, and process management.
3. Process: A process is an independent, running instance of a program in execution. Each process has its memory space, program counter, and system resources, ensuring that they do not interfere with one another.
4. Thread: A thread is a lightweight unit of execution within a process. Threads within the same process share the same memory space, enabling more efficient concurrent execution.
5. Concurrency: Concurrency is the ability of an operating system to execute multiple tasks simultaneously. It can be achieved through multi-threading or multi-processing.
6. Scheduling: Scheduling is the process of determining the order in which processes or threads should be executed by the CPU. It aims to optimize resource utilization and overall system performance.
7. Memory Management: Memory management refers to the allocation and tracking of memory space for processes and data. The OS manages the organization of memory, ensuring that each process operates in its allocated space.
8. File System: The file system is a method used by the OS to organize and store data on disk or other storage devices. It provides a hierarchical structure and methods to read, write, and delete files.
9. Virtual Memory: Virtual memory is a memory management technique that allows processes to access more memory than physically available. It uses a combination of RAM and disk space to create the illusion of abundant memory.
10. Device Drivers: Device drivers are software components that enable the operating system to communicate with hardware devices. They provide an abstraction layer, allowing the OS to access diverse hardware through a common interface.
11. Interrupt: An interrupt is a signal generated by hardware or software to gain the attention of the CPU. It temporarily suspends the current execution to handle a specific event or task.
12. Bootstrap: Bootstrap is the initial process of loading the operating system into memory when a computer is powered on. It initializes the system and transfers control to the OS kernel.
13. File Descriptor: A file descriptor is a small non-negative integer that represents an open file within a process. It serves as a reference to the file, facilitating read and write operations.
14. Deadlock: Deadlock is a situation in which two or more processes are unable to proceed because each is waiting for the other to release a resource. This can lead to a standstill in the system.
15. Page Fault: A page fault occurs when a process tries to access a page in virtual memory that is not currently in RAM but is stored on the disk. The OS handles page faults by bringing the required page into memory.

    *These terms where generated by ChatGPT and are subject to change*

## Other

### This is where people can put new terms they used in the documentation of there contributed code
