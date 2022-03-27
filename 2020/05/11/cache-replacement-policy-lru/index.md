
## Cache

### cache

为啥要有 `Cache` 呢？根本原因是各种存储速度不匹配。或者为了加快某个过程，直接将多次转换的转换结果直接缓存起来，便于再次使用时直接绕开这些转换过程。典型的就是 `MMU` 的地址翻译过程，直接将`虚拟地址`多次转换的最后结果，也就是`物理地址`，直接缓存到 `TLB` 中，下次再次访问同一个虚拟地址，直接从虚拟地址拿到物理地址，绕开多次转换过程，这可不就提高了速度。`CPU` 的速度比 `Memory` 快得多，为了配上 `CPU` 超级快的速度，也就有了 `L1`，`L2`，`L3` cache。还有就是存储器层次结构，*速度快，容量小*的存储作为*速度慢，容量大*的上级缓存。

### cache replacement

为啥要有 `cache replacement` 呢？ 因为再大的存储容量也有限，总有使用完的时候，何况速度快的存储，容量本身就小。cache 使用完，再有需要缓存的东西，就需要从缓存中剔除一些，来存放新的内容。那问题就来了，缓存使用完，*该剔除哪些已经缓存的内容呢？* 这就是所谓的 `cache replacement policy`。从不同的角度，可以有不同的替换策略。

### least recently used

本文来说明这个策略。我不想记忆定义。来通过一个实例来理解这个策略的思想。
磁盘要缓存到内存中的内容划分为相等的 page，称之为 `virtual page`，每一个 page 一个编号，称之为 page number。
同理，内存的容量也划分为和磁盘大小一致的 page，称之为 `page frame`。
以下为访问磁盘 page number 的顺序：
1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5
缓存大小为 3 个 page frame。组成一个*缓存 Queue*

#### virtual page 存放规则

#### 缓存队列没满

1. virtual page 已在缓存队列中，将代表 virtual page 的队列节点移动到队首。
2. virtual page 不在缓存队列中，添加代表这个 virtual page 的队列节点到队首。

#### 缓存队列已满

1. virtual page 已在缓存队列中，将代表 virtual page 的队列节点移动到队首。
2. virtual page 不在缓存队列中，*移除队尾队列节点*，添加代表这个 virtual page 的队列节点到队首。

#### LRU 图解

