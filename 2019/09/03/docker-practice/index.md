
> Centos7 Linux
> 运行的 Docker 容器是: mssql-node-docker-demo-app 

## docker info

![docker-info](https://github.com/stardustman/pictures/raw/main/img/docker-info.png) #(docker-info)

## docker inspect

```json
[
    {
        "Id": "405a378d6f88f903dda5972dbfb8f037efff22296c3aaf4c50d44ebd68ad8655",
        "Created": "2019-08-01T02:57:14.067532309Z",
        "Path": "/bin/sh",
        "Args": [
            "-c",
            "/bin/bash ./entrypoint.sh"
        ],
        "State": {
            "Status": "running",
            "Running": true,
            "Paused": false,
            "Restarting": false,
            "OOMKilled": false,
            "Dead": false,
            "Pid": 12164,
            "ExitCode": 0,
            "Error": "",
            "StartedAt": "2019-08-22T06:08:43.334320939Z",
            "FinishedAt": "2019-08-21T09:25:32.130119566Z"
        },
        "Image": "sha256:e600f86aa4e2aced43a193a28aa507651bfb77daa09ca8dbf286451a630cf27e", // node-web-app 的 docker 镜像 id
        "ResolvConfPath": "/var/lib/docker/containers/405a378d6f88f903dda5972dbfb8f037efff22296c3aaf4c50d44ebd68ad8655/resolv.conf",
        "HostnamePath": "/var/lib/docker/containers/405a378d6f88f903dda5972dbfb8f037efff22296c3aaf4c50d44ebd68ad8655/hostname",
        "HostsPath": "/var/lib/docker/containers/405a378d6f88f903dda5972dbfb8f037efff22296c3aaf4c50d44ebd68ad8655/hosts",
        "LogPath": "",
        "Name": "/friendly_hamilton", // docker container 的名称
        "RestartCount": 0,
        "Driver": "overlay2", // storage driver
        "MountLabel": "",
        "ProcessLabel": "",
        "AppArmorProfile": "",
        "ExecIDs": null,
        "HostConfig": {
            "Binds": null,
            "ContainerIDFile": "",
            "LogConfig": {
                "Type": "journald",
                "Config": {}
            },
            "NetworkMode": "default",
            "PortBindings": {
                "1433/tcp": [
                    {
                        "HostIp": "",
                        "HostPort": "1433"
                    }
                ],
                "8080/tcp": [
                    {
                        "HostIp": "",
                        "HostPort": "8080"
                    }
                ]
            },
            "RestartPolicy": {
                "Name": "no",
                "MaximumRetryCount": 0
            },
            "AutoRemove": false,
            "VolumeDriver": "",
            "VolumesFrom": null,
            "CapAdd": null,
            "CapDrop": null,
            "Dns": [],
            "DnsOptions": [],
            "DnsSearch": [],
            "ExtraHosts": null,
            "GroupAdd": null,
            "IpcMode": "",
            "Cgroup": "",
            "Links": null,
            "OomScoreAdj": 0,
            "PidMode": "",
            "Privileged": false,
            "PublishAllPorts": false,
            "ReadonlyRootfs": false,
            "SecurityOpt": null,
            "UTSMode": "",
            "UsernsMode": "",
            "ShmSize": 67108864,
            "Runtime": "docker-runc",
            "ConsoleSize": [
                0,
                0
            ],
            "Isolation": "",
            "CpuShares": 0,
            "Memory": 0,
            "NanoCpus": 0,
            "CgroupParent": "",
            "BlkioWeight": 0,
            "BlkioWeightDevice": null,
            "BlkioDeviceReadBps": null,
            "BlkioDeviceWriteBps": null,
            "BlkioDeviceReadIOps": null,
            "BlkioDeviceWriteIOps": null,
            "CpuPeriod": 0,
            "CpuQuota": 0,
            "CpuRealtimePeriod": 0,
            "CpuRealtimeRuntime": 0,
            "CpusetCpus": "",
            "CpusetMems": "",
            "Devices": [],
            "DiskQuota": 0,
            "KernelMemory": 0,
            "MemoryReservation": 0,
            "MemorySwap": 0,
            "MemorySwappiness": -1,
            "OomKillDisable": false,
            "PidsLimit": 0,
            "Ulimits": null,
            "CpuCount": 0,
            "CpuPercent": 0,
            "IOMaximumIOps": 0,
            "IOMaximumBandwidth": 0
        },
        "GraphDriver": {
            "Name": "overlay2",
            "Data": {
                "LowerDir": "/var/lib/docker/overlay2/efead8cb8732e4ccfc24485c253d02838842d6dfd9a2ee60a9142c69cb54f4a2-init/diff:/var/lib/docker/overlay2/83b7194d94d42c0ef8f78a2f4105cc7a7a215a8e885b695a933a8f8212db9cfa/diff:/var/lib/docker/overlay2/9549118d3883d39414fadf0dc17e654d1408878be3296b294711ee65ef336b98/diff:/var/lib/docker/overlay2/8e85764472e79d42f469dd3681883cd415e85098d6c42fb3dd239471212f87b3/diff:/var/lib/docker/overlay2/8dc8ec519bade76b07994adc690d1e6fcad408147835198a250b4c66c489a151/diff:/var/lib/docker/overlay2/b07d1c7463b2a4ff25f7800729381fdf5b036c12a065973b9d079580c3a50a5e/diff:/var/lib/docker/overlay2/60c9257cc9d8a7ac4b66d1c4cae5ff994471976f0a344f8ac1856a4fba7b290e/diff:/var/lib/docker/overlay2/2a00c5b47427e151e000ffe05de8121e956dbf15b0d356cfc9ad2754dc5554d5/diff:/var/lib/docker/overlay2/3495bcc2d508cbdad81b1f3eb49645fe0eb05caf0e99fbb6310d40fbdd8118a2/diff:/var/lib/docker/overlay2/9df322481c6f8b91b3583016634664e6ad991b8b846a9585385227a02b898770/diff:/var/lib/docker/overlay2/163747850da2115bac28b39ac6d2e08ca5c0e409fe6cae624155f8e61489aacd/diff:/var/lib/docker/overlay2/52405978404b67e0a67ba95cb12682d0e8a8ae781fd2b21dc2ab94197e97c38b/diff:/var/lib/docker/overlay2/c987c31253bca839eb789feb3e4fe7bc247b1e88dc88c67e2b4209fc26025fe8/diff:/var/lib/docker/overlay2/ed663d4b2fffda783f9b69e5eb7c139f34142e0e2d868460a3c1e1d925ec3e7e/diff:/var/lib/docker/overlay2/a0e3d07a65b7ee70d3869fa41418ecf71056b9ac0ba1182170621c83c77bba94/diff:/var/lib/docker/overlay2/aa4c66b37aae4e55a94aa60ac763a60851af34271ccb1a80f547e3d90e7ac2bf/diff:/var/lib/docker/overlay2/527b1a014d302c1fe835d778be6cd008de6741d290fbc0f2e64e3f25f7a7b8d0/diff",
                "MergedDir": "/var/lib/docker/overlay2/efead8cb8732e4ccfc24485c253d02838842d6dfd9a2ee60a9142c69cb54f4a2/merged",
                "UpperDir": "/var/lib/docker/overlay2/efead8cb8732e4ccfc24485c253d02838842d6dfd9a2ee60a9142c69cb54f4a2/diff",
                "WorkDir": "/var/lib/docker/overlay2/efead8cb8732e4ccfc24485c253d02838842d6dfd9a2ee60a9142c69cb54f4a2/work"
            }
        },
        "Mounts": [
            {
                "Type": "volume",
                "Name": "7bc1a7ad23925077cf8463c2d7a21b0cafc7d91305d89504f85fa76c33138834",
                "Source": "/var/lib/docker/volumes/7bc1a7ad23925077cf8463c2d7a21b0cafc7d91305d89504f85fa76c33138834/_data", // host 主机的被挂在的目录
                "Destination": "/var/opt/mssql", // Volume 挂载点
                "Driver": "local",
                "Mode": "",
                "RW": true,
                "Propagation": ""
            }
        ],
        "Config": {
            "Hostname": "405a378d6f88",
            "Domainname": "",
            "User": "",
            "AttachStdin": false,
            "AttachStdout": false,
            "AttachStderr": false,
            "ExposedPorts": {
                "1433/tcp": {},
                "8080/tcp": {}
            },
            "Tty": false,
            "OpenStdin": false,
            "StdinOnce": false,
            "Env": [
                "ACCEPT_EULA=Y",
                "SA_PASSWORD=Yukon900",
                "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
            ],
            "Cmd": [
                "/bin/sh",
                "-c",
                "/bin/bash ./entrypoint.sh"
            ],
            "ArgsEscaped": true,
            "Image": "node-web-app",
            "Volumes": {
                "/var/opt/mssql": {}
            },
            "WorkingDir": "/usr/src/app",
            "Entrypoint": null,
            "OnBuild": null,
            "Labels": {
                "com.microsoft.product": "Microsoft SQL Server",
                "com.microsoft.version": "14.0.3048.4",
                "vendor": "Microsoft"
            }
        },
        "NetworkSettings": {
            "Bridge": "",
            "SandboxID": "d21ee4834e9078042da65677d8db9717748eb7f86a6954c5e4eee29636159aa7",
            "HairpinMode": false,
            "LinkLocalIPv6Address": "",
            "LinkLocalIPv6PrefixLen": 0,
            "Ports": {
                "1433/tcp": [
                    {
                        "HostIp": "0.0.0.0",
                        "HostPort": "1433"
                    }
                ],
                "8080/tcp": [
                    {
                        "HostIp": "0.0.0.0",
                        "HostPort": "8080"
                    }
                ]
            },
            "SandboxKey": "/var/run/docker/netns/d21ee4834e90",
            "SecondaryIPAddresses": null,
            "SecondaryIPv6Addresses": null,
            "EndpointID": "46dac1ff81a297ede919100e0d06a743b845e34b802b54bca8cb8f02dcd33c85",
            "Gateway": "172.17.0.1",
            "GlobalIPv6Address": "",
            "GlobalIPv6PrefixLen": 0,
            "IPAddress": "172.17.0.2",
            "IPPrefixLen": 16,
            "IPv6Gateway": "",
            "MacAddress": "02:42:ac:11:00:02",
            "Networks": {
                "bridge": {
                    "IPAMConfig": null,
                    "Links": null,
                    "Aliases": null,
                    "NetworkID": "a8af0599996912141c3f2ce4b0a2bcc48eea9164a35672aac6307333450acb6b", //a8af05999969 
                    "EndpointID": "46dac1ff81a297ede919100e0d06a743b845e34b802b54bca8cb8f02dcd33c85",
                    "Gateway": "172.17.0.1",
                    "IPAddress": "172.17.0.2",
                    "IPPrefixLen": 16,
                    "IPv6Gateway": "",
                    "GlobalIPv6Address": "",
                    "GlobalIPv6PrefixLen": 0,
                    "MacAddress": "02:42:ac:11:00:02" 
                }
            }
        }
    }
]

```


## docker volume

### mount

```json
   "Mounts": [
            {
                "Type": "volume",
                "Name": "7bc1a7ad23925077cf8463c2d7a21b0cafc7d91305d89504f85fa76c33138834",
                "Source": "/var/lib/docker/volumes/7bc1a7ad23925077cf8463c2d7a21b0cafc7d91305d89504f85fa76c33138834/_data", // host 主机的被挂载的目录
                "Destination": "/var/opt/mssql", // Volume 挂载点
                "Driver": "local",
                "Mode": "",
                "RW": true,
                "Propagation": ""
            }
        ]
```

## Local Docker container 网络

```json
{
"ResolvConfPath": "/var/lib/docker/containers/405a378d6f88f903dda5972dbfb8f037efff22296c3aaf4c50d44ebd68ad8655/resolv.conf",
"HostnamePath": "/var/lib/docker/containers/405a378d6f88f903dda5972dbfb8f037efff22296c3aaf4c50d44ebd68ad8655/hostname",
"HostsPath": "/var/lib/docker/containers/405a378d6f88f903dda5972dbfb8f037efff22296c3aaf4c50d44ebd68ad8655/hosts",
}
```

![local-var-lib-docker-container](https://github.com/stardustman/pictures/raw/main/img/local-var-lib-docker-container.png) #(本地存储的容器的网络配置)

## Container 网络配置

![container-etc](https://github.com/stardustman/pictures/raw/main/img/container-etc.png) #(容器的网络配置)

## Docker Images 分层

```json
{
"GraphDriver": {
            "Name": "overlay2",
            "Data": {
                "LowerDir": "/var/lib/docker/overlay2/efead8cb8732e4ccfc24485c253d02838842d6dfd9a2ee60a9142c69cb54f4a2-init/diff:/var/lib/docker/overlay2/83b7194d94d42c0ef8f78a2f4105cc7a7a215a8e885b695a933a8f8212db9cfa/diff:/var/lib/docker/overlay2/9549118d3883d39414fadf0dc17e654d1408878be3296b294711ee65ef336b98/diff:/var/lib/docker/overlay2/8e85764472e79d42f469dd3681883cd415e85098d6c42fb3dd239471212f87b3/diff:/var/lib/docker/overlay2/8dc8ec519bade76b07994adc690d1e6fcad408147835198a250b4c66c489a151/diff:/var/lib/docker/overlay2/b07d1c7463b2a4ff25f7800729381fdf5b036c12a065973b9d079580c3a50a5e/diff:/var/lib/docker/overlay2/60c9257cc9d8a7ac4b66d1c4cae5ff994471976f0a344f8ac1856a4fba7b290e/diff:/var/lib/docker/overlay2/2a00c5b47427e151e000ffe05de8121e956dbf15b0d356cfc9ad2754dc5554d5/diff:/var/lib/docker/overlay2/3495bcc2d508cbdad81b1f3eb49645fe0eb05caf0e99fbb6310d40fbdd8118a2/diff:/var/lib/docker/overlay2/9df322481c6f8b91b3583016634664e6ad991b8b846a9585385227a02b898770/diff:/var/lib/docker/overlay2/163747850da2115bac28b39ac6d2e08ca5c0e409fe6cae624155f8e61489aacd/diff:/var/lib/docker/overlay2/52405978404b67e0a67ba95cb12682d0e8a8ae781fd2b21dc2ab94197e97c38b/diff:/var/lib/docker/overlay2/c987c31253bca839eb789feb3e4fe7bc247b1e88dc88c67e2b4209fc26025fe8/diff:/var/lib/docker/overlay2/ed663d4b2fffda783f9b69e5eb7c139f34142e0e2d868460a3c1e1d925ec3e7e/diff:/var/lib/docker/overlay2/a0e3d07a65b7ee70d3869fa41418ecf71056b9ac0ba1182170621c83c77bba94/diff:/var/lib/docker/overlay2/aa4c66b37aae4e55a94aa60ac763a60851af34271ccb1a80f547e3d90e7ac2bf/diff:/var/lib/docker/overlay2/527b1a014d302c1fe835d778be6cd008de6741d290fbc0f2e64e3f25f7a7b8d0/diff",
                "MergedDir": "/var/lib/docker/overlay2/efead8cb8732e4ccfc24485c253d02838842d6dfd9a2ee60a9142c69cb54f4a2/merged",
                "UpperDir": "/var/lib/docker/overlay2/efead8cb8732e4ccfc24485c253d02838842d6dfd9a2ee60a9142c69cb54f4a2/diff",
                "WorkDir": "/var/lib/docker/overlay2/efead8cb8732e4ccfc24485c253d02838842d6dfd9a2ee60a9142c69cb54f4a2/work"
            }
        }
}
```

### MergedDir

> 这个 MergedDir 层就是一个 Linux 镜像

![merged-dir](https://github.com/stardustman/pictures/raw/main/img/merged-dir.png) #( 镜像 Merged 层)

## Reference

1. [mssql-node-docker-demo-app](https://github.com/twright-msft/mssql-node-docker-demo-app/blob/master/Dockerfile)

2. [storage driver](https://docs.docker.com/storage/storagedriver/)
3. [var-run-docker-sock](https://medium.com/better-programming/about-var-run-docker-sock-3bfd276e12fd)
4. [docker 的 /var/run/docker.sock 参数](https://blog.csdn.net/boling_cavalry/article/details/92846483)