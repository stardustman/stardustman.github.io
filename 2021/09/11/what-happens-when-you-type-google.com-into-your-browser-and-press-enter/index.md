
## dns query

get the ip of the target domain

### browser cache

firefox(about:networking#dns)

### /etc/hosts

### os cache

1. macos 查看 dns 请求日志 sudo log stream --predicate 'process == "mDNSResponder"' --info


### dns resolver

/etc/resolv.conf

1. arp -a 查看有没有 dns 服务器的 mac 地址
2. ARP request for the nameserver
3. send dns query to get the ip of the domain

## arp 获取 gateway 的 mac 地址

## References

1. [keyborad-interrput](http://www.science.smith.edu/~nhowe/262/oldlabs/keyboard.html)
2. [Chrome 是怎么判断地址栏输入的东西是不是网址? -omnibox](https://www.zhihu.com/question/560616439/answer/2722866208)
3. [firefox dns](about:networking#dns) 
4. [ARP](https://www.fortinet.com/resources/cyberglossary/what-is-arp)
