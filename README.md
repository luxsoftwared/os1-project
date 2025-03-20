# **Multithreaded Kernel for RISC-V Emulator**

## **Project Overview**
This project is a simplified yet functional kernel for a multithreaded operating system, designed to run in a RISC-V emulation environment. It implements essential OS functionalities such as thread management, memory allocation, and synchronization using semaphores. It does not rely on any external libraries, which means that it refrains from utilizing system calls provided by the host operating system. More detailed project specification, operating modes, call hierarchy, and similar details can be seen in the PDF file ```Projektni zadatak 2024. v1.0.pdf```.

## **Features**
- **Thread Management:** Supports lightweight threads with context switching and scheduling.
- **Time Sharing:** Implements preemption using timer and keyboard interrupts.
- **Memory Allocation:** Provides dynamic memory allocation and deallocation.
- **Semaphore Synchronization:** Implements binary and counting semaphores.
- **System Calls API:** Exposes C and C++ APIs for interaction with the kernel.
- **Hardware Interaction:** Communicates with a simulated console and timer through system interrupts.

## **Technology Stack**
- **Programming Languages:** C, C++ (with minimal assembly for low-level operations)
- **Target Architecture:** RISC-V (RV64IMA variant)
- **Development Tools:** CLion, GNU Make, QEMU emulator

## **Project Structure**
```
.
├── /src              # Source code files 
├── /h                # Header files 
├── Makefile          # Build script  
└── README.md         # Project documentation 
```

## **Architecture Overview**
The kernel follows a layered structure for handling system calls and executing kernel functionalities:

1. **C++ API (`syscall_cpp.hpp`)**  
   - Provides an object-oriented interface for users.  
   - Implements C++ wrapper classes for threading (`Thread`), semaphores (`Semaphore`), and console interaction (`Console`).  
   - Calls the corresponding C API functions.

2. **C API (`syscall_c.hpp`)**  
   - Implements a procedural interface for system calls.  
   - Each function prepares arguments and invokes the **Application Binary Interface (ABI)** using an interrupt mechanism (`ecall`).

3. **ABI (Application Binary Interface)**  
   - Transfers system call parameters through CPU registers.  
   - Triggers a software interrupt (`ecall` instruction), switching execution from user mode to kernel mode.

4. **Interrupt Handling & Kernel Execution**  
   - The kernel's interrupt service routine (ISR) captures the system call.  
   - Based on the syscall number, the ISR invokes the corresponding kernel function (e.g., memory allocation, thread scheduling, semaphore operations).  
   - Kernel functions execute in **privileged mode**, directly managing hardware resources.

### **Execution Flow Example (Thread Start)**
C++ API -> C API -> ABI (registers) -> Software Interrupt (ecall) -> Kernel ISR -> Thread Manager

1. `Thread::start()` (C++ API) calls `thread_start(TCB* handle)` (C API).  
2. `thread_start(TCB* handle)` sets up arguments and calls `ecall`.  
3. The CPU switches to **privileged mode**, and the kernel ISR handles the interrupt, calling function from **Thread Manager**.  
4. The kernel’s **Thread Manager** readies the thread and schedules it for execution.  
5. Control returns to user mode after the system call completes.

## **Build the project using GNU Make**
The `make` command, when executing the `qemu` and `qemu-gdb` targets, starts the QEMU emulator, 
provides it with the configuration and executable file. The QEMU emulator then runs the executable program and connects to the development environment's terminal,
after which the running program will use that terminal as its standard input and output. After the program finishes execution, 
the QEMU emulator remains running and needs to be stopped. 

## **Usage**
The kernel initializes memory and starts executing the `userMain()` function. For the purpose of developing the project, currently `userMain` function calls testing functions. 
Developers can write their own code using functions provided by the `cpp_syscall.h` interface, and then call it from `userMain()` function.
