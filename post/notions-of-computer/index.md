
## 指导思想

   计算机是人发明的。由计算机组成的世界，其设计思想很明显要借鉴现实中的东西。正如一位伟人的著作开篇提出 `“谁是我们的敌人？谁是我们的朋友？这个问题是革命的首要问题。中国过去一切革命斗争成效甚少，其基本原因就是因为不能团结真正的朋友，以攻击真正的敌人。”`那么应对计算机，什么才是计算机组织背后的指导思想呢？个人认为有三个：1. hierarchy(层级)、2：group(分组)、3：order(有序)

### hierarchy(层级)

  *hierarchy* 在这里也可以理解成等级。层级结构最重要的一点就是最顶层在哪？用 `tree` 理解就是，`root` 在哪？这个信息整个层级都要知道。哈哈，是不是挺像皇帝。😄

#### 面对数量庞大的相同事物，怎样管理呢？用什么组织方式呢？

   这里的数量庞大要是成千上万的。比如中国有十几亿人口、中国有 960 万平方公里的土地，等等。首先简述一下历史上的经验，比如西周，采用分封制。天子、诸侯、卿、士、平民等所有人组成一个金字塔形状。每一个层级的人`权力大小不同`、`承担的责任和义务也不同`。对于土地则是封邦建国。分封制可以对人口、土地等进行有效的统治。更直观的理解就是形成稳定的阶级。到这是不是立刻就能想到由于每种存储的`价格`、`速度`、`容量`不同，应用在哪个地方也不同。也是组织成一个层级结构来构成整个计算机的存储系统。

   从数学的角度分析就是：用`对数的方式`搞定数量庞大的事物的管理。不管数量多庞大，用对数搞一下，数量立刻就降下来。比如 log<sub>2</sub>4294967296 = 32。对数的发明、解析几何的创立和微积分的建立是17世纪数学的三大成就。

##### 域名组织方式

> In 2017, 330.6 million domain names had been registered.
> 2017 年已经有 33 多亿的注册域名了。

如果让你组织数量如此庞大的域名，你该怎样组织呢？没错，也是组织成层级结构，来管理数量如此庞大的域名。一开始的解决方法是直接把域名信息放在 /etc/hosts 文件，随着域名增多，单独的一个文件没法满足要求，数量太多了嘛，放在一个文件，且不论能不能全部存储，查找都是一个大问题。

