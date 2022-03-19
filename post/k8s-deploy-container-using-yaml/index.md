
kubernetes 作为一个云上的操作系统，要想充分利用，就要了解 kubernetes 提供的功能。告诉系统要搞啥，k8s 帮你搞定这一切，归根结底，不开发 k8s 就是一个 k8s 的用户，知道 k8s 能具体做啥，写好 YAML 就可以。

## 如何在 k8s 上部署服务

### create Deployment

> One of the most common `Kubernetes object` is the `deployment object`. 
> The deployment object defines the `container spec required`, along with the `name` and `labels` used by other parts of Kubernetes to discover and connect to the application.

#### 准备 deployment.yaml

> The definition defines how to launch an application called webapp1 using the Docker Image katacoda/docker-http-server that runs on Port 80.

```yaml
apiVersion: apps/v1
kind: Deployment # object type
metadata:
  name: webapp1
spec:
  replicas: 1
  selector:
    matchLabels:
      app: webapp1
  template:
    metadata:
      labels:
        app: webapp1
    spec:
      containers:
      - name: webapp1
        image: katacoda/docker-http-server:latest
        ports:
        - containerPort: 80
```

#### 部署到 k8s 中

```bash
kubectl create -f `deployment.yaml`
```

#### 查看所有 deployment

```bash
kubectl get deployment
```

#### 查看某一个 deployment

```bash
kubectl describe deployment `webapp1`
```

### create Service

> kubernetes has powerful networking capabilities that control how applications communicate. 
> These networking configurations can also be controlled via YAML.

#### 准备 service.yaml

> The Service `selects` all applications with the label webapp1. 
> As multiple replicas, or instances, are deployed, 
> they will be `automatically load balanced` based on this common label. 
> The Service makes the application available via a `NodePort`.

```yaml
apiVersion: v1
kind: Service
metadata:
  name: webapp1-svc
  labels:
    app: webapp1
spec:
  type: NodePort
  ports:
  - port: 80
    nodePort: 30080
  selector:
    app: webapp1
```

#### 部署到 k8s 中

```bash
kubectl create -f `service.yaml`
```

#### 查看所有 service

```bash
kubectl get svc
```

#### 查看某一个 deployment

```bash
kubectl describe svc `webapp1-svc`
```

### scale `Deployment`

#### 配置 replicas

```yaml
apiVersion: apps/v1
kind: Deployment # object type
metadata:
  name: webapp1
spec:
  replicas: 4 # 
  selector:
    matchLabels:
      app: webapp1
  template:
    metadata:
      labels:
        app: webapp1
    spec:
      containers:
      - name: webapp1
        image: katacoda/docker-http-server:latest
        ports:
        - containerPort: 80
```

## References

1. [https://www.katacoda.com/courses/kubernetes/kubectl-run-containers](https://www.katacoda.com/courses/kubernetes/kubectl-run-containers)
2. [apiversion](https://matthewpalmer.net/kubernetes-app-developer/articles/kubernetes-apiversion-definition-guide.html)
3. [what is k8s deployment?](https://www.redhat.com/en/topics/containers/what-is-kubernetes-deployment)