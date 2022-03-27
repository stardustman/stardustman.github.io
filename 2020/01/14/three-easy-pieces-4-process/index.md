> 我们知道一个计算机 `CPU` 数量有限, 但是却可以同时跑数量远远多于 `CPU` 数量的程序. 其实这就是 `virtualizing`. 怎样进行 `CPU` 虚拟化呢? 通过抽象出 `process` 这个基本概念, 也就是 `running program`. 一个没有运行的程序, 也就是磁盘上的一堆指令集和一些数据. 通过 `time sharing` 让一个 `process stop`, 然后去 `run` 另外一个 `process`. 重复这个 `context switch` 过程, 可以让多个程序同时运行, 这就是在`virtualizing CPU`.
> `mechanisms` are low-level methods or protocols that implements a needed piece of functionality. 类似于 java 中的接口, 就是我要提供什么功能. 比如 `OS` 中的进程的 `context switch`这个机制.
>`policies` are algorithms for making some kind of decision within the OS.类似 java 中实现类, 怎么实现这个接口定义的方法. 比如 `OS` 中的进程调度策略就是为了实现进程 `context switch`.
> `mechanisms` 和 `policies` 分离, 和 java 中接口和实现非常相似. 实际上这也是软件设计的原则. `OS` 本身也是软件.

## physical organization of a modern system

![现代计算机系统物理架构](https://github.com/stardustman/pictures/raw/main/img/physical-organization-of-a-modern-system.png)

## the abstracttion: a process

### address space

存放程序的 `instructions` 和 `data`.

### program counter

程序寄存器, 为了实现 `process` 的 `stop` 和 `run`, 需要知道执行到哪一条指令了.

### stack pointer

function parameters, local variables, return addresses

### io

进程默认打开 `stdin(0)`, `stdout(1)`, `stderr(2)` 这三个 `file descriptor`.

## process API

### create

### destroy

kill a process or halt it

### wait

### miscellaneous control

*suspend* process, stop it from running for a while
*resume* process, contine it running

### status

## process creation: a little detail

![loading-from-program-to-process.png](https://github.com/stardustman/pictures/raw/main/img/loading-from-program-to-process.png)
把程序从磁盘加载到内存. 问题来了:

1. 是要全部加载到内存吗? (eagerly)
2. 还是部分加载到内存? (lazy, paging)
3. 抑或内存根本不够存放整个程序? (swapping)
4. 启动程序的必要代码和数据加载了吗? (一系列 run-time 设置)
5. 分配供函数执行的 run-time stack(or just stack). 在 C 语言中, 用来存放 `function parameters`, `local variables`, `return addresses`, 想一想 `main` 的函数参数 `argc`,`argv`, 应该就是 `OS` 初始化的.
6. 分配/回收 heap. (malloc 和 free).
7. set up related I/O
8. execute program, 跳到 entry point, 也就是 main().

![程序加载执行流程](https://github.com/stardustman/pictures/raw/main/img/load-process.svg)

## process states

简单来说, 分为 3 种状态:

1. running

**a process is running on a processor, which means it is executing instructions**

2. ready

**a process is ready to run but for some reason the OS has chosen not run it at this given moment**

3. blocked

**a process has performmed some kind of operation that makes it not ready to run until some other event takes place. A common example: when a process initiates an I/O request to a disk, it becomes blocked and thus some other process can use the processor**

![process-state-transition](https://github.com/stardustman/pictures/raw/main/img/process-state-transition.png)

## data structures

> 以下是 xv6 这个 `MIT` 教学 `OS` 的 [proc.h](https://github.com/stardustman/xv6-public/blob/master/proc.h)

```c
// Per-CPU state
struct cpu {
  uchar apicid;                // Local APIC ID 
  struct context *scheduler;   // swtch() here to enter scheduler
  struct taskstate ts;         // Used by x86 to find stack for interrupt
  struct segdesc gdt[NSEGS];   // x86 global descriptor table , 拿着 GDT 使用内存
  volatile uint started;       // Has the CPU started?
  int ncli;                    // Depth of pushcli nesting.
  int intena;                  // Were interrupts enabled before pushcli?
  struct proc *proc;           // The process running on this cpu or null
};

extern struct cpu cpus[NCPU];
extern int ncpu;

//PAGEBREAK: 17
// Saved registers for kernel context switches.
// Don't need to save all the segment registers (%cs, etc),
// because they are constant across kernel contexts.
// Don't need to save %eax, %ecx, %edx, because the
// x86 convention is that the caller has saved them.
// Contexts are stored at the bottom of the stack they
// describe; the stack pointer is the address of the context.
// The layout of the context matches the layout of the stack in swtch.S
// at the "Switch stacks" comment. Switch doesn't save eip explicitly,
// but it is on the stack and allocproc() manipulates it.
struct context {
  uint edi;
  uint esi;
  uint ebx;
  uint ebp;
  uint eip;
};

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// Per-process state
struct proc {
  uint sz;                     // Size of process memory (bytes) 分配的内存大小
  pde_t* pgdir;                // Page table  页表
  char *kstack;                // Bottom of kernel stack for this process
  enum procstate state;        // Process state 进程状态
  int pid;                     // Process ID
  struct proc *parent;         // Parent process 父进程
  struct trapframe *tf;        // Trap frame for current syscall
  struct context *context;     // swtch() here to run process
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)
};

// Process memory is laid out contiguously, low addresses first:
//   text
//   original data and bss
//   fixed-size stack
//   expandable heap
```

## summary

low-level(mechanisms) 实现进程功能
high-level(policies)  调度策略
机制和策略合在一起, 实现 CPU 的虚拟化

## reference

1. [linux process](https://mp.weixin.qq.com/s/USb5e2Zoc0LRgRShRpTYfg)

2. [three easy pieces](http://pages.cs.wisc.edu/~remzi/OSTEP/Homework/homework.html)

3. [file descriptor](https://www.computerhope.com/jargon/f/file-descriptor.htm)

4. [xv6-proc](https://github.com/stardustman/xv6-public/blob/master/proc.h)

5. [中断](https://zhuanlan.zhihu.com/p/26464793)
