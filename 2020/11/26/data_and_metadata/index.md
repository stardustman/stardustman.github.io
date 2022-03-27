
> 具体问题具体分析，确实非常重要。用正确的思想指导行动，才可事半功倍。
> 现实中的客观问题有意思的地方在于：无论你选择看得见，还是选择看不见，它都在那里，关键在于有没有人去解决，解决的人有多大的决心。

## data vs metadata

> Metadata is "data that provides information about other data". In other words, it is "`data about data`".
正如这个世上的人，无论承认与否，分为两类：统治者与被统治者。数据也不可避免的被分为两种：metadata 和 data。`数据的数据`和`数据`。两者相辅相成。

### physical

data  数据(被统治者)
metadata 元数据(统治者)
正如如果被统治者都没有了，那也谈不上统治者。在一个个实体确实存在的情况下，如果没有数据，那也谈不上元数据。

### logical

有时没有实体的存在但是也可以在`逻辑上`划分 data 和 Metadata 的。比如网络中的流量。仅仅是用户真正的数据吗？不，其实还有路由器之间的流量, 这部分流量对于整个网络系统的正常运行，不可缺少。

The control plane is the part of a network that controls `how data is forwarded`, while the data plane is the actual forwarding process.
那么网络流量就可以在逻辑上（也就是虚拟的）划分为：
control plane 控制平面（视为 Metadata）
data/forward plane 数据平面（视为 data）
从这个角度来理解，metadata 有控制数据的意思，控制真正的用户数据如何转发。虽然大家都是同一种事物(都是数据包)，但是又不是平等的事物。咳咳，换成人类，似乎也说得过去。

### structural metadata

这种结构性的 metadata 相当于一份说明书，来说明哪一个位置的数据是什么数据。

#### java class format

> Java 代码经过编译，可以的到一个 .class 文件。

##### Hello.java

```java
public class Hello {
    public static void main(String[] args) {
        System.out.println("Hello, World");
    }
}
```

##### Hello.class

```bash
root@aliyun:java# xxd Hello.class
00000000: cafe babe 0000 0034 001d 0a00 0600 0f09  .......4........
00000010: 0010 0011 0800 120a 0013 0014 0700 1507  ................
00000020: 0016 0100 063c 696e 6974 3e01 0003 2829  .....<init>...()
00000030: 5601 0004 436f 6465 0100 0f4c 696e 654e  V...Code...LineN
00000040: 756d 6265 7254 6162 6c65 0100 046d 6169  umberTable...mai
00000050: 6e01 0016 285b 4c6a 6176 612f 6c61 6e67  n...([Ljava/lang
00000060: 2f53 7472 696e 673b 2956 0100 0a53 6f75  /String;)V...Sou
00000070: 7263 6546 696c 6501 000a 4865 6c6c 6f2e  rceFile...Hello.
00000080: 6a61 7661 0c00 0700 0807 0017 0c00 1800  java............
00000090: 1901 000c 4865 6c6c 6f2c 2057 6f72 6c64  ....Hello, World
000000a0: 0700 1a0c 001b 001c 0100 0548 656c 6c6f  ...........Hello
000000b0: 0100 106a 6176 612f 6c61 6e67 2f4f 626a  ...java/lang/Obj
000000c0: 6563 7401 0010 6a61 7661 2f6c 616e 672f  ect...java/lang/
000000d0: 5379 7374 656d 0100 036f 7574 0100 154c  System...out...L
000000e0: 6a61 7661 2f69 6f2f 5072 696e 7453 7472  java/io/PrintStr
000000f0: 6561 6d3b 0100 136a 6176 612f 696f 2f50  eam;...java/io/P
00000100: 7269 6e74 5374 7265 616d 0100 0770 7269  rintStream...pri
00000110: 6e74 6c6e 0100 1528 4c6a 6176 612f 6c61  ntln...(Ljava/la
00000120: 6e67 2f53 7472 696e 673b 2956 0021 0005  ng/String;)V.!..
00000130: 0006 0000 0000 0002 0001 0007 0008 0001  ................
00000140: 0009 0000 001d 0001 0001 0000 0005 2ab7  ..............*.
00000150: 0001 b100 0000 0100 0a00 0000 0600 0100  ................
00000160: 0000 0100 0900 0b00 0c00 0100 0900 0000  ................
00000170: 2500 0200 0100 0000 09b2 0002 1203 b600  %...............
00000180: 04b1 0000 0001 000a 0000 000a 0002 0000  ................
00000190: 0003 0008 0004 0001 000d 0000 0002 000e  ................
```

#### Hello.class 怎样解析的？

如果丢给我一个 .class 文件，其他啥也没有。我也看不懂。但是由于 `class file format` 的存在，只要足够耐心，自己也可以解析出 .class 是怎样的。这里的 [class format](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html) 就是 Metadata。没有这个格式说明，谁也弄不明白这一堆二进制的 0 和 1 是啥意思。

#### IP header

IP 这个协议是用来将不同局域网组成互联网的。既然通信是双向的，要知道发送者的 IP，接收者的 IP，IP 包的总大小，IP Header 的大小，这个 IP 包能跨越多少路由器([TTL](https://www.cloudflare.com/learning/cdn/glossary/time-to-live-ttl/))，使用的 IP 协议版本，是哪一个上层协议在使用 IP 协议，IP 包是否切分了，切分后怎样组装，校验整个 IP 包在传输过程中是否发生了错误等信息。记在哪里呢？IP header，这个 IP header 就是 Metadata。这些也可理解成控制数据。

#### TCP header

TCP 传输层的协议。用来保证数据不丢失且按照发送顺序到达目的地。
怎样保证顺序呢？对每一个字节编号(sequence number)。
怎样保证数据不丢失呢？确认机制(ack)。
怎样找到目的地呢？一个在网络上的计算机，可能同时运行许多程序，如打开浏览器，在线听歌，在线编辑文档等等。用端口号(port number)。
同时，怎样知道接收方现在接收网络包的大小呢？用窗口(window size)。
怎样能知道网络包传输过程是否发生了错误？用校验和(checksum)。
这些信息都存在哪里了呢？TCP header 里。这就是 Metadata。也可理解成控制数据。

#### InnoDB row format

InnoDB 存储表是按行存储的。这只是逻辑上的说法。真正的数据存储是要落到磁盘上的。对于每一行，字段是不是空的，定长还是变长，变长字段的长度，该列的主键，这行数据是哪一个事务操作的，这些记录的相关日志都记在哪里了呢？
不错。这其实也是有相应的说明的。而且，InnoDB 提供了不同的 [row format](https://mariadb.com/kb/en/innodb-row-formats-overview/)。对用户的每一行来说，该行的 row format 就是 Metadata。当然这就是描述（或者说是控制）该行用户数据的。

## Reference

1. [Metadata](https://en.wikipedia.org/wiki/Metadata)
2. [what-is-metadata](https://dataedo.com/kb/data-glossary/what-is-metadata)
3. [what-is-the-control-plane](https://www.cloudflare.com/learning/network-layer/what-is-the-control-plane/)
4. [class file format](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html)
5. [InnoDB-row-format](https://mariadb.com/kb/en/innodb-row-formats-overview/)
6. [TTL](https://www.cloudflare.com/learning/cdn/glossary/time-to-live-ttl/)
