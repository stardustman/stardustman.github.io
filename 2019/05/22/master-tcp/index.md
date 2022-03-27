
> The Transmission Control Protocol (TCP) is intended for use as a highly
reliable **host-to-host** protocol between hosts in **packet-switched** computer communication networks, and in interconnected systems of such networks.

# TCP header format

![tcp header](https://github.com/stardustman/pictures/raw/main/img/master-tcp-tcp-header.png)

## basic data transfer

## reliability

* sequence number
* acknowledgement

## flow control

* window size

## multiplexing

* socket (ip, port)

## connections

> Each connection is uniquely specified by a **pair** of sockets identifying its two sides.(source ip, source port),(destination ip, destination port)

* sockets
* sequence numbers
* window size

# TCP 建立连接目的

> 交换通信双方的 ISN (Initial Sequence Number)
> 凡是占据序列号的任何TCP报文，一定对方确认，如果没有收到确认，会一直重传，直到达到规定的上限次数。
> syn , data , fin 占据 sequence number 需要确认. ack 不占据 sequence number 不需要确认.

# 参考链接

1. [TCP three way handshake 1]()
2. [TCP three way handshake 2]()
3. [rfc793](https://tools.ietf.org/html/rfc793)
