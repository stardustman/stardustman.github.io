
## 为啥需要 lock？

1. 多处理器的存在
2. 中断的存在

## lock 是啥？

lock 本质上是一个变量。变量本质上是一块内存。归根结底，lock 就是一块内存，用这块内存来保证线程的原子性。锁有两种状态：

1. 被占用
2. 空闲

## critical section

```c
    balance = balance + 1;
```

在多线程环境下，这段代码执行有问题。如何保证任意时刻，只有一个线程在执行这段代码呢？用 lock，获取这个锁之后，才可以执行这段代码，这段代码执行完毕后，释放这个锁。没有获取锁的线程，无法执行这段代码。也可以认为通过 lock 来`监视`这段代码任意时刻只能有一个线程执行。不错，monitor 本质上也是一个锁。

## 如何评估估锁

### mutual exclusion

   锁，提供互斥功能。

### fairless
  
是否会有线程饥饿

### performance

## 构建 Lock

### 控制中断

```c
void lock(){
    disableInterrupts(); // 关闭中断
}

void unlock(){
    enableInterrputs(); // 开启中断
}
```

> 对于单处理器，可以直接来关闭中断、开启中断的方式来实现线程的原子性。
> 关闭、开启中断的指令是特权指令。

#### 优点

实现简单、便于理解。在关闭中断的情况下，可以保证线程的执行不会被打断。

#### 缺点

1. 需要特权指令。可能被滥用。若一个程序关闭中断后，执行死循环，那么就霸占了 CPU，想要解决这个问题，就只好重启了。
2. 在多核的处理器上，关闭中断没有用。因为其他线程可以在其他处理器上运行，也可以进入临界区。
3. 可能会丢失其他的中断信号。一直在处理这个中断，没空搭理其他的中断请求。
4. 低效。处理中断还是很麻烦的。

### 失败的尝试，只用 Loads/Stores

```c
typedef struct __lock_t{
    int flag; // lock 状态
} lock_t;

void init(lock_t *mutex){
    // 0 -> lock is available
    // 1 -> held
    mutex->flag = 0;
}

void lock(lock_t *mutex){
    while(mutex->flag == 1){ // 检测锁是否被占用(L1)
        ; // spin-wait(do noting)(L2)
    }
    mutex->flag = 1; // 获取锁(L3)
}

void unlock(lock_t *mutex){
    mutex->flag = 0; // 释放锁(U1)
}

```

#### Loads/Stores 分析

