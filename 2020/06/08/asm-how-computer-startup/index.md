
`x86` 架构计算机是如何启动的？

## 16-bit Processors and Segmentation (1978)

> The IA-32 architecture family was preceded by `16-bit processors`, the 8086 and 8088. The 8086 has `16-bit registers` and a `16-bit external data bus`, with `20-bit addressing giving a 1-MByte address space`. The 8088 is similar to the 8086 except it has an 8-bit external data bus.
> The 8086/8088 introduced `segmentation` to the IA-32 architecture. With segmentation, a `16-bit segment register` contains a pointer to `a memory segment of up to 64 KBytes`. Using `four` segment registers at a time, 8086/8088 processors are able to address up to 256 KBytes `without` switching between segments. The 20-bit addresses that can be formed using a segment register and an additional 16-bit pointer provide a total address range of 1 MByte.

8086 是整个 intel 系列处理器的老祖宗。所有后续的一系列处理器都兼容 8086。唉，这就是路径依赖啊。这是一个 16-bit 的处理器。寄存器也是 16-bit 的。but 却能寻址 20-bit 的地址空间，也就是 1M 的内存。怎样在 16-bit 的处理器上实现呢？很简单，将 1M 的空间分成多个 64K 的 segment。那怎样用 16-bit 的寄存器表示 20-bit 的地址呢？也很简单，用 2 个 16-bit 的寄存器组合得来的(register1,register2)。那怎样组合的呢？`address = register1 << 4 + register2`。将 register1 << 4 和 register2 用一个 20-bit 的加法器就能得到内存地址。这种方式就是所谓的 `real address mode`。也即是拿到的是真正的内存地址。x86 架构的计算机启动时会先进入 real address mode。

## 计算机启动前置知识

1. CPU 工作方式是: 从`内存`中取指、执行。
2. 内存是存储数据的地方，给出一个内存地址，可以得到该处的数据。
3. CPU 从内存哪里取指令，由 `instruction pointer` 这个寄存器的值决定。这个值不断 +1，或者跳转(jump) 到某处。real address mode 下由 CS:IP 组合决定。

## BIOS

### BIOS 是啥

`BIOS` 是啥呢？一个软件而已。不像我们平时在 PC 上使用如 `QQ` 等软件是安装在硬盘上的。BIOS 安装在 ROM 中。这个软件干啥的呢？帮助计算机启动，开机时检测整个机器的硬件。计算机启动需要运行软件，软件运行需要计算机先启动。咳咳，成了先有鸡还是先有蛋了，这怎么行？所以需要 BIOS 这个在硬件上的软件来帮助，破除这种依赖。

### BIOS 大小

通过 `dmidecode` 这个命令来查看。也就是读取 BIOS 的信息。

```sh
root@aliyun:~# dmidecode -t bios -q  # 方法一
BIOS Information
        Vendor: SeaBIOS
        Version: 8c24b4c
        Release Date: 04/01/2014
        Address: 0xE8000
        Runtime Size: 96 kB
        ROM Size: 64 kB --- ROM Size，在此可以看到，BIOS 大小为 64K。
        Characteristics:
                BIOS characteristics not supported
                Targeted content distribution is supported
        BIOS Revision: 0.0

root@aliyun:~# cd /sys/class/dmi/id/  # 方法二
root@aliyun:id# ls
bios_date          chassis_serial   modalias        product_serial   sys_vendor
bios_vendor        chassis_type     power           product_uuid     uevent
bios_version       chassis_vendor   product_family  product_version
chassis_asset_tag  chassis_version  product_name    subsystem

```

## x86 架构计算机启动流程

![real address mode 1M 内存布局](https://github.com/stardustman/pictures/raw/main/img/real-address-mode-how-computer-startup.svg) #(real address mode 1M 内存布局)

1. 如上图所示，当按下 `power on` 之后，此时的 CPU 处于 real address mode。由硬件负责将 `BIOS 64K` 的内容加载到 `0xF0000 到 0xFFFFF`(64K 内存)。并将 `CS(code segment) 置为 0xF000`，`IP(instruction pointer) 置为 0xFFF0`。组合起来的内存地址 `PC` 就是 0xFFFF0。

2. 此时 CPU 从 `PC = 0xFFFF0` 处开始取指、执行。那么从 `0xFFFF0 到 0xFFFFF` 只有 16Bytes。空间太小了，能放啥呢？但是可以跳到其他地方去执行啊。不错，这里存放的有一条指令是 `jmp far f000:e05b`, 组合起来就是 `0xfe05b`。注意这个地址可是在 `0xF0000 到 0xFFFFF` 范围之内，也就是这条指令跳转到 `BIOS 内部的代码`去执行。从 0xFE05B 到 0xFFFF0 将近 8K 内存，代码不少。

3. 此时 CPU 从 `PC = 0xfe05b` 开始取值、执行。具体干啥呢？就是执行所谓的开机自检。检查计算机硬件。同时去找启动扇区，那什么才是启动扇区呢？若 0 盘 0 道 1 扇区最后两个字节分别是 0x55,0xaa，那就是启动区。很像 java class 文件中的魔数，就是标记一下。同时将找到的启动扇区(512Bytes) 复制到内存 `0x7C00 到 0x7DFF`(512Bytes) 处。最后会将 `CS 置为 0x0000， IP 置为 0x7C00`，组合起来就是 `0x7C00`。

4. 此时 CPU 从 `PC = 0x7C00` 开始取值、执行。具体干啥呢？这就是开发人员可以决定的了。在启动扇区上可以是加载 OS Kernel 的代码，硬盘分区，boot loader 等。

## References

1.[x86-ORG](http://dewkumar.blogspot.com/2012/01/what-is-org-origin-directive-in.html)
2. [how-to-see-rom-size](https://superuser.com/questions/806226/how-to-see-rom-size)
3. [bios-info-dmidecode](http://smilejay.com/2013/02/bios-info-dmidecode/)
4. [system-management-bios](https://en.wikipedia.org/wiki/System_Management_BIOS)
5. [计算机是如何启动的？](http://www.ruanyifeng.com/blog/2013/02/booting.html)
6. [多种 MBR 组织方式](https://en.wikipedia.org/wiki/Master_boot_record)
7. [计算机是怎样启动的？](https://mp.weixin.qq.com/s/g6m_r28bvyyWIu1CAzxLdg)