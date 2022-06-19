
# virtual memory address space layout

![virtual-memory-address-space](https://raw.githubusercontent.com/stardustman/pictures/main/img/virtual-memory-address-space.jpg)

## 代码测试

1. `cargo new vas-explore`

main.rs

```rust
use std::{thread,time};
fn main() {
    println!("Hello, world!");
    let sl = time::Duration::from_millis(10000000);
    thread::sleep(sl);
    println!("Goodbye, world!");
}
```

2. cargo build
3. ./target/debug/vas-explore
4. pidof vas-explore

## /proc/pid/maps

```bash
stardust@os:x86_64-linux-gnu$ pidof vas-explore
75599      
stardust@os:x86_64-linux-gnu$ cat /proc/75599/maps
556a8a831000-556a8a837000 r--p 00000000 08:12 815699                     /home/stardust/Desktop/rust/vas-explore/target/debug/vas-explore
556a8a837000-556a8a86e000 r-xp 00006000 08:12 815699                     /home/stardust/Desktop/rust/ vas-explore/target/debug/vas-explore # text(code segment) segment
556a8a86e000-556a8a87c000 r--p 0003d000 08:12 815699                     /home/stardust/Desktop/rust/vas-explore/target/debug/vas-explore
556a8a87d000-556a8a880000 r--p 0004b000 08:12 815699                     /home/stardust/Desktop/rust/vas-explore/target/debug/vas-explore
556a8a880000-556a8a881000 rw-p 0004e000 08:12 815699                     /home/stardust/Desktop/rust/vas-explore/target/debug/vas-explore
556a8c01b000-556a8c03c000 rw-p 00000000 00:00 0                          [heap] # heap segment
7f5e59636000-7f5e59638000 rw-p 00000000 00:00 0 
7f5e59638000-7f5e5965a000 r--p 00000000 08:12 533486                     /lib/x86_64-linux-gnu/libc-2.31.so
7f5e5965a000-7f5e597d2000 r-xp 00022000 08:12 533486                     /lib/x86_64-linux-gnu/libc-2.31.so
7f5e597d2000-7f5e59820000 r--p 0019a000 08:12 533486                     /lib/x86_64-linux-gnu/libc-2.31.so
7f5e59820000-7f5e59824000 r--p 001e7000 08:12 533486                     /lib/x86_64-linux-gnu/libc-2.31.so
7f5e59824000-7f5e59826000 rw-p 001eb000 08:12 533486                     /lib/x86_64-linux-gnu/libc-2.31.so
7f5e59826000-7f5e5982a000 rw-p 00000000 00:00 0 
7f5e5982a000-7f5e5982b000 r--p 00000000 08:12 533487                     /lib/x86_64-linux-gnu/libdl-2.31.so
7f5e5982b000-7f5e5982d000 r-xp 00001000 08:12 533487                     /lib/x86_64-linux-gnu/libdl-2.31.so
7f5e5982d000-7f5e5982e000 r--p 00003000 08:12 533487                     /lib/x86_64-linux-gnu/libdl-2.31.so
7f5e5982e000-7f5e5982f000 r--p 00003000 08:12 533487                     /lib/x86_64-linux-gnu/libdl-2.31.so
7f5e5982f000-7f5e59830000 rw-p 00004000 08:12 533487                     /lib/x86_64-linux-gnu/libdl-2.31.so
7f5e59830000-7f5e59836000 r--p 00000000 08:12 533569                     /lib/x86_64-linux-gnu/libpthread-2.31.so
7f5e59836000-7f5e59847000 r-xp 00006000 08:12 533569                     /lib/x86_64-linux-gnu/libpthread-2.31.so
7f5e59847000-7f5e5984d000 r--p 00017000 08:12 533569                     /lib/x86_64-linux-gnu/libpthread-2.31.so
7f5e5984d000-7f5e5984e000 r--p 0001c000 08:12 533569                     /lib/x86_64-linux-gnu/libpthread-2.31.so
7f5e5984e000-7f5e5984f000 rw-p 0001d000 08:12 533569                     /lib/x86_64-linux-gnu/libpthread-2.31.so
7f5e5984f000-7f5e59853000 rw-p 00000000 00:00 0 
7f5e59853000-7f5e59856000 r--p 00000000 08:12 531127                     /lib/x86_64-linux-gnu/libgcc_s.so.1
7f5e59856000-7f5e59868000 r-xp 00003000 08:12 531127                     /lib/x86_64-linux-gnu/libgcc_s.so.1
7f5e59868000-7f5e5986c000 r--p 00015000 08:12 531127                     /lib/x86_64-linux-gnu/libgcc_s.so.1
7f5e5986c000-7f5e5986d000 r--p 00018000 08:12 531127                     /lib/x86_64-linux-gnu/libgcc_s.so.1
7f5e5986d000-7f5e5986e000 rw-p 00019000 08:12 531127                     /lib/x86_64-linux-gnu/libgcc_s.so.1
7f5e5986e000-7f5e59870000 rw-p 00000000 00:00 0 
7f5e59885000-7f5e59886000 ---p 00000000 00:00 0 
7f5e59886000-7f5e59888000 rw-p 00000000 00:00 0 
7f5e59888000-7f5e59889000 r--p 00000000 08:12 533138                     /lib/x86_64-linux-gnu/ld-2.31.so
7f5e59889000-7f5e598ac000 r-xp 00001000 08:12 533138                     /lib/x86_64-linux-gnu/ld-2.31.so
7f5e598ac000-7f5e598b4000 r--p 00024000 08:12 533138                     /lib/x86_64-linux-gnu/ld-2.31.so
7f5e598b5000-7f5e598b6000 r--p 0002c000 08:12 533138                     /lib/x86_64-linux-gnu/ld-2.31.so
7f5e598b6000-7f5e598b7000 rw-p 0002d000 08:12 533138                     /lib/x86_64-linux-gnu/ld-2.31.so
7f5e598b7000-7f5e598b8000 rw-p 00000000 00:00 0 
7ffe91c09000-7ffe91c2a000 rw-p 00000000 00:00 0                          [stack] # stack segment
7ffe91c38000-7ffe91c3b000 r--p 00000000 00:00 0                          [vvar]
7ffe91c3b000-7ffe91c3c000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 --xp 00000000 00:00 0                  [vsyscall]
```

## row format

![row format](https://raw.githubusercontent.com/stardustman/pictures/main/img/virtual-memory-address-space-format.jpg)

## text

代码段，程序的可执行代码在这里。

## stack

栈区，从 maps 可以看到 `7ffe91c09000-7ffe91c2a000`，这个区域是这个进程的栈区，可读、可写。`7ffe91c09000` 这个 48bits 的地址就是虚拟内存地址。为什么是 48bits，因为 amd64 的虚拟地址用 48bits(256TB)地址空间足够了。

## heap

`556a8c01b000-556a8c03c000` 可以看到 heap 的地址，是比较小的地址段。

## heap 与 stack 会不会相互覆盖？

查看 /proc/pid/maps 计算：

(low address stack - high address heap) / 1024 / 1024 /1024 / 1024
= (0x7ffe91c09000 - 0x556a8c03c000) / 1024 / 1024 /1024 / 1024 = 42TB
可以看出相差有 42TB 的空间。就算真的覆盖了，内核会终止这个程序。这也就是虚拟内存的作用之一，程序可以使用远远超过实际物理内存的空间。

## rust 程序内存布局分析

```rust
static MAX: u32 = 0;
fn foo() {}
fn main() {
    let hello = "hello world".to_string();
    let data = Box::new(1);

    // string literals 指向 RODATA section 地址
    println!("RODATA: {:p}", "hello world!");

    // static 变量在 DATA section
    println!("DATA: (static var): {:p}", &MAX);

    // function 在 TEXT section
    println!("TEXT: (function):{:p}", foo as *const ());

    // string 结构体分配在栈上，所以其引用是指向的是一个栈地址
    println!("STACK: (&hello):{:p}", &hello);

    // 通过 deference 获取指向的 heap 上的数据，然后获取其引用
    println!("HEAP: (&(*hello)):{:p}", &(*hello));

    // Box 实现了 Pointer trait, 无须额外 deference
    println!("HEAP: (Box impl Pointer trait){:p},{:p}", data, &(data));
}

```
### 查看变量在虚拟内存中的分布

```bash
cargo run
   Compiling stack-or-heap v0.1.0 (/Users/stardust/Downloads/rust-projects/stack-or-heap)
    Finished dev [unoptimized + debuginfo] target(s) in 0.48s
     Running `target/debug/stack-or-heap`
RODATA: 0x106df1090
DATA: (static var): 0x106df1080
TEXT: (function):0x106db61b0
STACK: (&hello):0x7ff7b9149950
HEAP: (&(*hello)):0x60000346c050
HEAP: (Box impl Pointer trait)0x60000346c060,0x7ff7b9149968
```
# References

1. [/proc/pid/maps](https://www.baeldung.com/linux/proc-id-maps)
2. [virtual memory address space](https://en.wikipedia.org/wiki/Virtual_address_space)
3. [Memory_Management_in_Linux](https://elinux.org/images/b/b0/Introduction_to_Memory_Management_in_Linux.pdf)