![简单的 flag, Loads/Store](https://github.com/stardustman/pictures/raw/main/img/sync/ostep-lock-1.svg)


1. 上图可见，由于中断的存在，在1.检测锁是否被占用 2.获取锁 这两个步骤之间，可能被打断，这样两个线程都获取了这个锁。`这不符合锁的基本功能，没有提供互斥`。无法保证临界区，任意时刻只有一个线程在执行。
2. 如果一个线程 T1 已经获取了锁，另外一个线程 T2 在获取锁时，会在 L1 处不停的判断锁的状态，这就是在 L1 处 Spin，这就是所谓的 Spin Lock。

### test-and-set(atomic exchange) 构建锁

因为只是简单的 Loads/Stores(Reads/Writes) 不能构建出 Lock。所以引入和支持构建 Lock 的 test-and-set 指令。在 x86 下，指令是 [xchg](https://www.felixcloutier.com/x86/xchg)。Exchanges the contents of the destination (first) and source (second) operands. The operands can be `two general-purpose registers` or a `register and a memory location`. If a memory operand is referenced, the processor’s locking protocol is automatically implemented for the duration of the exchange operation, `regardless of the presence or absence of the LOCK prefix or of the value of the IOPL`. 用 `xchg` 原子交换来实现。

#### C 语言表示 test-and-set 语义

```c
int TestAndSet(int *old_ptr, int new) {
    int old = *old_ptr; // fetch old value at old_ptr, Read
    *old_ptr = new; // store ’new’ into old_ptr, Write
    return old; // return the old value
}
```

#### test-and-set 构建锁

```c
typedef struct __lock_t{
    int flag; // lock 状态
} lock_t;

void init(lock_t *lock){
    // 0 -> lock is available
    // 1 -> held
    lock->flag = 0;
}

void lock(lock_t *lock){
    while(TestAndSet(lock->flag, 1) == 1){ // 检测锁是否被占用,同时将 flag 设置为 1(L1)
        ; // spin-wait(do noting)(L2)
    }
}

void unlock(lock_t *lock){
    lock->flag = 0; // 释放锁(U1)
}

```

#### test-and-set spin-lock 分析

![test-and-set-spin-lock](https://github.com/stardustman/pictures/raw/main/img/sync/ostep-lock-test-and-set.svg)

##### correctness

   由上图分析可知，test-and-set 这个 spin lock 提供了 mutual exclusion，这是锁的基本功能。

##### fairness

spin lock 不是公平的，可能导致线程饥饿。

##### performance

###### 单处理器

对于 spin lock，假定有 N 个线程，其中有一个线程已经持有的这个锁。其余 N - 1 个线程来获取这个锁，当调度器调度一个线程获取锁时，会在时间片(T)内一直死循环判定 TestAndSet 的返回值，直到时间片消耗完，线程切出。会浪费 (N - 1) * T 这么多时间。

###### 多处理器

![多核处理器-spin-lock](https://github.com/stardustman/pictures/raw/main/img/sync/ostem-lock-multi-core-contend-spin-lock.svg)

由图可知，在多处理器环境中，若 critical section 很短，在 Thread1 释放这个锁后，其余的线程是可以获取这个锁的。耗时并不长。

### Compare-And-Swap(Exchange) 构建锁

#### C 语言表示 Compare-And-Swap 语义

```c
int CompareAndSwap(int *ptr, int expected, int new) {
    int original = *ptr;
    if (original == expected)
    *ptr = new;
    return original;
}

```

#### Compare-And-Swap 构建锁

```c
typedef struct __lock_t{
    int flag; // lock 状态
} lock_t;

void init(lock_t *lock){
    // 0 -> lock is available
    // 1 -> held
    lock->flag = 0;
}

void lock(lock_t *lock){
    while(CompareAndSwap(&lock->flag, 0, 1) == 1){ // 检测锁是否被占用
        ; // spin-wait(do noting)(L2)
    }
}

void unlock(lock_t *lock){
    lock->flag = 0; // 释放锁(U1)
}

```

##### x86-version of compare-and-swap

```c
char compare_and_swap(int *ptr, int old, int new) {
    unsigned char ret;
    // Note that sete sets a ’byte’ not the word
    __asm__ __volatile__ (
    " lock\n" // 保证原子性
    " cmpxchgl %2,%1\n" // cmpxchgl new, *ptr, %rax = old，The ZF flag is set if the values in the destination operand and register AL, AX, or EAX are equal; otherwise it is cleared. 
    " sete %0\n" // ZF = 0
    : "=q" (ret), "=m" (*ptr) // output 0,1
    : "r" (new), "m" (*ptr), "a" (old) // input 2,3,4
    : "memory"); // clobber list
    return ret;
}

```

### Fetch-And-Add 构建锁

#### C 语言表示 Fetch-And-Add 语义

```c
int FetchAndAdd(int *ptr) {
    int old = *ptr;
    *ptr = old + 1;
    return old;
}

```

#### 构建 ticket lock

```c
typedef struct __lock_t{
    int ticket;
    int turn;
} lock_t;

void lock_init(lock_t *lock){
    lock->ticket = 0;
    lock->turn = 0;
}

void lock(lock_t *lock){
    int myturn = FetchAndAdd(&lock->ticket); // 获取票号，类似于用票号来给要获取锁的线程排队。拿到票号的最终都会成功获取锁。
    while(lock->turn != myturn){
        ; // spin
    }
}

void unlock(lock_t *lock){
    lock->turn = lock->turn + 1; // 下一位
}
```

## 如何避免 spin？

### yield(放弃 CPU)

```c
void init() {
    flag = 0;
}

void lock() {
    while (TestAndSet(&flag, 1) == 1)
    yield(); // give up the CPU, 线程状态由 running->ready
}

void unlock() {
    flag = 0;
}

```

当一个线程无法获取锁时，不是 spin，而是 yield(), 线程状态由 running 变为 ready。
这样就节约了时间。但是无法解决饥饿问题。
若一个线程不停的获取锁，进入临界区，释放锁。
其他的尝试获取锁的线程就会不断的 yield。

### Using Queues: Sleeping Instead Of Spinning

#### Lock With Queues, Test-and-set, Yield, And Wakeup

```c
typedef struct __lock_t {
    int flag;   // lock
    int guard;  // spin lock
    queue_t *q; // 队列,需要锁的线程放在队列里，避免饥饿。
} lock_t;

void lock_init(lock_t *m) {
    m->flag = 0;
    m->guard = 0;
    queue_init(m->q);
}

void lock(lock_t *m) {
    while (TestAndSet(&m->guard, 1) == 1)
        ; //acquire guard lock by spinning
    if (m->flag == 0) {
        m->flag = 1; // lock is acquired
        m->guard = 0;
    } else {
        queue_add(m->q, gettid()); //1 线程 id 放入队列
        setpark(); // 将要 park()！！！！
        m->guard = 0;              //2
        park();                    //3 running->sleep，被 unpark() 叫醒，就像在这里返回。先去获取 guard 这个 spin lock。
    }
}

void unlock(lock_t *m) {
    // 先获取 guard 这个 spin lock
    while (TestAndSet(&m->guard, 1) == 1){
        ; //acquire guard lock by spinning
    }

    if (queue_empty(m->q)) // 队列为空，没有等待锁的线程
        m->flag = 0; // let go of lock; no one wants it
    else
        unpark(queue_remove(m->q)); //4 hold lock, 叫醒这个 m->q 线程。

    // (for next thread!)
    m->guard = 0;
}

```

## Spin Lock 的问题

### spin forever

![spin-lock-spin-forever](https://github.com/stardustman/pictures/raw/main/img/sync/ostep-spin-lock-spin-forever.svg)

线程有优先级的概念。
Thread1(priority1) < Thread(priority2), 在 Thread1 和 Thread2 都是 runnable 情况下，调度器保证先运行 Thread2。
由上图可知，Thread2 可能永远无法获取锁，导致系统挂掉。

### Priority Inversion

![spin-lock-priority-inversion](https://github.com/stardustman/pictures/raw/main/img/sync/ostep-spin-lock-priority-inversion.svg)

有上图可知 Thread2 有可能在 Thread3 之前运行，这不符合线程优先级的要求。
Thread2 和 Thread3 的优先级反转了。

## two-phrase-lock

1. spin(先旋转一段时间，看能不能获取锁)
2. sleep(先旋转一段时间，不能获取锁，线程进入 sleep 状态，放弃 CPU)

## References

1. [Lock](https://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf)
2. [gcc-inline-assembly-howto](https://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html)
3. [atomic-exchange](https://www.felixcloutier.com/x86/xchg)
4. [compare-and-swap-lock](https://github.com/remzi-arpacidusseau/ostep-code/tree/master/threads-locks)
5. [lock](https://www.felixcloutier.com/x86/lock)
6. [cpmxchg](https://www.felixcloutier.com/x86/cmpxchg)
7. [xadd](https://www.felixcloutier.com/x86/xadd)
8. [system-call-table](https://filippo.io/linux-syscall-table/)
9. [sched_yield](https://man7.org/linux/man-pages/man2/sched_yield.2.html)
10. [gettid](https://man7.org/linux/man-pages/man2/gettid.2.html)