
> There can be many caches `stacked` on top of each other.
> Cache 可以一层一层累积。

1. if you miss in one you try in the “lower level cache”
Lower level, mean higher number.
在上层的 Cache miss 了，可以在下层的 Cache 去找。依次类推。
2. There can also be `separate` caches for data and
instructions. Or the cache can be `“unified”`.
数据和指令的 Cache 可以独立，也可以混合。
3. to wit:
   * the `L1 data cache (d-cache)` is the one nearest processor. It
   corresponds to the “data memory” block in our pipeline
   diagrams
   * the `L1 instruction cache (i-cache)` corresponds to the
   “instruction memory” block in our pipeline diagrams.
   * The L2 sits `underneath` the L1s.
   * There is often an `L3 in modern systems`.

## cache 指导思想

### 局部性原理

#### temporal locality (时间局部性)

Taking advantage of temporal locality:

1. bring data into cache whenever its referenced
2. kick out something that hasn’t been used recently

#### spatial locality (空间局部性)

Taking advantage of spatial locality:
bring in a block of `contiguous data (cacheline)`, not just
the `requested data`.

## 基本问题

### 怎样找到 cache 中的数据？

> 以 32bits 的内存地址，来分析。
> (index, offset) 二维坐标来定位一个 byte 的数据。

#### index

 定位 cache line，可视为`横坐标`。
 现在可以通过 cache lines = cache size / cache line size 来计算。
 index bits = log2(cache lines)

#### offset

 通过 index 定位到 cache line 后，offset 定位到这个 cache line 的哪一个 byte。
 可以视为`纵坐标`。 
 offset bits = log2(offset)

#### tag

32bits 剩下的部分。

#### cache line 应该是多大？

> 这其实是分块(block)思想。在利用空间局部性原理。

##### cache line size 越大

1. Exploits more spatial locality.
2. Large cache lines effectively prefetch data that we have not
explicitly asked for.
更好地利用空间局部性，提前获取将要访问的数据。

##### cache line size 越小

1. Focuses on temporal locality.
2. If there is little spatial locality, large cache lines waste
space and bandwidth.
聚焦在时间局部性上，如果没有较好的空间局部性，提前 `fetch` 了数据，那就浪费了空间和带宽。

![cache_organization](https://github.com/stardustman/pictures/raw/main/img/cache_organization.svg) #(cache 组织方式)

### 实例分析

内存地址 32bits。

#### 1024 cache lines, 32 Bytes per line.

index bits = log2(1024) = 10
offset bits = log2(32) = 5
tags bits = 32 - index - offset = 17

#### 32KB cache, 64byte lines

index bits = log2(32KB / 64Bytes) = 9
offset bits = log2(64) = 6
tags bits = 32 - 9 - 6 = 17

#### set 是干什么的？

(set) `Associativity` means providing `more than one` place for a cache line to live.
One group of lines corresponds to each index.

1. it is called a “set”
2. `Each line` in a set is called a `“way”`
3. N-Way associativity requires N parallel comparators
set = ？

{% asset_img cache-2-ways-cache.svg 2-ways-cache%}
![2-ways-cache](https://github.com/stardustman/pictures/raw/main/img/cache-2-ways-cache.svg)

### cache 哪一种 address？

#### virtual memory address

#### physical memory address

![cache-memory-address-type](https://github.com/stardustman/pictures/raw/main/img/cache-memory-address-type.svg) #(缓存类型)

## References

1. [CSE141-Caching-Intro](http://cseweb.ucsd.edu/classes/wi12/cse141-a/Slides/09_Cache_intro.pdf)
2. [CSE141-Caching](http://cseweb.ucsd.edu/classes/sp10/cse141/pdf/07/09_CSE141-Caching.pdf)
3. [CSE141-Caches-Details](http://cseweb.ucsd.edu/classes/wi12/cse141-a/Slides/10_Caches_detail.pdf)
4. [x86-cache](https://www.0xffffff.org/2014/01/06/26-x86-cache/)
5. [memory-hotplug](https://www.kernel.org/doc/html/latest/admin-guide/mm/memory-hotplug.html)
6. [Cache 是怎样组织和工作的？](https://zhuanlan.zhihu.com/p/31859105)
7. [cacheinfo-windows](https://docs.microsoft.com/en-us/sysinternals/downloads/coreinfo)
8. [intel-cpu-cache](https://manybutfinite.com/post/intel-cpu-caches/)
9. [caches3-w.pdf](http://www.cs.cornell.edu/courses/cs3410/2013sp/lecture/18-caches3-w.pdf)
10. [wiki-cpu-cache](https://en.wikipedia.org/wiki/CPU_cache)
11. [cos316-10-cpu-cache.pdf](https://www.cs.princeton.edu/courses/archive/fall19/cos316/lectures/10-cpu-cache.pdf)
12. [cos355-Intel-CacheOverview](http://aturing.umcs.maine.edu/~meadow/courses/cos335/Intel-CacheOverview.pdf)
13. [细说Cache-L1/L2/L3/TLB](https://zhuanlan.zhihu.com/p/31875174)