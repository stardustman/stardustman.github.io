
> 分析的 zookeeper 代码版本为 3.9.1 。    
> zookeer 的 transaction log 为二进制文件，采用的是大端序。  
> zookeeper 数据持久化的功能在  `zookeeper/server/persistence` 下。  
> 解析日志就可以获取 zookeeper 的数据。可以用来实现实时备份到另一个独立的 zookeeper 集群。

# 日志文件命名规则

```java
/**
    * Creates a valid transaction log file name.
    *
    * @param zxid used as a file name suffix (extension)
    * @return file name
    */
public static String makeLogName(long zxid) {
    return FileTxnLog.LOG_FILE_PREFIX + "." + Long.toHexString(zxid);
}
```

# 日志文件布局

由 File Header、TxnList、ZeroPad 三部分构成

![zk-transaction-log-file-format](https://raw.githubusercontent.com/stardustman/pictures/main/img/zookeeper-transaction-log-file-format.svg)

# `log.500000004` 文件分析

分析的日志文件 [log.500000004](https://raw.githubusercontent.com/stardustman/pictures/main/img/log.500000004)

## 二进制文件查看

```bash
xxd log.500000004.new|head -n 50
00000000: 5a4b 4c47 0000 0002 0000 0000 0000 0000  ZKLG............
00000010: 0000 0000 eaa1 0b2a 0000 0030 0300 c755  .......*...0...U
00000020: cf61 0000 0000 0000 0000 0005 0000 0004  .a..............
00000030: 0000 018b 1815 73ab ffff fff6 0000 7530  ......s.......u0
00000040: 0000 0002 0000 000a a5c7 7377 4200 0000  ..........swB...
00000050: 005f 8e12 2a00 0000 5f03 00c7 55cf 6100  ._..*..._...U.a.
00000060: 0000 0000 0100 0000 0500 0000 0500 0001  ................
00000070: 8b18 159e 3300 0000 0100 0000 0b2f 612f  ....3......../a/
00000080: 7374 6172 6475 7374 ffff ffff 0000 0001  stardust........
00000090: 0000 001f 0000 0005 776f 726c 6400 0000  ........world...
000000a0: 0661 6e79 6f6e 6500 0000 000d 0000 0002  .anyone.........
000000b0: 0000 000c 6113 4355 4200 0000 0007 0a0e  ....a.CUB.......
000000c0: 4000 0000 4c03 00c7 55cf 6100 0000 0000  @...L...U.a.....
000000d0: 0200 0000 0500 0000 0600 0001 8b18 15e5  ................
000000e0: b800 0000 0500 0000 0b2f 612f 7374 6172  ........./a/star
000000f0: 6475 7374 0000 0009 7468 6553 686f 6775  dust....theShogu
00000100: 6e00 0000 0100 0000 0200 0000 0c14 8609  n...............
00000110: d442 0000 0000 e9ee 12d7 0000 0058 0300  .B...........X..
00000120: c755 cf61 0000 0000 0003 0000 0005 0000  .U.a............
00000130: 0007 0000 018b 1816 66ef 0000 0005 0000  ........f.......
00000140: 000b 2f61 2f73 7461 7264 7573 7400 0000  ../a/stardust...
00000150: 1574 6865 3133 4c6f 6164 734f 6654 6865  .the13LoadsOfThe
00000160: 5368 6f67 756e 0000 0002 0000 0002 0000  Shogun..........
00000170: 000c 39b4 35f7 4200 0000 001a cb0a c700  ..9.5.B.........
00000180: 0000 3b03 00c7 55cf 6100 0000 0000 0500  ..;...U.a.......
00000190: 0000 0500 0000 0800 0001 8b18 167c ed00  .............|..
000001a0: 0000 0200 0000 0b2f 612f 7374 6172 6475  ......./a/stardu
000001b0: 7374 0000 0002 0000 000b 676d 37e4 4200  st........gm7.B.
000001c0: 0000 00ef 370a d300 0000 3003 00c7 55cf  ....7.....0...U.
000001d0: 6100 0000 0000 0000 0000 0500 0000 0900  a...............
000001e0: 0001 8b18 16f9 d4ff ffff f500 0000 0000  ................
000001f0: 0000 0200 0000 0b67 6d37 e442 0000 0000  .......gm7.B....
00000200: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000210: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000220: 0000 0000 0000 0000 0000 0000 0000 0000  ................

```
## log 二进制文件图解

![log.50000004.jpg](https://raw.githubusercontent.com/stardustman/pictures/main/img/zookeeper-log.500000004.jpg)

### log.`500000004` 怎么来的？

> 可以看到 txn log 文件，写入的第一个 transaction 的 `zxid` 就是 `0x0000000500000004`。这个就是 log.`500000004` 的由来。

### 什么是 `zxid`？

![zookeeper-zxid](https://raw.githubusercontent.com/stardustman/pictures/main/img/zookeeper-zxid-format.svg)

### 文件结构

1. 第一行 FileHeader。
2. 第二行开始是 transaction log, 每一个 transaction log 用 0x42 作为 EOF。可以知道 `log.500000004`有 6 个 transaction。
3. 剩余的是 Zero Pad。

### transaction(txn) 类型

> 仅分析 `log.500000004`中的 6 个。还有其他类型的 txn。 

#### **transaction 代码位置**

zookeeper-jute 中 org.apache.zookeeper.txn 下, 这个是 jute compiler 生成的一系列类。

#### **create session txn**

>  第 1 个 txn 的 type= 0xfffffff6=-10  
> [OpCode=-10](https://github.com/apache/zookeeper/blob/release-3.9.1/zookeeper-server/src/main/java/org/apache/zookeeper/ZooDefs.java#L98)

#### **create txn**

>  第 2 个 txn 的 type= 0x00000001=1  
> [OpCode=1](https://github.com/apache/zookeeper/blob/release-3.9.1/zookeeper-server/src/main/java/org/apache/zookeeper/ZooDefs.java#L40)


#### **set data txn**

>  第 3、4 个 txn 的 type= 0x00000005=5  
> [OpCode=5](https://github.com/apache/zookeeper/blob/release-3.9.1/zookeeper-server/src/main/java/org/apache/zookeeper/ZooDefs.java#L40)


#### **delete txn**

>  第 5 个 txn 的 type= 0x00000002=2  
> [OpCode=2](https://github.com/apache/zookeeper/blob/release-3.9.1/zookeeper-server/src/main/java/org/apache/zookeeper/ZooDefs.java#L42)

#### **close session txn**

>  第 6 个 txn 的 type= 0xfffffff5=-11  
> [OpCode=-11](https://github.com/apache/zookeeper/blob/release-3.9.1/zookeeper-server/src/main/java/org/apache/zookeeper/ZooDefs.java#L100)





# References
1. [zookeeper/server/persistence](https://github.com/apache/zookeeper/tree/release-3.9.1/zookeeper-server/src/main/java/org/apache/zookeeper/server/persistence)
2. [ZooDefs#OpCode](https://github.com/apache/zookeeper/blob/release-3.9.1/zookeeper-server/src/main/java/org/apache/zookeeper/ZooDefs.java#L36)
3. [FileTxnLog.java](https://github.com/apache/zookeeper/blob/release-3.9.1/zookeeper-server/src/main/java/org/apache/zookeeper/server/persistence/FileTxnLog.java#L57)
4. [Util#makeLogName](https://github.com/apache/zookeeper/blob/release-3.9.1/zookeeper-server/src/main/java/org/apache/zookeeper/server/persistence/Util.java#L84)
5. [log.500000004](https://raw.githubusercontent.com/stardustman/pictures/main/img/log.500000004)
6. [zklogtool](https://github.com/alenca/zklogtool/tree/master/zklogtool)
7. [hexyl-二进制分析工具](https://github.com/sharkdp/hexyl/releases)
8. [dataLogDir](https://zookeeper.apache.org/doc/r3.4.8/zookeeperAdmin.html#sc_zkCommands:~:text=The%20Log%20Directory%20contains,written%20to%20that%20log.)




