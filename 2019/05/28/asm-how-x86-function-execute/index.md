> 前一阵子去看 java 虚拟机原理, 忽然痛悟到虚拟机也是机器啊, 呵呵也就是个软件而已. 看到 java 方法调用太复杂. 字节码那一套又不太熟悉, 还不如直接去看 C 编译后的汇编代码.
> 目的: 搞明白 X86 架构下函数到底是怎么调用执行的.

# assembly syntax for X86

## gas (gnu assembler syntax), 也就是 AT&T 风格. 

本文采用该风格.

```cpp
swap(int, int):
        pushq   %rbp
        movq    %rsp, %rbp
        movl    %edi, -20(%rbp)
        movl    %esi, -24(%rbp)
        movl    -20(%rbp), %eax
        movl    %eax, -4(%rbp)
        movl    -24(%rbp), %eax
        movl    %eax, -20(%rbp)
        movl    -4(%rbp), %eax
        movl    %eax, -24(%rbp)
        nop
        popq    %rbp
        ret
```

## intel syntax

```x86asm
swap(int, int):
        push    rbp
        mov     rbp, rsp
        mov     DWORD PTR [rbp-20], edi
        mov     DWORD PTR [rbp-24], esi
        mov     eax, DWORD PTR [rbp-20]
        mov     DWORD PTR [rbp-4], eax
        mov     eax, DWORD PTR [rbp-24]
        mov     DWORD PTR [rbp-20], eax
        mov     eax, DWORD PTR [rbp-4]
        mov     DWORD PTR [rbp-24], eax
        nop
        pop     rbp
        ret
```

# instruction suffixes

| 缩写 | 全称 | 位数  |
| ---- | ---- | ----- |
| b    | byte | 8bit  |
| w    | word | 16bit |
| l    | long | 32bit |
| q    | quad | 64bit |

# addressing mode(寻址模式)

> CPU 寻址方式, 也就是拿到数据的方式.

## direct addressing(直接寻址)

> movb $0x05,%al  
> 表示为:R[al] = 0x05;  
> 将立即数 0x05(1 byte) 复制到寄存器 al  

## indirect addressing(间接寻址)

> 间接寻址也就是到内存里去找

### register to memory

> movl %eax, -4(%ebp)  
> 表示为: mem[R[ebp]-4] = R[eax];  
> 将寄存器 eax 里面的值复制到寄存器 ebp 的值减去 4 指向的内存地址处(也就是 R[ebp] -4 的值是一个内存地址).   
> 通过寄存器指向了内存地址, 是不是很熟悉的指针啊, 对, 就是指针. C 语言的指针就是这么玩的啊!

### memory to register

> movl -4(%ebp), %eax  
> %eax 表示为: R[eax] = mem[R[ebp] -4];  
> 将寄存器 esp 的值减去 4 的值指向的内存地址处存放的值, 复制到寄存器 eax  

# program counter for stored program

> PC = PC + \(instruction size in bytes\)  
> (instruction) (src1) (src2) (dst)  
> In most processors, the PC is `incremented after fetching` an instruction,
> and holds the memory address of ("points to") the next instruction that would be executed.   
> 这里就用到了指令周期(instruction cycle)这个概念了, fetch, decode, execute.
> 注意到 PC 这个寄存器, 在 CPU fetch 了一条指令后就自动增加了.
> (In a processor where the incrementation precedes the fetch, the PC points to the current instruction being executed.)  
> 同样的在 CPU fetch 一条指令之前, PC 指向当前正在执行的指令.  
> 注意: 不直接操作 ip(instruction pointer) 也叫 pc(program counter) 这个寄存器, 如果这个能被编译器直接操作的话, 就完全想跳到哪执行就跳到哪执行了. 
> 实际上 `call` 和 `ret` 指令就是在间接操作 `pc` 这个寄存器. 
> call 带来的效果之一就是 push %rip, ret 带来的效果之一就是 pop %rip. 两者具有对称作用啊!

# change control flow(改变控制流)

## `jmp` label

