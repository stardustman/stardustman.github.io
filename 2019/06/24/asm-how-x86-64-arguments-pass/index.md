### x86-64 下函数参数传递, 汇编层面分析

> To pass parameters to the subroutine, we put up to six of them into registers (in order: rdi, rsi, rdx, rcx, r8, r9).  
> If there are more than six parameters to the subroutine, then push the rest onto the stack in reverse order (i.e. last parameter first)  
> – since the stack grows down, the first of the extra parameters (really the seventh parameter) parameter will be stored at the lowest address (this inversion of parameters was historically used to allow functions to be passed a variable number of parameters).

# 代码分析

## C main

```c
int main(){
    int result = 0;
    result = testArgs(1,2,3,4,5,6,7,8);
    return 0;
}

```

### Asm main 

gcc -o main main.c

```gas
main:
        pushq   %rbp
        movq    %rsp, %rbp
        subq    $16, %rsp
        movl    $0, -4(%rbp) ;result
        pushq   $8 ; 最后一个参数
        pushq   $7 ; 倒数第二个参数
        movl    $6, %r9d ; 第六个参数
        movl    $5, %r8d ; 第五个参数
        movl    $4, %ecx ; 第四个参数
        movl    $3, %edx ; 第三个参数
        movl    $2, %esi ; 第二个参数
        movl    $1, %edi ; 第一个参数
        call    testArgs(long, long, long, long, long, long, long, long)
        addq    $16, %rsp ; 回收分配的栈空间
        movl    %eax, -4(%rbp)
        movl    $0, %eax
        leave
        ret
```

### C testArgs

```c
int testArgs(long a1,long a2,long a3,long a4,long a5,long a6,long a7,long a8){
    long sum = 0;
    sum = a1 + a2 + a3 + a4 +a5 + a6 + a7 + a8;
    return sum;
}
```

### Asm testArgs

```x86asm
testArgs(long, long, long, long, long, long, long, long):
        pushq   %rbp
        movq    %rsp, %rbp
        movq    %rdi, -24(%rbp) ; a1  栈底
        movq    %rsi, -32(%rbp) ; a2
        movq    %rdx, -40(%rbp) ; a3
        movq    %rcx, -48(%rbp) ; a4
        movq    %r8, -56(%rbp)  ; a5
        movq    %r9, -64(%rbp)  ; a6
        movq    $0, -8(%rbp)    ; sum
        movq    -24(%rbp), %rdx ; a1
        movq    -32(%rbp), %rax ; a2
        addq    %rax, %rdx      ; rdx = a1 + a2
        movq    -40(%rbp), %rax ; a3
        addq    %rax, %rdx
        movq    -48(%rbp), %rax ; a4
        addq    %rax, %rdx
        movq    -56(%rbp), %rax ; a5
        addq    %rax, %rdx
        movq    -64(%rbp), %rax ; a6
        addq    %rax, %rdx
        movq    16(%rbp), %rax  ; a7，其实是到 main 的栈帧里取的,main 准备的参数,因为 rbp 就是 stack frame 的栈底
        addq    %rax, %rdx
        movq    24(%rbp), %rax  ; a8，其实是到 main 的栈帧里取的,main 准备的参数
        addq    %rdx, %rax
        movq    %rax, -8(%rbp)  ; sum = rax
        movq    -8(%rbp), %rax  ; rax = sum 也就是返回值
        popq    %rbp
        ret
```

> 综上可知，编译器默认会将所有的参数复制到栈上。

## 栈帧分析

![x86-64-arguments-pass](https://github.com/stardustman/pictures/raw/main/img/x86-64-arguments-pass.png)  
#(x86-64-arguments-pass) 栈顶在下方
> 上图是执行完 testArgs 前两句汇编的栈帧状态图.  
> 分析:
> testArgs 中 `movq    16(%rbp), %rax`  ; 将 rbp + 16 地址处的数值(参数 a7)复制到 rax  
rbp + 16 的原因是: 参数 a7 是 main 准备的, 属于 main 的栈帧.  
16 Bytes 包括 8 Bytes 的 Return address(main 函数中的 call 指令压入的 call 指令下一条指令的地址)  
和 8 Bytes 的 main 的 rbp(testArgs 函数中第一条指令: pushq %rbp).

## Reference

1. [callee-caller](http://web.stanford.edu/class/cs107/guide/x86-64.html)