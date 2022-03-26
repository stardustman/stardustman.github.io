> 弄清楚 I/O Multiplexing 和 Linux 中 select, poll, epoll 之间的关系.

## multiplexer

> `Multiplexer` is a combinational circuit that has maximum of 2^n data inputs, `n` `selection lines` and `single output line`. One of these data inputs will be connected to the output based on the values of selection lines.
Since there are `n` selection lines, there will be 2^n possible combinations of zeros and ones. So, each combination will select only one data input. Multiplexer is also called as Mux.
> 聚合多个输入, 通过 selection lines 来选择一个输出.

### 4x1 Multiplexer 分析

> 聚合 I<sub>0</sub>, I<sub>1</sub>, I<sub>2</sub>, I<sub>3</sub> 这 4 个输入, 通过 S1 和 S0 来选择一个输出.

### Block diagram of 4x1 Multiplexer

![4x1_Multiplexer](https://github.com/stardustman/pictures/raw/main/img/4x1_Multiplexer.png)

### Y<sub>(output)</sub> 输出值的分析

#### S<sub>1</sub> = 0, S<sub>0</sub> = 0 时 Y 的值由 I<sub>0</sub> 决定

![multiplexers_S1_S0_I0](https://github.com/stardustman/pictures/raw/main/img/multiplexers_S1_S0_I0.png)
> S1 = 0, S0 = 0. 此时的 I1, I2, I3, 无论是 0 还是 1, 对应的 `and gate` 输出都是 0, 对最终输出 `Y` 没有影响. 此时 I0 的输入决定 `Y` 的值. 也就是 `Y` = I0.

#### S<sub>1</sub> = 0, S<sub>0</sub> = 1 时 Y 的值由 I<sub>1</sub> 决定

![multiplexers_S1_S0_I1](https://github.com/stardustman/pictures/raw/main/img/multiplexers_S1_S0_I1.png)
> S1 = 0, S0 = 1. 此时的 I0, I2, I3, 无论是 0 还是 1, 对应的 `and gate` 输出都是 0, 对最终输出 `Y` 没有影响. 此时 I1 的输入决定 `Y` 的值. 也就是 `Y` = I1.

#### S<sub>1</sub> = 1, S<sub>0</sub> = 0 时 Y 的值由 I<sub>2</sub> 决定

![multiplexers_S1_S0_I2](https://github.com/stardustman/pictures/raw/main/img/multiplexers_S1_S0_I2.png)
> S1 = 1, S0 = 0. 此时的 I0, I1, I3, 无论是 0 还是 1, 对应的 `and gate` 输出都是 0, 对最终输出 `Y` 没有影响. 此时 I2 的输入决定 `Y` 的值. 也就是 `Y` = I2.

#### S<sub>1</sub> = 1, S<sub>1</sub> = 0 时 Y 的值由 I<sub>3</sub> 决定

![multiplexers_S1_S0_I3](https://github.com/stardustman/pictures/raw/main/img/multiplexers_S1_S0_I3.png)
> S1 = 1, S0 = 1. 此时的 I0, I1, I2, 无论是 0 还是 1, 对应的 `and gate` 输出都是 0, 对最终输出 `Y` 没有影响. 此时 I3 的输入决定 `Y` 的值. 也就是 `Y` = I3.

### Truth table of 4x1 Multiplexer

S1(高位) | S0(低位) |S1S0(值)| Y
--| -- | -- | --
0 | 0 | 0 | I<sub>0</sub>
0 | 1 | 1 | I<sub>1</sub>
1 | 0 | 2 | I<sub>2</sub>
1 | 1 | 3 | I<sub>3</sub>

### Multiplexer (Mux)

> 4x1 Multiplexer 相当于提供了一种映射, Y = I<sub>(S1S0)</sub>.
> 通过组合S<sub>1</sub> 和 S<sub>0</sub> 的值, 可以直接得到 I<sub>S1S0</sub> 的值.也就是通过 4x1 Multiplexer 输出的 Y 值是 I<sub>0</sub>, I<sub>1</sub>, I<sub>2</sub>, I<sub>3</sub> 之一.

## I/O Multiplexing

> I/O multiplexing 这里面的 multiplexing 指的其实是在单个线程通过记录跟踪每一个 Sock(I/O 流)的状态.
> select, poll, epoll 都是 I/O multiplexing的具体的实现, 之所以有这三个存在，其实是他们出现是有先后顺序的.

## I/O Multiplexing 和 Multiplexer

> I<sub>0</sub>, I<sub>1</sub>, I<sub>2</sub>, I<sub>3</sub> 视为 4 个 I/O 流. 值为 1 时, 视为一个 I/O 流. 4x1 Multiplexer 作为硬件实现的 I/O multiplexing. 可以周期性的设置S<sub>1</sub> 和 S<sub>0</sub> 的值, 通过监测 Y 的输出, 得到 I<sub>0</sub>, I<sub>1</sub>, I<sub>2</sub>, I<sub>3</sub> 的输入.
{% asset_img multiplexers_S1_S0_I0.png multiplexers_S1_S0_I0 %}

### select

select 被实现以后, 很快就暴露出了很多问题.

* select 会修改传入的参数数组, 这个对于一个需要调用很多次的函数, 是非常不友好的.
* select 如果任何一个 sock(I/O stream) 出现了数据, select 仅仅会返回, 但是并不会告诉你是那个 sock 上有数,于是你只能自己一个一个的找.

> 相当于监测到 `Mutiplexer` 的 Y = 1, 但是不知道 I<sub>0</sub>, I<sub>1</sub>, I<sub>2</sub>, I<sub>3</sub>哪一个的输入是 1. 也就是不告诉 S<sub>1</sub> 和 S<sub>0</sub>的值, 那就只好再去具体查看一次.
I<sub>0</sub>, I<sub>1</sub>, I<sub>2</sub>, I<sub>3</sub>的值.

* select 只能监视 1024 个链接. 相当于实现了一个 1024x1 的 Mutiplexer. 只能有1024 个输入.
* select 不是线程安全的.

### poll

> 修复版的 select

* poll 去掉了 1024 个链接的限制

### epoll

epoll 可以说是 I/O 多路复用最新的一个实现, epoll 修复了 poll 和 select 绝大部分问题, 比如:

* epoll 现在是线程安全的。
* epoll 现在不仅告诉你 sock 组里面数据，还会告诉你具体哪个 sock 有数据, 你不用自己去找了.

> 相当于监测到  `Mutiplexer` 的 Y = 1, 且告知了此时的 S<sub>1</sub> 和 S<sub>0</sub> 的值, 通过组合就可以知道是哪一个具体的输入是 1 了.

## References

1. [circuits_multiplexers](https://www.tutorialspoint.com/digital_circuits/digital_circuits_multiplexers.htm)
2. [circuits_demultiplexers](https://www.tutorialspoint.com/digital_circuits/digital_circuits_demultiplexers.htm)
3. [epoll的本质](https://zhuanlan.zhihu.com/p/64138532)
4. [I/O multiplexing](https://www.zhihu.com/question/32163005)