> When a jump instruction executes (in the last step of the machine cycle), it puts a new address into the PC. Now the fetch at the top of the next machine cycle fetches the instruction at that new address. Instead of executing the instruction that follows the jump instruction in memory, the processor "jumps" to an instruction somewhere else in memory.  
> jmp 指令把 label 所在的地址, 复制给 pc 寄存器. 这就改变了程序的控制流. 然后程序流程就脱离了原来的执行流. 和 call label 很相似, 对, call 指令作用之一就包括了一个隐式的 jmp label. 
> 函数调用也就是把控制权交给了被调用者. 但是控制权最后要回到调用函数那里. 只不过 call 指令在函数交出控制权之前还多干了一件事, 就是把此时的 pc 值 push 到了栈里. 

# stack management 

## stack pointer

> A stack register is a computer central processor register whose purpose is to keep track of a call stack.  
> push pop 指令操作的是 sp(stack pointer) 这个寄存器.  
> 栈底地址: 由bp(base pointer) 保存  
> 栈分配空间: sp 减去需要的地址空间大小(所谓的栈向下生长);   
> 栈回收空间: sp 加上需要的地址空间大小(所谓的栈向上收缩);(PS: 相当无聊的话)  

![x86-64-stack](https://github.com/stardustman/pictures/raw/main/img/x86-64-stack.png) #(x86-64-stack)

## `pushl` %eax

> push value of %eax onto stack
> The push instruction places its operand onto the top of the hardware supported stack in memory. Specifically, push first decrements ESP by 4, then places its operand into the contents of the 32-bit location at address [ESP]. ESP (the stack pointer) is decremented by push since the x86 stack grows down - i.e. the stack grows from high addresses to lower addresses.  
> 这里可以看到 push 的是多字节的数据, 那就涉及到怎样排列多字节数据的问题了. 也就是所谓的字节序的问题. X86 采用所谓的小端, 也就是把数字按照顺序放到栈里, 数字的高位放在了比较大的内存地址那里.(这里不做讨论)
作用等价于

```asm
subl $4, %esp ;分配4个字节的空间, 所谓的栈向下生长
movl %eax, (%esp) ;将 eax 的值复制到 esp 指到的内存地址处
```

## `popl` %eax

> pop %eax off stack
> The pop instruction removes the 4-byte data element from the top of the hardware-supported stack into the specified operand (i.e. register or memory location). It first moves the 4 bytes located at memory location [ESP] into the specified register or memory location, and then increments SP by 4. 
作用等价于

```asm
movl (%esp),%eax ;将 esp 指向的内存地址里面的值复制到 eax
addl $4,%esp ;回收空间
```

# function call and return

## `call` <label>

> The call instruction first pushes the current code location onto the hardware supported stack in memory(see the push instruction for details), and then performs an unconditional jump to the code location indicated by the label operand. Unlike the simple jump instructions, the call instruction saves the location to return to when the subroutine completes.  
> 注意到 CPU 在 fetch 到 call 指令后, PC 就已经自动加 1 了. 此时的 PC 值也就是所谓的函数返回地址. 
> call 指令做了两件事, 第一件事: 将此时的 ip 保存到栈中, 第二件事: jump 到 label 位置, 此时已经改变了 PC 的值.  
> call label 
作用等价于 
 
```asm
pushq %rip  
jmp label 
``` 

## `ret`

> The ret instruction implements a subroutine return mechanism. This instruction first pops a code location off the hardware supported in-memory stack (也就是 call 指令压入栈中的 PC, 将这个值复制到 PC 寄存器)(see the pop instruction for details). It then performs an unconditional jump to the retrieved code location.  
> 所以啊, call(含有一个 push 操作) 和 ret(含有一个 pop 操作) 指令, 这是实现控制流跳转和恢复的关键. 也间接操作了 sp 这个寄存器. 硬件实现的功能, 不需要过多的计较.  
> ret 

作用等价于:

```gas
popq %rip
```

# call stack

> In computer science, a call stack is a stack data structure that stores information about the active subroutines of a computer program. This kind of stack is also known as an execution stack, program stack, control stack, run-time stack, or machine stack, and is often shortened to just "the stack".  
> A call stack is used for several related purposes, but the main reason for having one is to keep track of the point to which each active subroutine should return control when it finishes executing.   
> An active subroutine is one that has been called but is yet to complete execution after which control should be handed back to the point of call. Such activations of subroutines may be nested to any level (recursive as a special case), hence the stack structure.

## example

> for example, a subroutine DrawSquare calls a subroutine DrawLine from four different places, DrawLine must know where to return when its execution completes.  
> To accomplish this, the address following the instruction that jumps to DrawLine, the return address, is pushed onto the call stack with each call.

![callstack-layout-for-upward-growing-stacks](https://github.com/stardustman/pictures/raw/main/img/callstack-layout-for-upward-growing-stacks.png)  
#(callstack-layout-for-upward-growing-stacks)

# code analysis

```c
void swap(int a, int b){
    int tmp = a;
    a = b;
    b = tmp;
}
```

```asm
; 64 bit 机器 , AT&T 风格的汇编
swap(int, int):
        pushq   %rbp ; 上一个栈帧(main)的基地址压栈 等价于 subq $8, %rsp; movq %rbp,(%rsp)
        movq    %rsp, %rbp ; 开辟新的函数栈帧, 也就是形成一个新的栈的基地址
        movl    %edi, -20(%rbp) ; 参数 a
        movl    %esi, -24(%rbp) ; 参数 b
        movl    -20(%rbp), %eax ; 把 a 赋值给 %eax
        movl    %eax, -4(%rbp)  ; 把 %eax (a)赋值给 %rbp - 4(a) 的地址处
        movl    -24(%rbp), %eax ; 把 b 赋值给 % eax（b）
        movl    %eax, -20(%rbp) ; 把 %eax (b) 赋值给 %rbp - 20（b） 的地址处,完成 b 的交换
        movl    -4(%rbp), %eax  ; 把 %rbp - 4 地址处的值(a) 赋值给 %eax (a)
        movl    %eax, -24(%rbp) ; 把 %eax (a) 赋值给 %rbp - 24 的地址处, 完成 a 的交换
        nop ; 延时
        popq    %rbp ; 等价于 movq (%rsp), %rbp ; 上一个函数栈帧(main)的基地址恢复; addq $8, %rsp ; 上一个函数的 %rsp 恢复
        ret ; 1. popq %rip. (恢复 main 的 pc, call swap 这条指令压入的 pc ) 2. jmp % rip 处继续执行.(也就是 movl $0, %eax 这条指令的地址)
```

```cpp
int main() {
    swap(1, 2);
    return 0;
}
```

```asm
main:
        pushq   %rbp
        movq    %rsp, %rbp
        movl    $2, %esi ; 由 caller(main 函数) 准备函数参数 2
        movl    $1, %edi ; 由 caller(main 函数) 准备函数参数 1
        call    swap ; 在 CPU fetch 了 call 指令后, pc 已经指向了下一条指令, 也就是 movl $0, %eax 这条指令. 此时的 call 指令完成了两件事, 第一件事: 将 pc(old) 压入到栈中(swap 函数 ret 指令(函数返回)就是把这个 pc(old) pop 到 pc 这个寄存器, CPU 就能接着执行 movl $0, %eax 这条指令了), 第二件事: jump 到swap的地址, 开始执行swap的代码.
        movl    $0, %eax ; 返回值 0 
        popq    %rbp
        ret
```

# C compare to  Assembly

![c-swap-to-asm-swap](https://github.com/stardustman/pictures/raw/main/img/c-swap-to-asm-swap.png)

# asm execute graph

![asm-execute-graph](https://github.com/stardustman/pictures/raw/main/img/asm-execute-graph.png)

> 注意: 示意图里面的是 64 bit 的汇编代码.  
  注意: 所有的 push 和 pop 指令都会改变 sp 寄存器的值.  
  图1 main 函数执行完 pushq %rbp 和 movq %rsp, %rbp, 开辟 main 函数的栈帧.  
  图2 main 函数执行 call swap. call 指令两个作用: 1. 将 movl $0, %eax 这条指令的地址(X)压入栈中. 2. jump 到 swap 的地址.  
  图3 是 swap 函数的栈帧, 此时新函数的栈帧 rsp 和 rbp 指向的是相同的内存地址.  
  图4 所有的 mov 使用的内存地址, 都是通过 rbp 来偏移得到, rbp 的值并没有发生改变.   
  图5 执行完 popq %rsp, 恢复 main 函数的栈基址(rbp), 也就是和图1 一样.  
  图6 执行完 ret 恢复为 main 函数的栈帧(这里主要是 rsp, rbp, pc, 个人理解把 pc 视为栈帧的一部分, 因为函数调用控制权发生转移, 幕后也离不开 pc 这个寄存器的变化).  
  ret 的作用等价于 popq %rip. 但是无法直接操作 ip(pc) 这个寄存器.  
  也就相当于间接改变 ip. 此时 pc 已被 ret 指令恢复成了 X. (此时实际上控制权已经回到 main 函数了), 接下来就是继续执行 main 函数的代码.  
  其实 swap 函数的栈帧已经被销毁了. 也就是再也访问不到 swap 函数里的变量了. 这就是 C 语言里的所谓的本地变量的本质.  
  注意: 图1 和 图6 , 图2 和 图5 完全一样, 这不是有意为之, 按照 X86 的函数调用机制就是这样的.    
  在被调用函数(swap)执行 popq % rbp, 这条指令就是要恢复调用函数(main)的 rbp, 执行 ret 这条指令就是要恢复调用函数(main)的下一条指令的地址.    
  也就是将 pc 的值恢复为 X, 这样就可以接着执行了嘛. 也就是所谓的恢复调用者(main)的栈帧.  
  也就是 main 函数调用 swap 函数(call 指令)保留 main 的状态(也就是 main 函数的 rbp 和 pc), swap 执行到最后(popq, ret)负责恢复现场(也就是恢复 main 函数的 rbp 和 pc). call 和 ret 指令的也分别有 push %rip 和 pop %rip 的作用. 很对称的操作!

# bombs

```gas
pushq   %rbp  ; 保留上一个函数(也就是调用者)的栈基址
movq    %rsp, %rbp ; 新函数的栈基址. 一个新的栈帧 sp 和 bp 指向的是同一个地址
```

> 一个所谓的栈帧(stack frame)就是由 sp(stack pointer) 和 bp(base pointer) 这两个寄存器来维护的。  
> 在编译器没有开启优化情况下，这两句会出现在每一个函数的开始, 那么问题来了 main 函数里面保留的是哪一个调用函数的栈基址呢?   
> 个人推测, 不一定正确, 我们知道创建进程(线程)是 OS 内核的功能, 当然进程销毁也是内核的功能.   
> 内核同样维护着属于内核空间的栈帧, 当进程创建完毕后, 我们写的 C 代码应该是被内核里的函数调用的, 这样的话 main 里面 pushq %rbp 应该是保留的内核函数的栈基址. 这样 main 的 ret 返回后就能接着执行内核函数里面的逻辑了. (估计也就是销毁进程一系列操作了, 这样才能把分配的资源收回来啊!)

# references

1. [program counter](https://en.wikipedia.org/wiki/Program_counter)
2. [A reader's guide to x86 assembly](http://cseweb.ucsd.edu/classes/sp10/cse141/pdf/02/S01_x86_64.key.pdf)
3. [x86 guide](https://www.cs.virginia.edu/~evans/cs216/guides/x86.html)
4. [instruction cycle](https://en.wikipedia.org/wiki/Instruction_cycle)
5. [how jump work](http://chortle.ccsu.edu/assemblytutorial/Chapter-17/ass17_2.html)
6. [stack pointer](https://en.wikipedia.org/wiki/Stack_register)
7. [call stack](https://en.wikipedia.org/wiki/Call_stack)
8. [stack-winding(push)-unwinding(pop)](https://www.bogotobogo.com/cplusplus/stackunwinding.php)
9. [understand-heap-assembly](https://www.bogotobogo.com/cplusplus/assembly.php)
10. [eax-x86-register-meaning-and-history](https://keleshev.com/eax-x86-register-meaning-and-history/)
11. [flint.cs.yale.edu/cs421/papers/x86-asm/asm.html](https://flint.cs.yale.edu/cs421/papers/x86-asm/asm.html)
12. [at&t 语法](https://csiflabs.cs.ucdavis.edu/~ssdavis/50/att-syntax.htm)
13. [x86-instruction-list](https://c9x.me/x86/)
14. [stack-frame-example](https://people.cs.rutgers.edu/~pxk/419/notes/frames.html)
