
# 斐波那契数列

## 问题描述

> 第一个月初有一对刚诞生的兔子
第二个月之后(第三个月)它们可以生育
每月每对可生育的兔子会诞生下一对新兔子
兔子永不死
> 问第 n 月有多少对兔子?

## 图解

> 假设在n月有兔子总共 a 对, n+1 月总共有 b 对. 在 n+2 月必定总共有 a+b 对: 因为在 n+2 月的时候, 前一月(n+1月) 的 b 对兔子可以存留至第 n+2 月(在当月属于新诞生的兔子尚不能生育). 而新生育出的兔子对数等于所有在 n 月就已存在的 a 对.

![fibonacci-sequence](https://github.com/stardustman/pictures/raw/main/img/fibonacci-sequence.png) 

# 代码分析

## C 代码

```cpp
#include <stdio.h>
long fib(long n){
    if(n <= 2){
        return 1;
    }
   return fib(n-1) + fib(n-2);
}

int main(){
    long result = 0;
    result = fib(4);
    //printf("%d\n",result);
    return 0;
}
```

## Asm 代码

### 中间代码

```gas
fib(long):
        pushq   %rbp
        movq    %rsp, %rbp
        pushq   %rbx
        subq    $24, %rsp
        movq    %rdi, -24(%rbp) // n
        cmpq    $2, -24(%rbp)   // n - 2
        jg      .L2 
        movl    $1, %eax        // n <= 2
        jmp     .L3
.L2:
        movq    -24(%rbp), %rax // n
        subq    $1, %rax // n - 1
        movq    %rax, %rdi
        call    fib(long)

        movq    %rax, %rbx // 
        movq    -24(%rbp), %rax // n
        subq    $2, %rax // n - 2
        movq    %rax, %rdi
        call    fib(long)
        addq    %rbx, %rax
.L3:
        addq    $24, %rsp // 释放分配的栈空间
        popq    %rbx
        popq    %rbp
        ret
main:
        pushq   %rbp
        movq    %rsp, %rbp
        subq    $16, %rsp
        movl    $3, %edi  // n = 3
        call    fib(long)
        movq    %rax, -8(%rbp) // result = fib(3)
        movl    $0, %eax
        leave
        ret

```

![fib-label](https://github.com/stardustman/pictures/raw/main/img/fib-label.png)

### 反编译可执行代码

```gas
fib(long):
        push   %rbp
        mov    %rsp,%rbp
        push   %rbx
        sub    $0x18,%rsp
        mov    %rdi,-0x18(%rbp)
        cmpq   $0x2,-0x18(%rbp)
        jg     4004cd <fib(long)+0x1b> // 0x1b 是地址为 4004cd 这条指令之前的所有指令的字节数
        mov    $0x1,%eax
        jmp    4004f3 <fib(long)+0x41> // 0x41 是地址为 4004f3 这条指令之前的所有指令的字节数
        mov    -0x18(%rbp),%rax
        sub    $0x1,%rax
        mov    %rax,%rdi
        callq  4004b2 <fib(long)>
        mov    %rax,%rbx
        mov    -0x18(%rbp),%rax
        sub    $0x2,%rax
        mov    %rax,%rdi
        callq  4004b2 <fib(long)>
        add    %rbx,%rax
        add    $0x18,%rsp
        pop    %rbx
        pop    %rbp
        retq   
main:
        push   %rbp
        mov    %rsp,%rbp
        sub    $0x10,%rsp
        movq   $0x0,-0x8(%rbp)
        mov    $0x4,%edi
        callq  4004b2 <fib(long)>
        mov    %rax,-0x8(%rbp)
        mov    $0x0,%eax
        leaveq 
        retq   
        nop
```

![fib](https://github.com/stardustman/pictures/raw/main/img/fib-label.png)

> fib(n)函数里调用 fib(n - 1) 和 fib(n - 2)视为两个和 fib(n) 完全不同的函数, 因为函数返回地址在汇编层面根本不一样. <br>
> fib(n), fib(n - 1) 和 fib(n - 2) 的处理逻辑不一样, 只是之间有依赖而已. <br>
> 也可以视为编译器级别的函数重载. 理解成三个不同的函数, 这个递归就很好理解了. <br>

## 调用栈调用顺序分析

### 调用栈图解分析

![fib(4)调用栈分析](https://github.com/stardustman/pictures/raw/main/img/fib(4)-call-stack.png) #(fib(4)调用栈分析)

### 符合递归终止时调用栈执行顺序

> fib(3) = fib(2) + fib(1), 就符合递归退出的条件.

#### 代码执行顺序

![fib(3)-execute-sequence](https://github.com/stardustman/pictures/raw/main/img/fib(3)-execute-sequence.png) #(fib(3)-execute-sequence)

#### 二叉树后序遍历的视角来分析

> 递归终止条件: <br>
> fib(2) = 1 视为左叶子节点 <br>
> fib(1) = 1 视为右叶子节点 <br>
> fib(3) = fib(2) + fib(1) 视为父节点 <br>

![fib(3)-execute-squence-treefy](https://github.com/stardustman/pictures/raw/main/img/fib(3)-execute-squence-treefy.png) #(符合递归退出代码执行流程)
> fib(3) 调用 fib(2), 计算出参数 n = 2. 进入左叶子节点. <br>
> fib(2) 返回 fib(3), fib(2) 的返回值 rax = 1, 复制给 rbx. <br>
> fib(3) 调用 fib(1), 计算出参数 n = 1. 进入右叶子节点. <br>
> fib(1) 返回 fib(3), fib(1) 的返回值 rax = 1 <br>
> fib(3) 计算 fib(3) = fib(2) + fib(1) = rbx + rax = 1 + 1 = 2 <br>

#### 栈帧的创建和销毁-二叉树后序遍历

> fib(n) = fib(n-1) + fib(n-2)(n > 2) 递归调用可以看作是栈帧按照二叉树后续遍历(左子树-右子树-根)的顺序动态的创建和销毁. <br>
> 着色方框为创建的栈帧 <br>
> 白色方框为销毁的栈帧或者还未创建的栈帧 <br>
> 方框由白色变为着色: 栈帧创建 <br>
> 方框由着色变为白色: 栈帧销毁 <br>

![fib-dynamic-stack-frame](https://github.com/stardustman/pictures/raw/main/img/fib-dynamic-stack-frame.png)

> 从图中可以看出假如 main 调用 fib(5), 会一直调用到 f(2) 才会终止. 则调用栈状态如图 0. 此时递归最大的栈深度是 4, 如果 n 值过大, 会很容易发生 Stack Overflow 这种错误. 比方说 fib(1000) 会一直创建到 fib(2) 这个栈帧, 递归才会开始返回. <br>
> fib(2) 返回 1 给 fib(3), fib(3) 保存这个返回值. f(2) 栈帧销毁. 如图 1. <br>
> fib(3) 调用 fib(2) 如图 2.
> fib(2) 返回 1 给 fib(3), fib(3) 将 fib(2)返回值和 fib(1) 的返回值相加. 如图 3. <br>
> fib(3) 返回 fib(2)返回值和 fib(1) 的返回值相加的结果. 如图 4. <br>

##### 效率低原因分析

> 计算的结果并没有保存. <br>
> 每一次进入递归之后都是从基本的 fib(2) 和 fib(1) 向上返回. <br>
> 中间伴随着大量的栈帧创建和销毁, 以及重复的函数计算.函数栈帧的创建和销毁是耗时的操作, 这可就很慢了. <br>
> fib(n) 开辟栈帧总数量可以表示为: sum(n) = 2^(n - 2) + 1 (n >= 3). <br>
> 时间复杂度是: O(2^n) <br>

##### 运行时间测试

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LENGTH 51
unsigned long fib(unsigned long n){
    if(n <= 2){
        return 1;
    }
   return fib(n-1) + fib(n-2);
}

int main(){
    unsigned long long result = 0;
    clock_t beginTime,endTime;
    beginTime = clock();
    result = fib(LENGTH - 1);
    endTime = clock();
    printf("Running Time %f Seconds\n",(double)(endTime - beginTime)/CLOCKS_PER_SEC);
    printf(" %d :: %lu \n", LENGTH - 1, result);
    return 0;
}
```

> fib(n) 开辟栈帧总数量可以表示为: sum(n) = 2^(n - 2) + 1 (n >= 3). <br>
> 时间复杂度是: O(2^n) <br>
> fib(50) 开辟栈帧总量是: sum(50) = 2^(48) + 1 <br>
> 时间复杂度是: O(2 ^ 50) <br>
> 计算 fib(50) 费了 65 秒左右 <br>

##### 解决方法

> 用数组保存已经计算出来的 fib(n) 的结果. 修改计算逻辑, 如果 fib(n) 已经被计算, 直接使用, 不再进入递归计算. <br>
> 这可不就是有拿空间换时间的感觉. <br>

## 优化递归

### 空间换时间优化代码

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
# define LENGTH 51
// 计算 fib(50)
// 全局的用来保存 fib(n)(n > 2) 计算结果的数组.
unsigned long result[LENGTH];
unsigned long fib(long n){
    // fib(n) 已经被计算, 那就直接返回.
    if(result[n] != 0){
        return result[n];
    } else{
    // 保存 fib(n) 的计算结果
       result[n] =  fib(n-1) + fib(n-2);
    } 
   return result[n];
}

int main(){
    clock_t beginTime,endTime;
    // 由于 1, 1, 2, 3, 5 ..... 都是大于 0 的结果, 全部初始化为 0, 相当于都没有计算结果.
    for(int i = 0; i < LENGTH ; i++){
        result[i] = 0;
    }
    // fib(1) 的计算结果为 1
    result[1] = 1;
    // fib(2) 的计算结果为 1
    result[2] = 1;
    beginTime = clock();
    fib(LENGTH - 1);
    endTime = clock();
    printf("Running Time %f Seconds\n",(double)(endTime - beginTime)/CLOCKS_PER_SEC);
    for(int i = 1; i < LENGTH; i++){
        printf(" %i :: %lu \n", i, result[i]);
    }
}
```

### 优化递归函数栈帧

![fib-optimize-stack-frame](https://github.com/stardustman/pictures/raw/main/img/fib-optimize-stack-frame.png)
*fib(5)递归栈帧)*

> 着色方框-开辟销毁的栈帧 <br>
> 白色方框-不用开辟的栈帧 <br>
> 优化后的 fib(n) 开辟栈帧总数量可以表示为: sum(n) = (n - 2)(n >= 3). <br>
> 时间复杂度是: O(n) <br>
> 计算 fib(50) 开辟栈帧总数量是: sum(50) = 48 <br>
> fib(50) = 3996334433 <br>
> 时间复杂度是 O(50), 结果几乎秒算. <br>
> unsigned long result[LENGTH]; <br>
> 拿空间换时间, 其实这句话有问题的, 拿出来空间, 计算逻辑也是要优化的. <br>

### 尾递归优化

#### 非优化递归

```cpp
unsigned long fib(unsigned long n){
    if(n <= 2){
        return 1;
    }
   return fib(n-1) + fib(n-2);
}
```

> 汇编视角下这里 fib(n-1) 和 fib(n-2) 与 fib(n) 其实根本不是同一个函数. <br>
> 三者的逻辑其实不同, 但是函数之间有依赖. <br>

#### 优化递归

> 尾递归的实现, 往往需要改写递归函数, 确保最后一步只调用自身. 做到这一点的方法, 就是把所有用到的内部变量改写成函数的参数. <br>
> 尾调用的概念非常简单, 一句话就能说清楚, 就是指某个函数的最后一步是调用另一个函数. <br>

##### 中间变量改成函数的参数

```cpp
unsigned long fib(unsigned long n, unsigned long prev, unsigned long sum ){
    if(n <= 3){
        return sum;
    }
    return fib(n-1, sum , prev + sum); // 尾调用
}
```
> 这里 fib(n-1, sum , prev + sum) 的才是和 fib(unsigned long n, unsigned long prev, unsigned long sum ) 完全一样的函数.

##### 包装一层

```cpp
unsigned long   fibonacc(unsigned long n){
    if( n <= 2){
        return 1;
    } 
    // 1 : fib(2)
    // 2 : fib(2) + fib(1) = fib(3)
    return fib(n, 1, 2);
}
```

> 计算结果保存在参数里.

##### fib(n) 汇编代码分析

![fib-tail-recursion](https://github.com/stardustman/pictures/raw/main/img/fib-tail-recursion.png)
#(尾递归汇编代码)

> 尾递归的 fib(n-1, sum , prev + sum) 是尾调用, 也就是函数执行完没有其他的操作了, 就直接返回了. <br>
> 符合 n <= 3 的条件, 汇编直接 jmp 到销毁栈帧的代码. 因为返回值在符合递归退出条件时, 已经被设置到 rax 里了. <br>

## 结论

> 用简单的一句话,递归就是调用函数本身. 这句话是相当不负责任的.  <br>
> 递归函数在汇编级别的调用自己, 尽管调用的是自己, 递归的函数名尽管相同, 但是函数返回地址是不相同的. 这也意味着其实逻辑完全可能不一样. <br>
> 结合栈帧的创建和销毁, 就可以理解 `Stack Overflow` 这种错误. <br>

# References

1. [asm-cmpq](https://stackoverflow.com/questions/21440403/what-does-the-cmpq-instruction-do)
2. [asm-jg](https://www.aldeid.com/wiki/X86-assembly/Instructions/jg)
3. [asm-jp-table](http://faydoc.tripod.com/cpu/jg.htm)
4. [斐波那契数列](https://zh.wikipedia.org/wiki/%E6%96%90%E6%B3%A2%E9%82%A3%E5%A5%91%E6%95%B0%E5%88%97)
5. [asm-tool](https://godbolt.org/)
6. [尾调用(tail-call)之尾递归](http://www.ruanyifeng.com/blog/2015/04/tail-call.html)
7. [动态规划](https://mp.weixin.qq.com/s/GncAsmZ1dZKyNMOwN5-sjw)
