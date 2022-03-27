
## container image

> A container image is a tar file containing tar files. Each of the tar file is a layer. Once all tar files have been extract into the same location then you have the container's filesystem.
`container image` 是一个 `tar` 文件. 每一个 `tar` 文件就是一个 layer

### docker pull 拉取一个镜像

```sh
docker pull redis:3.2.11-alpine
```

> 3.2.11-alpine: Pulling from library/redis
ff3a5c916c92: Pull complete
aae70a2e6027: Pull complete
87c655da471c: Pull complete
bc3141806bdc: Pull complete
53616fb426d9: Pull complete
9791c5883c6a: Pull complete
Digest: sha256:ebf1948b84dcaaa0f8a2849cce6f2548edb8862e2829e3e7d9e4cd5a324fb3b7
Status: Downloaded newer image for redis:3.2.11-alpine

> 可以看到 redis:3.2.11-alpine 这个 image 拉取了 *ff3a5c916c92*,
*aae70a2e6027*, *87c655da471c*, *bc3141806bdc*, *53616fb426d9*, *9791c5883c6a* 共 6 次.

### docker save 验证 image 分层

```sh
docker save --help
Usage:  docker save [OPTIONS] IMAGE [IMAGE...]
Save one or more images to a tar archive (streamed to STDOUT by default)
Options:
  -o, --output string   Write to a file, instead of STDOUT
```

#### 验证 redis:3.2.11-alpine 分层

##### docker save 保存 image 为 tar 文件

```sh
docker  save  redis:3.2.11-alpine > redis.tar
```

##### 解压 tar 文件

```sh
tar -xvf redis.tar
```

```sh
tree
.
├── 46a2fed8167f5d523f9a9c07f17a7cd151412fed437272b517ee4e46587e5557
│   ├── json // 这一 layer 的配置文件
│   ├── layer.tar
│   └── VERSION
├── 498654318d0999ce36c7b90901ed8bd8cb63d86837cb101ea1ec9bb092f44e59
│   ├── json // 这一 layer 的配置文件
│   ├── layer.tar
│   └── VERSION
├── ad01e7adb4e23f63a0a1a1d258c165d852768fb2e4cc2d9d5e71698e9672093c
│   ├── json // 这一 layer 的配置文件
│   ├── layer.tar
│   └── VERSION
├── ca0b6709748d024a67c502558ea88dc8a1f8a858d380f5ddafa1504126a3b018.json // 这个 `image` 的配置文件
├── da2a73e79c2ccb87834d7ce3e43d274a750177fe6527ea3f8492d08d3bb0123c
│   ├── json // 这一 layer 的配置文件
│   ├── layer.tar
│   └── VERSION
├── db1a23fc1daa8135a1c6c695f7b416a0ac0eb1d8ca873928385a3edaba6ac9a3
│   ├── json // 这一 layer 的配置文件
│   ├── layer.tar
│   └── VERSION
├── f07352aa34c241692cae1ce60ade187857d0bffa3a31390867038d46b1e7739c
│   ├── json // 这一 layer 的配置文件
│   ├── layer.tar
│   └── VERSION
├── manifest.json
├── redis.tar // docker save 的 tar 文件
└── repositories
```

##### 查看解压后的 tar 文件

> ls
> 46a2fed8167f5d523f9a9c07f17a7cd151412fed437272b517ee4e46587e5557
> 498654318d0999ce36c7b90901ed8bd8cb63d86837cb101ea1ec9bb092f44e59
> ad01e7adb4e23f63a0a1a1d258c165d852768fb2e4cc2d9d5e71698e9672093c
> ca0b6709748d024a67c502558ea88dc8a1f8a858d380f5ddafa1504126a3b018.json
> da2a73e79c2ccb87834d7ce3e43d274a750177fe6527ea3f8492d08d3bb0123c
> db1a23fc1daa8135a1c6c695f7b416a0ac0eb1d8ca873928385a3edaba6ac9a3
> f07352aa34c241692cae1ce60ade187857d0bffa3a31390867038d46b1e7739c
> manifest.json
> repositories

##### 查看 manifest.json

