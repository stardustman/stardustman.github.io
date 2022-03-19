
## Processes

> Containers are just normal Linux Processes with additional configuration applied.

容器只是应用了附加配置的普通 Linux 进程。

### redis container

#### lanch redis-server process

> Usage:  docker run [OPTIONS] IMAGE [COMMAND] [ARG...]
> docker run --help
> -d, --detach   Run container in background and print container ID
> --name string  Assign a name to the container

```sh
docker run -d --name=db redis:alpine
```

在后台启动一个 `container name` 是 `db` 的容器, 并打印容器的 `ID`

> Unable to find image 'redis:alpine' locally
alpine: Pulling from library/redis
63bc94deeb28: Pull complete
828e397560e3: Pull complete
5902d88df6c2: Pull complete
157b2c953c6d: Pull complete
b5212c16b59d: Pull complete
5f8f01031701: Pull complete
Digest: sha256:613ab7e1c0175cae18b69c291512e5e8f1129175f6617ff2126b7ac9a1e5c550
Status: Downloaded newer image for redis:alpine
0a5ceea3df52e200d45772c4507b20d7c95a97dcd3939cb33a8d5bc1b842e334

#### 检验

##### pidof \<process-name>

```sh
pidof redis-server
```

##### ps

```sh
ps aux | grep redis-server
```

##### docker top *container-name*

> docker top can help us identify information about the process including the PID (Process ID) and PPID (Parent Process ID)

```sh
docker top db
```

##### pstree

> pstree will list all of the sub processes

```sh
pstree -c -p -A $(pgrep dockerd)
```

## Process Directory

> Linux is just a series of magic files and contents, this makes it fun to explore and navigate to see what is happening under the covers, and in some cases, change the contents to see the results. The configuration for each process is defined within the /proc directory. If you know the process ID, then you can identify the configuration directory.

每一个 `process` 的配置都定义在 `/proc` 文件夹下. 如果知道 `process ID`, 就能找到该进程的配置文件夹

### redis-server 进程的配置

#### 找到 redis-server 的 pid

```sh
DBPID=$(pgrep redis-server)
echo Redis is $DBPID
ls /proc
```

#### 查看 redis-server 进程的环境变量

```sh
ls /proc/$DBPID
cat /proc/$DBPID/environ
```

结果如下:
> HOSTNAME=f3b9624ce46cSHLVL=2REDIS_DOWNLOAD_SHA=
> 61db74eabf6801f057fd24b590232f2f337d422280fd19486
> eca03be87d3a82bHOME=/home/redisPATH=/usr/local/sb
> in:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
> REDIS_DOWNLOAD_URL=http://download.redis.io/releases/redis-5.0.7.tar.gzREDIS_VERSION=5.0.7PWD=/data

解析查看:

```javascript
'HOSTNAME=5fba1658e4ceSHLVL=2REDIS_DOWNLOAD_SHA=61db74eabf6801f057fd24b590232f2f337d422280fd19486eca03be87d3a82bHOME=/home/redisPATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/binREDIS_DOWNLOAD_URL=http://download.redis.io/releases/redis-5.0.7.tar.gzREDIS_VERSION=5.0.7PWD=/data'.match(/[A-Z]+=[^A-Z]*/g) // regexp 解析查看 key=value
```

#### docker 容器里执行 env 命令

``` sh
docker exec -it db env
```

`env` 内容如下:
> PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
HOSTNAME=f3b9624ce46c
TERM=xterm
REDIS_VERSION=5.0.7
REDIS_DOWNLOAD_URL=http://download.redis.io/releases/redis-5.0.7.tar.gz
REDIS_DOWNLOAD_SHA=61db74eabf6801f057fd24b590232f2f337d422280fd19486eca03be87d3a82b
HOME=/root

## namespace

