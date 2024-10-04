
> 怎样学习一个新的东西，也可以按照一定的套路的。这个东西解决什么问题？这个东西怎样解决问题？
> 还有没有其他方法？

## 三步走分析一下 DNS

### 解决什么问题

根据域名找到 IP。为啥要找到域名对应的 IP 呢？答案很简单，因为 `TCP/IP` 协议栈需要。
很常见的一个问题就是浏览器得到一个 web page 的流程是啥样的。

### 什么方式解决问题

怎样组织`域名(domain name)`？这是域名系统的首要问题。底层的组织架构，决定了域名解析的流程。有点经济基础决定上层建筑的意思。域名少的时候可以直接放在一个文件里。对，这个文件在类 unix 系统里就是 `/etc/hosts` 的文件。但是域名超级多怎么办？分类、组成层次结构。就像 Linux 的文件系统的组织方式一样。组织成一个树状结构。DNS 的查询问题，那就可以视为树的遍历问题。

#### top level domain

`com`、`org`、 `cn` 等等就是所谓的[顶级域名](http://data.iana.org/TLD/tlds-alpha-by-domain.txt)。也就是一级域名。那么问题就来了，这些顶级域名存放在哪呢？存放到 [root server](https://www.cloudflare.com/learning/dns/glossary/dns-root-server/)。换一种理解方法就是，这些 root server 就是顶级域名的 metadata。这些 [root server IP](https://www.iana.org/domains/root/servers) 是固定的。实现 DNS 这个功能的软件内置这些 root server 的 ip。这一点很重要。

#### second level domain

以 `www.google.com.` 为例 `google` 就是二级域名。其他的依此类推。

#### DNS 层级树

![DNS 层级树](https://github.com/stardustman/pictures/raw/main/img/dns-hierarchy.svg)

> 可以看到整个 DNS 的入口就是 root。也就是记录顶级域名在哪些服务器上的信息。这是 `root` 也就是不需要解析这些记录顶级域名的服务器。直接固定了这些 [root server 的 IP](https://www.iana.org/domains/root/servers) 地址。实现 DNS 功能的软件，直接内置这些固定的 IP。DNS 查询第一步就是向这些 root server 查询顶级域名对应哪些 nameserver，也就是顶级域名对应的 IP。其实 DNS也就是个分布式层级数据库。注册域名也就是向数据库添加一条记录。但是不是免费注册的。

#### DNS Zone

![DNS Zone](https://github.com/stardustman/pictures/raw/main/img/dns-zone.svg)

> 可以看到整个 DNS 层级树，每一个节点对应一个 Zone。

#### uncached DNS lookup

![dns-look-up](https://github.com/stardustman/pictures/raw/main/img/network-dns.svg)

> 图中 `OS stub resovler` 就是负责 DNS 解析的进程，发起 DNS 递归查询请求。
> `local dns server` 发起迭代查询，查询域名对应的 IP。之后返给 `OS stub resovler`(name servers that provide the complete answers to user queries)。

1. 这个进程向 `local dns server` 发起递归查询。也就是只需要等 `local dns server` 查询结果就行。
2. `local dns server` 发起迭代查询。因为 `local dns server` 有所有 root server 的 IP，选择一个 IP，根据顶级域名属于哪一个 `TLD Zone`， 发起迭代查询。
3. 可以查询得到记录这个顶级域名的 TLD server 有哪些，以及这些服务器的 IP。
4. `local dns server` 选择一个 TLD server，发起迭代查询。
5. 得到这个域名的权威服务器。
6. 选择一个权威服务器，发起迭代查询，查询域名对应的 IP。
7. `local dns server` 从权威服务器得到域名对应的 IP。
8. 返还给 `OS stub resovler`。递归查询结束。

> Systems normally lookup any name (including domain names) using the hosts file first (if present), followed by DNS.
> However, the nsswitch.conf file (typically in /etc) controls this order (normally hosts: file dns), allowing the order to be changed or the file value to be deleted entirely depending on local needs.

### 有没有其他解决方案

## dig 命令

### dig domainname

```
 ~ dig github.com

; <<>> DiG 9.10.6 <<>> github.com
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 23033
;; flags: qr rd ra; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 0

;; QUESTION SECTION:
;github.com.   IN A

;; ANSWER SECTION:
github.com.  17 IN A 52.74.223.119  # github.com. 这个域名对应的一个 ip

;; Query time: 14 msec
;; SERVER: 61.139.2.69#53(61.139.2.69)
;; WHEN: Thu Apr 23 19:53:17 CST 2020
;; MSG SIZE  rcvd: 44

```

### wireshark 抓包验证实际的网络架构

#### TCP/IP stack

![tcp-ip-stack](https://github.com/stardustman/pictures/raw/main/img/tcp-ip-stack-dns.png)

### dig +trace domainname

```bash
~ dig +trace github.com

; <<>> DiG 9.10.6 <<>> +trace github.com
;; global options: +cmd
.   81219 IN NS k.root-servers.net.
.   81219 IN NS e.root-servers.net.
.   81219 IN NS b.root-servers.net.
.   81219 IN NS a.root-servers.net.
.   81219 IN NS m.root-servers.net.
.   81219 IN NS l.root-servers.net.
.   81219 IN NS d.root-servers.net.
.   81219 IN NS f.root-servers.net.
.   81219 IN NS g.root-servers.net.
.   81219 IN NS h.root-servers.net.
.   81219 IN NS j.root-servers.net.
.   81219 IN NS c.root-servers.net.
.   81219 IN NS i.root-servers.net.
;; Received 228 bytes from 61.139.2.69#53(61.139.2.69) in 15 ms #local dns server，本地 DNS 拥有完整的 root server ip 列表。内置到本地 DNS 服务器。

com.   172800 IN NS l.gtld-servers.net.  # 记录 com. 这个顶级域名的服务器，generic top level domain server
com.   172800 IN NS b.gtld-servers.net.
com.   172800 IN NS c.gtld-servers.net.
com.   172800 IN NS d.gtld-servers.net.
com.   172800 IN NS e.gtld-servers.net.
com.   172800 IN NS f.gtld-servers.net.
com.   172800 IN NS g.gtld-servers.net.
com.   172800 IN NS a.gtld-servers.net.
com.   172800 IN NS h.gtld-servers.net.
com.   172800 IN NS i.gtld-servers.net.
com.   172800 IN NS j.gtld-servers.net.
com.   172800 IN NS k.gtld-servers.net.
com.   172800 IN NS m.gtld-servers.net.
com.   86400 IN DS 30909 8 2 E2D3C916F6DEEAC73294E8268FB5885044A833FC5459588F4A9184CF C41A5766
com.   86400 IN RRSIG DS 8 1 86400 20200506050000 20200423040000 48903 . Enqd4bffSJHlv8mqHcAptS9+fHo3cg6vCIQgrOZ3aBfx2nG4CjS27iEG 6u6NsPfcKVrz7RoU2xDtDMNJOcMBJdJe/lGNeo14N69SM0/MV2Z8wZBD HRPd2Y8Z1nyc7EnDbkFzuWV5G6vafaQa4KTQpG6jGOPIQpYESNiinxfy 0QQWUJELvjSOideCPqQcug2P2Pln7BzWiqFSg6I8d1h44349LsXcbr5W zJwZQJKLoDd0ysyGl5Bq3UZgfWYvTJFmmZ/OwKYT4QQ4BE0MtbVwae8X 9yI6d9jENsz+eebEEtxLt6o/LNQb3PdNhUH1PC1pxfHiEiNyYUcvbJdf Flgntg==
;; Received 1170 bytes from 192.203.230.10#53(e.root-servers.net) in 211 ms # 从 e.root-servers.net 这个 root server 查询到的 TLD namesever，也就是存储 com. 这个顶级域名的服务器。

github.com.  172800 IN NS ns1.p16.dynect.net. # 记录 github.com. 这个域名的权威服务器
github.com.  172800 IN NS ns3.p16.dynect.net.
github.com.  172800 IN NS ns2.p16.dynect.net.
github.com.  172800 IN NS ns4.p16.dynect.net.
github.com.  172800 IN NS ns-520.awsdns-01.net.
github.com.  172800 IN NS ns-421.awsdns-52.com.
github.com.  172800 IN NS ns-1707.awsdns-21.co.uk.
github.com.  172800 IN NS ns-1283.awsdns-32.org.
CK0POJMG874LJREF7EFN8430QVIT8BSM.com. 86400 IN NSEC3 1 1 0 - CK0Q1GIN43N1ARRC9OSM6QPQR81H5M9A  NS SOA RRSIG DNSKEY NSEC3PARAM
CK0POJMG874LJREF7EFN8430QVIT8BSM.com. 86400 IN RRSIG NSEC3 8 2 86400 20200427045033 20200420034033 39844 com. BDAWMm4AsSRfrBwOXhNN8ihw++J2UkhEk8A6LeSIz1llDs5kt0CcOqTM OK1txlQNr3N0k1RTBW/hkEZ1mxjIHF2GARt/hpO43ILPBVs/vhKAKCKh A96XJG+NzN0t/heB6mFNASKAcmVdfT5a0tVkmmzhGktDp5ECdZAIFRhZ 8y6jxsUhGP7ZiN1AfFBi1cWeSth5FW4Btpy9RffVKg5lIA==
4KB4DFS71LEP8G8P8VT4CCUSQNL4CNCS.com. 86400 IN NSEC3 1 1 0 - 4KB4PTQQ5CTA7POCTGM7RUFC8B1RKTEU  NS DS RRSIG
4KB4DFS71LEP8G8P8VT4CCUSQNL4CNCS.com. 86400 IN RRSIG NSEC3 8 2 86400 20200428061912 20200421050912 39844 com. PFN1p6XvR4SmD4ucRULmtYIMQkFf/ZrdEWz6bRVtcepYF6QwTC/VOVYM 7PV0tuTTdr1LqkUwQJ7mv23vtKnjFba2Cf7pBOTz9JuCHUC0Qml9cOc6 Cwjhx7haj6h0VEzS+oIY7l2p/af/B36iQCXASVN3Y6s/i+66iXIg905z GqFnAKOgR0R4KJx5KwHIzBkgxr/I/SyCd+HNw+2fr15X0w==
;; Received 824 bytes from 192.26.92.30#53(c.gtld-servers.net) in 173 ms # 从 c.gtld-servers.net 这个 TLD nameserver 查询到存储 github.com. 这个域名的权威服务器。

github.com.  60 IN A 13.250.177.223 # 得到了 github.com. 这个域名的一个 ip
github.com.  900 IN NS ns-1707.awsdns-21.co.uk.
github.com.  900 IN NS ns3.p16.dynect.net.
github.com.  900 IN NS ns2.p16.dynect.net.
github.com.  900 IN NS ns-1283.awsdns-32.org.
github.com.  900 IN NS ns-421.awsdns-52.com.
github.com.  900 IN NS ns-520.awsdns-01.net.
github.com.  900 IN NS ns4.p16.dynect.net.
github.com.  900 IN NS ns1.p16.dynect.net.
;; Received 275 bytes from 204.13.250.16#53(ns2.p16.dynect.net) in 198 ms 从 ns2.p16.dynect.net 这个权威服务器查询到的 github.com. 的 ip 是 13.250.177.223。至此，得到域名对应的 ip。

```

## DNS 记录类型

### CNAME

`Canonical Name` 域名指向另一个域名。见下图：

```bash
 dig -t cname music.163.com

; <<>> DiG 9.10.6 <<>> -t cname music.163.com
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 12400
;; flags: qr aa rd ra; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 0

;; QUESTION SECTION:
;music.163.com.			IN	CNAME

;; ANSWER SECTION:
music.163.com.		1	IN	CNAME	music.ntes53.netease.com.

;; Query time: 0 msec
;; SERVER: 198.18.0.2#53(198.18.0.2)
;; WHEN: Fri Oct 06 11:41:17 CST 2023
;; MSG SIZE  rcvd: 82
```

### A

`address` ip address

### NS

`name server` 保存下一级域名的服务器，只设置成域名。

### PTR

`pointer record`，根据 ip 查询域名。类似 ARP，用 ip 找 MAC 地址。

### MX

mail exchange 电子邮件服务器

### TXT

[可以获取这个网站在使用那些第三方服务](https://allagora.wordpress.com/2022/03/22/use-dns-txt-lookup-to-gain-quick-insights-which-services-companies-are-using/)

## Reference

1. [DNS 入门](https://www.ruanyifeng.com/blog/2016/06/dns.html)
2. [Introduction and Layered Network Architecture](http://kilyos.ee.bilkent.edu.tr/~ee538/lecture1.pdf)
3. [root servers ips](https://www.iana.org/domains/root/servers)
4. [all-root-servers](https://root-servers.org/)
5. [rfc-dns](https://tools.ietf.org/html/rfc2929)
6. [dns-server-types](https://www.cloudflare.com/learning/dns/dns-server-types/)
7. [what is dns](https://www.cloudflare.com/learning/dns/what-is-dns/)
8. [what is recursive dns](https://www.cloudflare.com/learning/dns/what-is-recursive-dns/)
9. [dns record types](https://simpledns.plus/help/dns-record-types)
10. [ascii](https://www.cs.cmu.edu/~pattis/15-1XX/common/handouts/ascii.html)
11. [dns-zone](https://www.cloudflare.com/learning/dns/glossary/dns-zone/)
12. [zone-file](https://bind9.readthedocs.io/en/v9.18.21/chapter3.html#localhost-zone-file)
13. [viewdns](https://viewdns.info/)
14. [dns record txt](https://allagora.wordpress.com/2022/03/22/use-dns-txt-lookup-to-gain-quick-insights-which-services-companies-are-using/)
15. [https://www.nslookup.io/](https://www.nslookup.io/)
16. [域名信息查询](https://whois.gandi.net/zh-hans)
17. [dns延迟测试](https://www.dnsperf.com/)
18. [dns实践](https://messwithdns.net/)