![lru](https://github.com/stardustman/pictures/raw/main/img/lru.svg)

可以看到一直是对`缓存队列队首`操作。
1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5

1. 访问 page 1，缓存队列中没有，page 1 放到队首。
2. 访问 page 2，缓存队列中没有，page 2 放到队首。
3. 访问 page 3，缓存队列中没有，page 3 放到队首。
4. 访问 page 4，缓存队列中没有，且缓存队列满了。移除队尾 page 1，page 4 放到队首。
5. 访问 page 1，缓存队列中没有，且缓存队列满了。移除队尾 page 2，page 1 放到队首。
6. 访问 page 2，缓存队列中没有，且缓存队列满了。移除队尾 page 3，page 2 放到队首。
7. 访问 page 5，缓存队列中没有，且缓存队列满了。移除队尾 page 4，page 5 放到队首。
8. 访问 page 1，缓存队列中有，page 1 移动到队首。
9. 访问 page 2，缓存队列中有，page 2 移动到队首。
10. 访问 page 3，缓存队列中没有，且缓存队列满了。移除队尾 page 5，page 3 放到队首。
11. 访问 page 4，缓存队列中没有，且缓存队列满了。移除队尾 page 1，page 4 放到队首。
12. 访问 page 5，缓存队列中没有，且缓存队列满了。移除队尾 page 2，page 5 放到队首。
最后缓存队列中存放的是 page 5，page 4，page 3。

> 由上可知从队首到队尾包含着一个信息就是，越接近队首，越是最近刚访问的 page。缓存背后的原理其实和`程序运行的局部性原理`有关。
> 通过上述操作，可以看到队列节点调整非常频繁，且队列节点之间有先后关系。这也决定了队列是个双端队列。
> QNode 指针数组指向已经在缓存队列中的代表 page 的队列节点。

#### LRU 实现

```c
// A C program to show implementation of LRU cache 
#include <stdio.h> 
#include <stdlib.h> 
  
// A Queue Node (Queue is implemented using Doubly Linked List) 
typedef struct QNode 
{ 
    struct QNode *prev, *next; 
    unsigned pageNumber;  // the page number stored in this QNode 
} QNode; 
  
// A Queue (A FIFO collection of Queue Nodes) 
typedef struct Queue 
{ 
    unsigned count;  // Number of filled frames 
    unsigned numberOfFrames; // total number of frames 
    QNode *front, *rear; 
} Queue; 
  
// A hash (Collection of pointers to Queue Nodes) 
// 怎样判定 page 是不是已经在 cache 中，用这个结构。
typedef struct Hash 
{ 
    int capacity; // how many pages can be there 
    QNode* *array; // an array of queue nodes pointer，QNode 指针数组下标作为 page number 直接索引得到 QNode，或者判定 QNode 存在与否。
} Hash; 
  
// A utility function to create a new Queue Node. The queue Node 
// will store the given 'pageNumber' 
QNode* newQNode( unsigned pageNumber ) 
{ 
    // Allocate memory and assign 'pageNumber' 
    QNode* temp = (QNode *)malloc( sizeof( QNode ) ); 
    temp->pageNumber = pageNumber; 
  
    // Initialize prev and next as NULL
    temp->prev = temp->next = NULL; 
  
    return temp; 
} 
  
// A utility function to create an empty Queue. 
// The queue can have at most 'numberOfFrames' nodes 
Queue* createQueue( int numberOfFrames ) 
{ 
    Queue* queue = (Queue *)malloc( sizeof( Queue ) ); 
  
    // The queue is empty 
    queue->count = 0; 
    queue->front = queue->rear = NULL; 
  
    // Number of frames that can be stored in memory 
    queue->numberOfFrames = numberOfFrames; 
  
    return queue; 
} 
  
// A utility function to create an empty Hash of given capacity 
// capacity 其实就是 page number 范围
Hash* createHash( int capacity ) 
{ 
    // Allocate memory for hash 
    Hash* hash = (Hash *) malloc( sizeof( Hash ) ); 
    printf("====>%d\n", sizeof(Hash));
    hash->capacity = capacity; 
  
    // Create an array of pointers for refering queue nodes 
    hash->array = (QNode **) malloc( hash->capacity * sizeof( QNode* ) ); 
  
    // Initialize all hash entries as empty 
    int i; 
    for( i = 0; i < hash->capacity; ++i ) 
        hash->array[i] = NULL; 
  
    return hash; 
} 
  
// A function to check if there is slot available in memory 
int AreAllFramesFull( Queue* queue ) 
{ 
    return queue->count == queue->numberOfFrames; 
} 
  
// A utility function to check if queue is empty 
int isQueueEmpty( Queue* queue ) 
{ 
    return queue->rear == NULL; 
} 
  
// A utility function to delete a frame from queue 
void deQueue( Queue* queue ) 
{ 
    // 判断队列是不是空
    if( isQueueEmpty( queue ) ) 
        return; 
  
    // If this is the only node in list, then change front 
    // 只有一个 node
    if (queue->front == queue->rear) 
        queue->front = NULL; 
  
    // Change rear and remove the previous rear 
    QNode* temp = queue->rear; 
    queue->rear = queue->rear->prev; 
  
    if (queue->rear) 
        queue->rear->next = NULL; 
  
    free( temp ); 
  
    // decrement the number of full frames by 1 
    queue->count--; 
} 
  
// A function to add a page with given 'pageNumber' to both queue 
// and hash 
void Enqueue( Queue* queue, Hash* hash, unsigned pageNumber ) 
{ 
    // If all frames are full, remove the page at the rear 
    if ( AreAllFramesFull ( queue ) ) 
    { 
        // remove page from hash 
        // 移除队尾 QNode
        hash->array[ queue->rear->pageNumber ] = NULL; 
        deQueue( queue ); 
    } 
  
    // Create a new node with given page number, 
    // And add the new node to the front of queue 
    QNode* temp = newQNode( pageNumber ); 
    temp->next = queue->front; 
  
    // If queue is empty, change both front and rear pointers 
    if ( isQueueEmpty( queue ) ) 
        queue->rear = queue->front = temp; 
    else  // Else change the front 
    { 
        // 队首前一个 Node 是 temp
        queue->front->prev = temp;
        // 队首调整成 temp 
        queue->front = temp; 
    } 
  
    // Add page entry to hash also 
    hash->array[ pageNumber ] = temp; 
  
    // increment number of full frames 
    queue->count++; 
} 
  
// This function is called when a page with given 'pageNumber' is referenced 
// from cache (or memory). There are two cases: 
// 1. Frame is not there in memory, we bring it in memory and add to the front 
//    of queue 
// 2. Frame is there in memory, we move the frame to front of queue 
void ReferencePage( Queue* queue, Hash* hash, unsigned pageNumber ) 
{ 
    QNode* reqPage = hash->array[ pageNumber ]; 
  
    // the page is not in cache, bring it 
    if ( reqPage == NULL ) 
        Enqueue( queue, hash, pageNumber ); 
  
    // page is there and not at front, change pointer 
    else if (reqPage != queue->front) 
    { 
        // Unlink rquested page from its current location 
        // in queue. 把这个 Node 从队列上拿下来
        reqPage->prev->next = reqPage->next; 
        // 这个 Node 不是最后一个
        if (reqPage->next) 
           // reqPage 的下一个 Node 的 prev 指向 reqPage 的前一个 Node
           reqPage->next->prev = reqPage->prev; 
  
        // If the requested page is rear, then change rear 
        // as this node will be moved to front 
        // reqPage 是队尾
        if (reqPage == queue->rear) 
        { 
           // 更改队尾 Node
           queue->rear = reqPage->prev; 
           // 队尾 Node 没有下一个 Node 了
           queue->rear->next = NULL; 
        } 
  
        // Put the requested page before current front 
        // reqPage 成为新的队首 Node
        reqPage->next = queue->front; 
        reqPage->prev = NULL; 
  
        // Change prev of current front 
        // old 队首的 prev 变成 reqPage
        reqPage->next->prev = reqPage; 
  
        // Change front to the requested page 
        // 队列的队首是 reqPage
        queue->front = reqPage; 
    } 
} 
  
// Driver program to test above functions 
int main() 
{ 
    // 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5
    // Let cache can hold 3 pages 
    Queue* q = createQueue( 3 ); 
  
    // Let 6 different pages can be requested (pages to be 
    // referenced are numbered from 0 to 5 
    Hash* hash = createHash( 6 ); 
  
    // Let us refer pages  1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5
    ReferencePage( q, hash, 1); 
    ReferencePage( q, hash, 2); 
    ReferencePage( q, hash, 3); 
    ReferencePage( q, hash, 4); 
    ReferencePage( q, hash, 1); 
    ReferencePage( q, hash, 2);
    ReferencePage( q, hash, 5); 
    ReferencePage( q, hash, 1); 
    ReferencePage( q, hash, 2); 
    ReferencePage( q, hash, 3); 
    ReferencePage( q, hash, 4); 
    ReferencePage( q, hash, 5); 
  
    // Let us print cache frames after the above referenced pages 
    printf ("%d ", q->front->pageNumber); 
    printf ("%d ", q->front->next->pageNumber); 
    printf ("%d ", q->front->next->next->pageNumber); 
  
    return 0; 
} 
```

## References

1. [least recently used](https://www.geeksforgeeks.org/lru-cache-implementation/)
2. [cache wiki](https://en.wikipedia.org/wiki/Cache_(computing))
