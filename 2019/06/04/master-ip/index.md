
# Motivation

  The Internet Protocol is designed for use in interconnected systems of **packet-switched** computer communication networks.  Such a system has been called a "catenet".  The internet protocol provides for transmitting blocks of data called datagrams from sources to destinations, where sources and destinations are hosts identified by **fixed length addresses**.  The internet protocol also provides for **fragmentation** and **reassembly** of long datagrams, if necessary, for transmission through "small packet" networks.
> IPv4 is a connectionless protocol for use on packet-switched networks. It operates on a best effort delivery model, in that it does not guarantee delivery, nor does it assure proper sequencing or avoidance of duplicate delivery. These aspects, including data integrity, are addressed by an upper layer transport protocol, such as the Transmission Control Protocol (TCP).

# IP header format

  ![ip-header-format](https://github.com/stardustman/pictures/raw/main/img/ip-header-format.png)

# Ip datagram

![ip-format](https://github.com/stardustman/pictures/raw/main/img/ip-format.png)

# IPV4 example

![ip-capture-wireshark](https://github.com/stardustman/pictures/raw/main/img/ip-capture-wireshark.jpg)

![ip-capture-wireshark-example](https://github.com/stardustman/pictures/raw/main/img/ip-capture-wireshark-example.png)

## version(4 bits)

0x4(4)

## Header Length(4 bits)

0x5(5)

## Type Of Service(TOS)(8 bits)

0x00(0)

## Total Length(16 bits)

0x28(40)

## Identification(16 bits)

0xd279

## Flags(3 bits)

0x4 0100

1. 0 Reserved bit not set
2. 1 Don't fragment set
3. 0 More fragments not set

## Fragment Offset(13 bits)

> This field indicates where in the datagram this fragment belongs.
> The fragment offset is measured in units of 8 octets (64 bits). The
first fragment has offset zero.

0x000 (0000 0000 0000)

## Time to Live(TTL)(8 bits)

0x80 TTL:(128)

## Protocal(8 bits)

0x06 TCP:(6)

## Header Checksum(16 bits)

0x2e35

## Source IP Address(32 bits)

0x 0a:fe:c9:ad -> 10.254.201.173

## Destination IP Address(32 bits)

0x ac:10:79:65 -> 172.16.121.101

# References

1. [ip fragmentation and reassembly](http://user.it.uu.se/~rmg/teaching/IP.pdf)
2. [fragmentation & reassembly](https://en.m.wikipedia.org/wiki/IPv4#Fragmentation_and_reassembly)
3. [ip 重组](https://mp.weixin.qq.com/s/ifZk5Ox9cnixq1ZxgwFBSw)
