
## Basic Pointer

### 为啥需要 Pointer？

1. `更容易`在不同代码段之间`共享`信息，在不同代码段之间来回复制也是可以的。但用指针的形式更好。
2. `链式`数据结构, 如链表和二叉树。

### pointer dereference

指针必须要有指向的值，才可以 dereference。
没有指向的指针，dereference 时会 runtime error。

### NULL pointer

C -> NULL -> 0 -> false
java -> null

### pointer assignment

指针赋值

### copy

#### shallow copy

copy reference

#### deep copy

copy real value

## two levels

![two-levels](https://github.com/stardustman/pictures/raw/main/img/clang/memory_and_pointers-two-levels.svg)

### C 语言下指针探析

```c
long pointer(void * ptr){
    long *p = (long*)ptr;
    return *p;
}
```

```asm
pointer:
        pushq   %rbp
        movq    %rsp, %rbp
        movq    %rdi, -24(%rbp)
        movq    -24(%rbp), %rax
        movq    %rax, -8(%rbp)
        movq    -8(%rbp), %rax
        movq    (%rax), %rax
        popq    %rbp
        ret
```

![clang-pointer](https://github.com/stardustman/pictures/raw/main/img/clang/asm-pointer.png)

由上可知：`long *p = (long*)ptr` 被编译成

1. movq    -8(%rbp), %rax   %rax 这个寄存器里放的是传来的指针
2. movq    (%rax), %rax     (%rax) 寄存器`间接寻址`，也就是取寄存器 %rax 指向的内存里的值。这就是指针在汇编层面的意义。

## memory

variable -> variable name -> memory
编译后，变量名就没有了。

### local memory

> local memory is allocated automatically on function call and it is deallocated automatically when a function exits.

编译器管理 local memory，size 编译时决定。
Local memory 最根本的性质：相互独立，这是本地内存优点和缺点的根本原因。

#### stack frame

local memory 通过调用函数时创建的`栈帧`来体现。

##### allocation

栈帧创建时, local variable 一并创建。

##### deallocation

栈帧销毁时，local variable 一并销毁。

##### lifetime

随着函数栈帧同生共死。

##### local/stack/automatic variable

栈帧中的变量。
local/automatic 底层经常使用 stack 结构来实现。

![栈帧-lexical-scope](https://github.com/stardustman/pictures/raw/main/img/clang/clang-square-function.png)

##### advantage of locals

1. convenient. 函数需要临时的内存空间做计算，计算完毕后，释放。
2. efficient. 分配和释放都很快。
3. local copies.
   > pass by value
   > local parameters are basically local copies of the information from the caller.
   > caller 原值
   > callee 副本，如何修改不影响 caller 中的值
   > 软件设计原则：seperate components

##### disadvantage of locals

1. short lifetime. 和栈帧同生共死。(heap memory 解决)
2. restricted communication。caller's parameter -> callee 单向通信。(reference parameter 解决) 

### heap/dynamic memory

malloc()
free()

#### lifetime

程序员自主决定何时申请内存, 申请多少内存，何时释放内存。
runtime 决定。

##  parameter

### pass by value

local copy
caller parameter(原值)
callee parameter(副本)
separate components

### pass by reference

其实就是指针。这个二级结构。
pointer(上级)，把上级传出去，就可以指挥下级了。
pointee(下级)
一级指针也可以理解为 `hop`，非常像 router 中的下一跳。

## ownership

> each blok of memory has `exactly one` "owner" who takes responsibility for `deallocating` it.

### caller ownership

### callee allocated and return

## Reference

1. [memory and pointers](http://cslibrary.stanford.edu/102/)
2. [malloc](https://man7.org/linux/man-pages/man3/malloc.3.html)
3. [free](https://man7.org/linux/man-pages/man3/free.3p.html)
4. [square-function](https://godbolt.org/z/ncfo1q4M7)