![domain-name-hierarchy](https://github.com/stardustman/pictures/raw/main/img/dns-root-server.png)

##### IP 分配管理方式

IP Addresses are `unique identifiers` used to determine who is who on the Internet.
IP 地址是一个 32bit 的无符号整数。范围是 [0,2^32-1], 一共有 4294967296 个整数。将近 43 亿个 ip 地址(这里不考虑公网、私网)该怎样分配使用呢？由 [Internet Assigned Numbers Authority (IANA)](https://www.iana.org/numbers) 负责协调管理所有 IP，同时负责分配自治系统号(Autonomous System number)。
> Both IPv4 and IPv6 addresses are generally assigned in a `hierarchical manner`. Users are assigned IP addresses by `Internet service providers (ISPs)`. ISPs obtain allocations of IP addresses from a `local Internet registry (LIR`) or `National Internet Registry (NIR)`, or from their appropriate `Regional Internet Registry (RIR)`:

数量庞大的 IP 地址怎样分配管理呢？组成层级结构。通过一级一级`委托`，管理数量庞大的 IP 地址。

![地址分配管理](https://github.com/stardustman/pictures/raw/main/img/IP_AS_Number.svg)

##### 文件组织方式

![file system hierarchy](https://github.com/stardustman/pictures/raw/main/img/file_system_hierarchy.png)

#### 不同性质的事物，怎样组织呢？

##### memory hierarchy

![ComputerMemoryHierarchy](https://github.com/stardustman/pictures/raw/main/img/ComputerMemoryHierarchy.svg)

##### Typical memory hierarchy

![ typical-memory-hierarchy ](https://github.com/stardustman/pictures/raw/main/img/typical-memory-hierarchy.png)

##### OSI 参考模型

  网络每一层，功能不同。

![OSI VS TCP/IP](https://github.com/stardustman/pictures/raw/main/img/network-layer.svg)

### group(分组)

  体量庞大的东西的基层组织。

  1. 可视为层级结构的最底层。block、page、packet、fragment、set
  2. 也可以视为不同的模块。module、segment、section
  3. 也可以理解成分类。sort

####  域名

  对域名来说一个分组可以理解成一个`一级域名`下的所有域名。如 `org`下的域名。当然也可以理解成一个 `二级域名`下的所有域名。如 `google.com` 下 Google 家所有的域名。依次类推。可以看出，同种事物进行层级组织，本身就是在进行分组。

#### IP 地址

  > An autonomous system (AS) is a very large network or group of networks with a single routing policy. Each AS is assigned a unique ASN, which is a number that identifies the AS.

  没错。IP 地址也是通过一个一个的实体来管理使用的，一个实体就是一个自治系统。(autonomous system)。比如中国移动这个运营商的 AS Number 是 [56041](https://ipinfo.io/AS56041)，管理一系列 IP 地址。再比如 Apple 的 AS Number 是 [714](https://ipinfo.io/AS714)。

#### Internet Protocol

  > Data traversing the Internet is `divided into` smaller pieces, called `packets`. IP information is attached to `each packet`, and this information helps `routers` to send packets to the right place.

  通过网络发送大量的信息，也是拆分成一个个 `packet`。这里也可以理解成分组。

#### 文件系统

  文件系统管理磁盘空间。磁盘空间划分为一个个相同大小的 Block。一个扇区是 512Bytes，这可以视为一个分组。

####  网络流量
  
  > Ports are virtual places within an operating system where network connections start and end. They help computers sort the `network traffic` they receive.

  计算机从网络上接收的 packet 怎样判定哪一个是接收者呢？对网络流量分类就行。怎样分类呢？用端口号，不同的接收者，接收不同端口的流量。

####  subnet

  > Think of the Internet as a network of networks: computers are connected to each other within networks, and these networks connect to other networks. This enables these computers to connect with other computers both near and far.
  > A network is a group of two or more connected computing devices.

  划分子网，也是在分组。

### order(顺序)
  
  顺序、分支、循环。顺序就是程序三大结构之一。没有秩序的世界是混乱的，啥事也做不成。
  TCP 协议目的之一就是要保证计算机接收到的网络包的顺序。怎样保证顺序呢？非常简单，为每一个 Byte 编号就行。

## References

1. [memory-hierarchy](https://www.geeksforgeeks.org/memory-hierarchy-design-and-its-characteristics/)
2. [what-is-a-domain-name](https://www.cloudflare.com/learning/dns/glossary/what-is-a-domain-name/)
3. [dns-root-server](https://www.cloudflare.com/learning/dns/glossary/dns-root-server/)
4. [ip number resources](https://www.iana.org/numbers)
5. [The Internet Numbers Registry System](https://tools.ietf.org/html/rfc7020)
6. [Filesystem_Hierarchy_Standard](https://en.wikipedia.org/wiki/Filesystem_Hierarchy_Standard)
7. [Filesystem Hierarchy Standard List](https://www.pathname.com/fhs/2.2/)
8. [network-layer](https://www.cloudflare.com/learning/network-layer/what-is-the-network-layer/)
9. [internet-protocol](https://www.cloudflare.com/learning/network-layer/internet-protocol/)
10. [what-is-a-computer-port](https://www.cloudflare.com/learning/network-layer/what-is-a-computer-port/)
11. [open-systems-interconnection-model](https://www.cloudflare.com/learning/ddos/glossary/open-systems-interconnection-model-osi/)
12. [CSE141-Caching](http://cseweb.ucsd.edu/classes/sp10/cse141/pdf/07/09_CSE141-Caching.pdf)
