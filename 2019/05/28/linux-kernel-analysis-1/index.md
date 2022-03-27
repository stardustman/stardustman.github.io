
# analysis01

## 设计理念

机制与策略分离
机制 -- 提供什么样的功能
策略 -- 如何使用这些功能
> 说实在的这句话第一次听到还是挺震撼的, 一时觉得大学里的操作系统都不知道在干嘛, 我们学的就是机制, 比如进程创建功能, 进程创建完成后具体如何使用与OS内核不再有关. 文件创建功能, 文件创建完,如何使用交给用户. 其实很类似编程中的接口, 接口定义功能, 实现负责具体的策略.

## 内核类型

1. 单内核 (Linux)

> 可加载的 Linux 内核模块, 提高维护性.

2. 微内核

## Linux Kernel

### 双链表

#### 结构

```c
struct list_head{
    struct list_head *next , *prev
};
```

> 这个结构很有意思, 改变两个指针的含义, 会发现几乎描述了很多数据结构.
> 只有一个 *next, 单链表, 只进行头部插入数据-栈, 只进行尾部插入数据-队列
>*left, *right

#### 初始化

```c
define LIST_HEAD_INIT(name){&name, &name}
```

> 前后指针都指向自己

#### 声明并初始化

```c
define LIST_HEAD(name) struct list_head(name){LIST_HEAD_INIT(name)}
```

#### 判空

```c
static inline int list_empty(const struct list_head*head){
    return head->next == head;
}
```

#### 添加节点

```c
static inline void _ _list_add(struct list_head*new,struct list_head*prev,struct_head*next){
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}
```

> static 该函数作用域仅在本文件内, 可以隐藏信息
> inline 编译器展开该函数

```c
static inline void _ _list_add_tail(struct list_head*new,struct list_head*head){
    _ _list_add(new,head->prev,head);
}
```
