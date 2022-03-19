
## 前言

无论多么花里胡哨的功能，最终落地到一台计算机上，都是二进制代码。虽然 java 代码跑在 jvm 平台之上，但是 jvm 只是负责执行 java 自定义的一套 bytecode 的工具，只要能解释字节码，这个工具用什么语言写都是可以的。主流的 HotSpot 虚拟机选择的 C++。

## 二进制代码探析

### C 语言举例

#### gcc 不开启优化

```c
#include<stdio.h>

long  desc(long a) {
    return a - 1;
}

int main() {
    printf("%ld\n", desc(3));
    return 0;
}
```

具体[汇编级别的参数传递](https://stardustman.github.io/2019/06/24/asm-how-x86-64-arguments-pass/) 可以参考这篇。x86 架构下[函数如何之行](https://stardustman.github.io/2019/05/28/asm-how-x86-function-execute/) 可以参考这篇。

gcc -o descv1 desc.c 之后通过 objdump -d descv11 可以找到 `long desc(long a)` 的汇编代码如下：

```asm
000000000000064a <desc>:
 64a:   55                      push   %rbp              ;❶ 保存上一个栈帧的 base，进入一个函数的常规操作。
 64b:   48 89 e5                mov    %rsp,%rbp         ;❷ 开辟新的栈帧
 64e:   48 89 7d f8             mov    %rdi,-0x8(%rbp)   ;❸ 参数 a（rdi）入栈
 652:   48 8b 45 f8             mov    -0x8(%rbp),%rax   ;❹ 参数 a 从栈中复制到 rax
 656:   48 83 e8 01             sub    $0x1,%rax         ;❺ a = a - 1
 65a:   5d                      pop    %rbp              ;❻ 弹出 rbp，也即是恢复上一个栈帧的 base
 65b:   c3                      retq                     ;❼ 函数返回
```

可以看到 ❸ 和 ❹ 这两步有些多余，这是没有开启优化，gcc 默认采用的方式，把所有的参数先放到栈上。我们可以进行优化。

#### 对 gcc 不开启优化选项的汇编代码优化

```c
#include <stdio.h>
#include <memory.h>
#include <sys/mman.h>

typedef int (*desc_func)(int a);
int main()
{
    char desc_code[] = {
        0x55,             // ❶ push rbp
        0x48, 0x89, 0xe5, // ❷ mov rsp, rbp

        0x89, 0xf8,             // mov edi, eax ??
        0x48, 0x83, 0xe8, 0x01, //  sub  $0x1,%rax

        0x5d, // ❻ pop rbp
        0xc3  // ❼ ret
    };

    void *temp = mmap(NULL, sizeof(desc_code),
                      PROT_WRITE | PROT_EXEC,
                      MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    memcpy(temp, desc_code, sizeof(desc_code));
    desc_func p_desc = (desc_func)temp;
    printf("%d\n", p_desc(3));

    return 0;
}
```

> 可以看到在运行中创建 desc 函数，`运行时生成可执行的机器码`这种方式其实就是 JIT 实现的核心操作。
> 同时可以看到，根本没有必要开辟栈帧来执行这个 desc 函数。也就是可以把 ➊ ➋ ➏ 栈帧相关操作删除。

#### 进一步优化，消除栈帧

```c
#include <stdio.h>
#include <memory.h>
#include <sys/mman.h>

typedef int (*desc_func)(int a);
int main()
{
    char desc_code[] = {

        0x89, 0xf8,             // mov edi, eax ??
        0x48, 0x83, 0xe8, 0x01, //  sub  $0x1,%rax
        0xc3  // ❼ ret
    };

    void *temp = mmap(NULL, sizeof(desc_code),
                      PROT_WRITE | PROT_EXEC,
                      MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    memcpy(temp, desc_code, sizeof(desc_code));
    desc_func p_desc = (desc_func)temp;
    printf("%d\n", p_desc(3));

    return 0;
}
```

> 可以看到不开辟栈帧，一样可以执行代码。

#### lea addr dst 优化

```c
 char code[] = { 
        0x48, 0x8d, 0x47 0xff,    // lea -0x1(rdi), rax lea 直接将 rdi 寄存器里的值减去 1 复制给 rax
        0xc3                 // ret
    };
```

> 这个和 gcc -o desc desc.c -O2 结果是一样的。

## References

1. [java 杂谈之 JIT](https://mp.weixin.qq.com/s/TokGNbdnf2XkhHSbneppgg)
2. [x64-instruction-table](http://ref.x86asm.net/coder64.html)
3. [https://web.stanford.edu/class/archive/cs/cs107/cs107.1166/guide_x86-64.html](https://web.stanford.edu/class/archive/cs/cs107/cs107.1166/guide_x86-64.html)
4. [http://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf](http://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf)
5. [load-effective-address(lea)](https://www.felixcloutier.com/x86/lea)
6. [java-bytecode](https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html)
7. [圆圈数字等](https://blog.csdn.net/appleyuchi/article/details/84036985)
