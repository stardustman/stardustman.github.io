# storage

## Disk Controller

> 我们知道定位一个磁盘空间需要三个参数(platter, track, sector). 但是 cpu 不使用这么麻烦的方式, cpu 使用的是逻辑盘号. 也就是磁盘控制器将逻辑盘号翻译成 platter, track, sector. 磁盘控制器充当一个中间层. 也就是只关心逻辑盘号, 不关心具体的(platter, track, sector).
> 通过逻辑盘号将三维的(platter, track, sector) 转换为一维数组, 这就是抽象的意义啊. 这就是降维思维嘛.

## Memory Hierarchy

> 我们知道不同类型的存储, 每一个 bit 的价格不一样, 元件不同, 速度也不同. 世上事情大抵如此,不会集所有优点于一身.  
> 利用程序的局部性, 构造一个层次存储系统, 每一层的存储当做下一层的缓存,(也就是下一层的一部分)这才会有 hit 和 miss 的根本原因啊! 这才会引出缓存的 placement policy 和 replacement policy. 缓存这种思想和普遍, 做开发时 redis 作为数据库的缓存, 也是解决速度不匹配的问题.