> One of the fundamental parts of a container is namespaces. The concept of namespaces is to limit what processes can see and access certain parts of the system, such as other network interfaces or processes
> When a container is started, the container runtime, such as Docker, will create new namespaces to sandbox the process. By running a process in it's own Pid namespace, it will look like it's the only process on the system.

- Mount (mnt)
- Process (pid)
- Network (net)
- Interprocess Communication (ipc)
- UTS (hostnames) aka unix time-sharing system
- User ID (user)
- Control group (cgroup)

### lanched "contained" processes

> Without using a runtime such as Docker, a process can still operate within it's own namespace. One tool to help is unshare.

不用 `docker` 这个 `runtime`, 也可以用 `unshare` 这个工具来创建一个拥有自己的 `namespace` 的进程.

```sh
unshare --help
```

> Run a program with some namespaces unshared from the parent.

运行一个程序, 不共享父进程的某些 namespaces

```sh
unshare --fork --pid --mount-proc bash
ps
exit
```

### namespace 是啥

> Namespaces are inode locations on disk. this allows for processes to shared/reused the same namespace, allowing them to view and interact.

`namespace` 是硬盘上的 `inode`. 这允许 `processes` 共享/复用相同的 `namespace`, 允许进程之间可以互相看得到, 互相交流.

#### 查看 redis-server 的所有 namespace

```sh
ls -liha /proc/$DBPID/ns
```

#### attach process to existing NameSpaces

> `nsenter` run a program with namespaces of other processes.

在其他进程的 `namespace` 中运行一个程序

```txt
nsenter --help
nsenter [options] [<program> [<argument>...]]
 Options:
 -a, --all              enter all namespaces
 -t, --target <pid>     target process to get namespaces from
 -m, --mount[=<file>]   enter mount namespace
 -u, --uts[=<file>]     enter UTS namespace (hostname etc)
 -i, --ipc[=<file>]     enter System V IPC namespace
 -n, --net[=<file>]     enter network namespace
 -p, --pid[=<file>]     enter pid namespace
 -C, --cgroup[=<file>]  enter cgroup namespace
 -U, --user[=<file>]    enter user namespace
 -S, --setuid <uid>     set uid in entered namespace
 -G, --setgid <gid>     set gid in entered namespace
     --preserve-credentials do not touch uids or gids
 -r, --root[=<dir>]     set the root directory
 -w, --wd[=<dir>]       set the working directory
 -F, --no-fork          do not fork before exec'ing <program>
 -Z, --follow-context   set SELinux context according to --target PID
```

 ```sh
nsenter --target $DBPID --mount --uts --ipc --net --pid ps aux
 ```

在 `redis-server` 的 `mount`, `uts`, `ipc`, `net`, `pid` 这些命名空间里执行 `ps` 命令

#### share namespaces of redis-server

> With Docker, these namespaces can be shared using the syntax container:\<container-name>. For example, the command below will connect nginx to the DB namespace

```sh
docker run -d --name=web --net=container:db nginx:alpine
```

启动 `nginx` 的容器. 共享 `redis-server` 的 `net` namespace

- 查看 `nginx` 容器的 `net` 的 namespace

```sh
 ls -lha /proc/$WEBPID/ns/ | grep net
```

> dr-x--x--x 2 systemd-network systemd-journal 0 Dec 24 13:40 .
dr-xr-xr-x 9 systemd-network systemd-journal 0 Dec 24 13:40 ..
lrwxrwxrwx 1 systemd-network systemd-journal 0 Dec 24 13:40 cgroup -> cgroup:[4026531835]
lrwxrwxrwx 1 systemd-network systemd-journal 0 Dec 24 13:40 ipc -> ipc:[4026532225]
lrwxrwxrwx 1 systemd-network systemd-journal 0 Dec 24 13:40 mnt -> mnt:[4026532223]
lrwxrwxrwx 1 systemd-network systemd-journal 0 Dec 24 13:40 net -> net:[4026532160] -- 注意这个是共享的 redis-server 的 net 命名空间
lrwxrwxrwx 1 systemd-network systemd-journal 0 Dec 24 13:40 pid -> pid:[4026532226]
lrwxrwxrwx 1 systemd-network systemd-journal 0 Dec 24 13:40 user -> user:[4026531837]
lrwxrwxrwx 1 systemd-network systemd-journal 0 Dec 24 13:40 uts -> uts:[4026532224]