```json
[
    {
        "Config":"ca0b6709748d024a67c502558ea88dc8a1f8a858d380f5ddafa1504126a3b018.json", // 整个 image 的配置文件
        "RepoTags":[
            "redis:3.2.11-alpine" // image 的 tag
        ],
        "Layers":[
            "498654318d0999ce36c7b90901ed8bd8cb63d86837cb101ea1ec9bb092f44e59/layer.tar", // 1
            "ad01e7adb4e23f63a0a1a1d258c165d852768fb2e4cc2d9d5e71698e9672093c/layer.tar", // 2
            "da2a73e79c2ccb87834d7ce3e43d274a750177fe6527ea3f8492d08d3bb0123c/layer.tar", // 3
            "db1a23fc1daa8135a1c6c695f7b416a0ac0eb1d8ca873928385a3edaba6ac9a3/layer.tar", // 4
            "f07352aa34c241692cae1ce60ade187857d0bffa3a31390867038d46b1e7739c/layer.tar", // 5
            "46a2fed8167f5d523f9a9c07f17a7cd151412fed437272b517ee4e46587e5557/layer.tar"  // 6
        ]
    }
]
```

> 由上可知, `redis:3.2.11-alpine` 这个 `image` 有 6 个 `*layer.tar` 组成. 每一个 layer.tar 对应 image 一层.

##### 查看 repositories

```sh
cat repositories
```

结果如下:

```json
{"redis":{"3.2.11-alpine":"46a2fed8167f5d523f9a9c07f17a7cd151412fed437272b517ee4e46587e5557"}}
```

### 创建一个空的 image

> build an image from scratch

```sh
docker import --help
```

>Usage:  docker import [OPTIONS] file|URL|- [REPOSITORY[:TAG]]
Import the contents from a tarball to create a filesystem image
Options:
  -c, --change list      Apply Dockerfile instruction to the created image
  -m, --message string   Set commit message for imported image

1. 使用 docker import 生成一个空镜像

```sh
tar cv --files-from /dev/null | docker import - empty
```

生成的 **empty** 空镜像 ID 如下:
sha256:8880ab5b7953a69e0a9252cbeeb537f0667ec10d3f849289117f8434802c0c49

2. 查看生成的镜像

```sh
docker images
```

### Creating Image without Dockerfile

1. 得到 `BusyBox rootfs`

```sh
curl -LO https://raw.githubusercontent.com/moby/moby/a575b0b1384b2ba89b79cbd7e770fbeb616758b3/contrib/mkimage/busybox-static && chmod +x busybox-static
```

2. 执行 ./busybox-static busybox

3. ls -lha busybox

4. 向 busybox 添加一个版本信息

```sh
echo KatacodaPrivateBuild > busybox/release
```

5. 用 busybox 这个文件夹生成一个 image

>the directory can be converted into a tar and automatically imported into Docker as an image.

```sh
tar -C busybox -c . | docker import - busybox
```

6. 启动一个容器并查看 release 信息

```sh
docker run busybox cat /release
```

### busy-static shell 脚本分析

```sh
#!/usr/bin/env bash
set -e #  Exit immediately if a command exits with a non-zero status.

rootfsDir="$1" # $1 是第一个参数
shift # 相当于 shift 1, 也就是把第一个参数丢掉, 后面的都前进一步

busybox="$(which busybox 2>/dev/null || true)" # 拿到 busybox 的路径
if [ -z "$busybox" ]; then
 echo >&2 'error: busybox: not found'
 echo >&2 '  install it with your distribution "busybox-static" package'
 exit 1
fi
if ! ldd "$busybox" 2>&1 | grep -q 'not a dynamic executable'; then
 echo >&2 "error: '$busybox' appears to be a dynamic executable"
 echo >&2 '  you should install your distribution "busybox-static" package instead'
 exit 1
fi

mkdir -p "$rootfsDir/bin" # 创建 $rootfsDir/bin
rm -f "$rootfsDir/bin/busybox" # just in case
cp "$busybox" "$rootfsDir/bin/busybox"

(
 cd "$rootfsDir"

 IFS=$'\n'
 modules=( $(bin/busybox --list-modules) ) # 得到 busybox 所有的模块
 unset IFS

 for module in "${modules[@]}"; do
  mkdir -p "$(dirname "$module")"
        # ln target linkname
  ln -sf /bin/busybox "$module" # 强制创建符号链接
 done
)

```

## References

1. [busybox-static](https://raw.githubusercontent.com/moby/moby/a575b0b1384b2ba89b79cbd7e770fbeb616758b3/contrib/mkimage/busybox-static)
2. [docker-intro](http://jm.taobao.org/2016/05/12/introduction-to-docker/)
