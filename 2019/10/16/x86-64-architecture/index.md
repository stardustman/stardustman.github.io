
> 操作系统其实很大一部分面向 `CPU` 来编程的。一些 `OS` 的概念直接来源于 `CPU` 的术语，或者和 `CPU` 关联性非常大。要想彻底理解 `OS` ，无法绕过 `CPU`。因为 Intel 的 `X86`(Intel 80386 之后的一系列 CPU 称之为 X86 架构) 是业界的标准。掌握这个架构对于理解 `OS` 是非常必要的。因为 `X86` 系列的 `CPU` 向后兼容，所以本文使用 Intel 8086(16 bit) 和 Intel 80386(32 bit) 来试图理解 `X86` 架构。

## Intel 8086

### 16 bit processor

> `16-bit` is a computer hardware device or software program capable of transferring 16 bits of data at a time. 一次传输 16 bits 的数据
> For example, early computer processors (e.g., 8088 and 80286) were 16-bit processors, meaning they were capable of working with 16-bit binary numbers (decimal number up to 65,535). Anything larger and the computer would need to break the number into smaller pieces.

### 1M 的寻址空间带来的问题与解决方法

#### Intel 8086 地址引脚

![intel8086_address](https://github.com/stardustman/pictures/raw/main/img/intel8086_address.png) #(Intel 8086 寻址空间)

> 上图的 AD<sub>0</sub> ~ AD<sub>15</sub> 是复用(Multiplex)引脚
> A<sub>16</sub> ~ A<sub>19</sub> 也是复用引脚

- ALE(Address Latch Enable) = 1 是作为地址线 A<sub>0</sub>~A<sub>15</sub>(寻址) 再加上A<sub>16</sub> ~ A<sub>19</sub>.
- ALE(Address Latch Enable) = 0 是作为数据线 D<sub>0</sub>~D<sub>15</sub>(传输 16 bit 数据)

#### 物理地址生成

> 20 bit 来表示一个内存地址. 但是 Intel 8086 是一个 `16 bit` 的处理器(其实就是一次只能传输 16 bit 的数据). 怎样来表示 `20 bit` 的内存地址呢? 比如一个内存地址是: `0x18AC9`, 该怎样表示呢? 其实道理很简单, 既然没法一次传输 20 bit, 那就分两次好了. 最后用两个 `16 bit` 的数据来合成 `0x18AC9` 这个地址. 很显然要有生成 `0x18AC9` 这个地址的策略. 在 Intel 8086 中, 将 `0x18AC9` 拆分为 `0x1234`(segment address) 和 `0x6789`(offset address) 这两个 `16 bit` 的地址, 这就是所谓的逻辑地址(logical address). 怎样生成 `0x18AC9` 这个真实的物理地址呢? 也很简单使用一个 `20 bit` 的 `adder`(加法器). 0x18AC9 = 0x1234 \* 16 + 0x6789

![intel8086_real_address_mode_translation](https://github.com/stardustman/pictures/raw/main/img/intel8086_real_address_mode_translation.png)

#### Real Address Mode

> 物理地址由两部分组成: segment address 和 offset address. 这两个地址不能直接使用, 是用来合成真正的物理地址的. 这两个地址称之为 `逻辑地址`. 16 \* segment address + offset address 合成的地址称之为 `物理地址`, 也叫 `线性地址`(也就是从 0 ~ 2^20 -1 地址空间). Intel 8086 这种得到物理地址的机制就叫做 `real address mode`. 也就是所谓的 `实模式`.
> PS: 多说一句, `实模式` 这个翻译有点坑, 翻译成 `实地址模式` 不就十分清楚了吗?

#### segmented memory

> physical address = 16 \* segment address + offset address
> offset address 是 16 bit, 决定了一个 segment 的范围是 64K. 将 1M 的寻址空间以 64K 为一个 segment 划分为多个 segment. 反过来说就是一个 segment 可以选择一个 64K 范围内的地址. 是的, `segment` 可以当作是一种 `selector`, 就像 `CSS` 里面的选择器.
> 那么这个 segment 存放在哪里呢? 放在专门的 `segment register` 里.

#### segment registers

> Intel 8086 提供了 `Extra Segment`(ES), `Data Segment`(DS), `Stack Segment`(SS), `Code Segment`(CS) 这 4 个 段寄存器.

##### code segment(CS) + instruction pointer(IP)

> code segment 存放 segment address, 这是个专门的代码段寄存器, 用来存放代码的地址.
> instruction pointer 存放 offset address, 这个是专门的指令寄存器, 用来存放下一条指令的地址.
> 所以访问代码的 `物理地址 = 16 * CS + IP`

##### stack segment(SS) + stack pointer(SP)

> stack segment 存放 segment address, 这个是专门的栈帧段寄存器.
> stack pointer 存放 offset address, 这个专门的记录栈顶的寄存器.
> 所以访问函数栈帧的 `物理地址 = 16 * SS + SP`

> 注意: 如果令 SS = 0, 那么访问函数栈帧的 `物理地址 = SP`, 也就是只能访问 64K 的空间.
> 注意: real address mode 能够不受限制地访问 1M 的地址空间. 程序可以自由的访问 1M 的地址空间, 无法进行内存的保护, 这也是 `Intel 80386` 要解决的问题之一.

####

### general purpose register

### memory mapping

> 已安装内存 8G(7.88G) 可用？？？原因在哪里呢？

## Intel 80386

## Reference

1. [bus interface unit](http://8086microprocessor4u.blogspot.com/2012/12/bus-interface-unit-biu.html)
2. [execution unit](http://8086microprocessor4u.blogspot.com/2015/04/the-execution-unit-eu.html)
3. [8086 Functional Units](https://www.tutorialspoint.com/microprocessor/microprocessor_8086_functional_units.htm)
4. [components of cpu](https://www.csetutor.com/components-of-cpu-and-their-functions/)
5. [8086-microprocessor-architecture](https://vdocuments.mx/8086-microprocessor-architecture-5584489b9c738.html)
6. [8086-microprocessor](https://www.academia.edu/40314276/8086_microprocessor20190911_87442_nxpkcz)
7. [memory-segmentation-8086-microprocessor](https://www.geeksforgeeks.org/memory-segmentation-8086-microprocessor/)
8. [Advanced Microcomputer Programming](https://www.cs.usfca.edu/~cruse/cs630f06/)
9. [16 bit processor mean?](https://www.computerhope.com/jargon/num/16bit.htm)
10. [memory layout](https://cgnail.github.io/academic/mem-layout/)
11. [x86-overview](https://cs.lmu.edu/~ray/notes/x86overview/)
12. [计算机位数发展](https://www.zhihu.com/question/552957041/answer/2738625402)