- 查看 `redis-server` 容器的 `net` 命名空间

```sh
ls -lha /proc/$DBPID/ns/ | grep net
```

> lrwxrwxrwx 1 999 packer 0 Dec 24 13:39 net -> net:[4026532160]

## CGroups

> CGroups limit the amount of resources a process can consume. These cgroups are values defined in particular files within the /proc directory.
CGroups 用来限制一个进程可以使用的资源的量. cgroups 的值被定义在 /proc 文件夹下的特定文件里.

### 查看 redis-server 的 CGroups

```sh
cat /proc/$DBPID/cgroup
```

- 得到 redis-server 的容器 ID

```sh
DBID=$(docker ps --no-trunc | grep 'db' | awk '{print $1}')
```

- 查看 redis-server 的 cpu stats 和 usages

```sh
cat /sys/fs/cgroup/cpu,cpuacct/docker/$DBID/cpuacct.stat
```

- 查看 redis-server 的 cpu shares

```sh
cat /sys/fs/cgroup/cpu,cpuacct/docker/$DBID/cpu.shares
```

- 查看所有容器的 memory 的 docker cgroups 配置

```sh
ls /sys/fs/cgroup/memory/docker
```

### 查看所有的 CGroups

```sh
 ls /sys/fs/cgroup
```

### 配置 cgroups

> 用 cgroups 配置 内存, memory quotes 存放在 memory.limit_in_bytes
这个文件

- 容器默认没有内存限制

```sh
docker stats db --no-strem ; 只查看一条信息 
```

- 限制 db 这个容器的内存使用

```sh
echo 8000000 > /sys/fs/cgroup/memory/docker/$DBID/memory.limit_in_bytes
```

## Seccomp 限制可以使用的 syscall

> When assigned to a process it means the process will be limited to a subset of the ability system calls. If it attempts to call a blocked system call is will recieve the error "Operation Not Allowed". [容器默认可以使用的 syscall](https://github.com/moby/moby/blob/a575b0b1384b2ba89b79cbd7e770fbeb616758b3/profiles/seccomp/default.json)

```sh
cat /proc/$DBPID/status
cat /proc/$DBPID/status | grep Seccomp
```

> The flag meaning are: 0: disabled 1: strict 2: filtering

## Capabilities

> Capabilities are groupings about what a process or user has permission to do. These Capabilities might cover multiple system calls or actions, such as changing the system time or hostname.

- 查看 redis-server 的 Capabilities

> The status file also containers the Capabilities flag. A process can drop as many Capabilities as possible to ensure it's secure.

```sh
cat /proc/$DBPID/status | grep ^Cap
```

结果如下:
>CapInh: 00000000a80425fb
CapPrm: 0000000000000000
CapEff: 0000000000000000
CapBnd: 00000000a80425fb
CapAmb: 0000000000000000

- 解析 redis-server 的 flags

> The flags are stored as a bitmask that can be decoded with capsh

```sh
capsh --decode=00000000a80425fb
```

解析结果如下:
> 0x00000000a80425fb=cap_chown,cap_dac_override,cap_fowner,
> cap_fsetid,cap_kill,cap_setgid,cap_setuid,cap_setpcap,
> cap_net_bind_service,cap_net_raw,cap_sys_chroot,cap_mknod,
> cap_audit_write,cap_setfcap

## Reference

1. [what is a container](https://www.katacoda.com/courses/container-runtimes/what-is-a-container)
2. [awk](http://www.ruanyifeng.com/blog/2018/11/awk.html)