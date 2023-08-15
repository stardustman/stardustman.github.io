
### 线性 or 非线性

数据结构可以分为两大类：线性结构和非线性结构。线性结构典型的就是数组和链表，非线性典型就是树了。复杂的数据结构基本上都是两者的组合。

### binary heap

二叉堆本质上就是个完全二叉树。但是却用数组来表示。为啥呢？因为数组的下标之间有关系，可以表示二叉树节点之间的层级关系。因为是 `完全二叉树`，这就保证了一维展开就是一个下标连续的数组，数组通过下标操作起来相当简洁。堆顶(root)也就是数组第一个元素 `array[0]`，是最小元素，就是小顶堆。反之，大顶堆。

### 堆的自我调整

#### 插入节点

新节点放在数组最后，然后对该节点进行上浮操作。

#### 删除节点

删除堆顶元素，最后一个节点放在堆顶，对此时的堆顶进行下沉操作。

#### 构建二叉堆

所有的非叶子节点进行上浮操作。

#### 二叉堆自我调整图解


![binary-heap](https://github.com/stardustman/pictures/raw/main/img/binary-heap.svg) #(二叉堆的自我调整)

#### 代码实现

```java
import java.util.Arrays;

public class HeapOperator {

    // 自下而上，上浮
    public static void upAdjust(int [] array){
        // 最后一个节点
        int childIndex = array.length - 1;
        // 最后一个非叶子节点
        int parentIndex = (childIndex - 1) / 2;
        int temp = array[childIndex];

        while (childIndex > 0 && temp < array[parentIndex]){
            array[childIndex] = array[parentIndex];
            childIndex = parentIndex;
            parentIndex = (childIndex - 1) / 2;
        }
        array[childIndex] = temp;
    }

    // 自上而下，下沉
    public static void downAdjust(int [] array, int parentIndex, int length){
        int temp = array[parentIndex];
        int childIndex = 2 * parentIndex + 1;

        while (childIndex < length){
            // 有右节点，且右节点小于左节点
            if (childIndex + 1 < length && array[childIndex] > array[childIndex + 1]){
                childIndex++;
            }

            if(temp <= array[childIndex]){
               break;
            }
            array[parentIndex] = array[childIndex];
            parentIndex = childIndex;
            childIndex = 2 * parentIndex + 1;
        }
        array[parentIndex] = temp;
    }

    public  static  void buildHeap(int[] array){
        for (int i = (array.length - 1)/ 2; i >= 0 ; i--) {
            downAdjust(array, i, array.length - 1);
        }
    }

    public static void heapSort(int [] array){
        // 先构建一个小顶堆，得到一个最小值。
        // i 为 parentIndex，左右子节点分别是 2 * i + 1, 2 * i + 2
        for (int i = (array.length - 1 -2 ) / 2; i >= 0 ; i--) {
            downAdjust(array, i, array.length);
        }

        // 堆顶和目前堆的最后一个节点交换，重新构建堆。
        for (int i = array.length - 1; i > 0 ; i--) {
            int temp = array[i];
            array[i] = array[0];
            array[0] = temp;
            downAdjust(array, 0 , i);
        }
    }


    public static void main(String[] args){
        int [] array = new int[]{1,3,2,6,5,7,8,9,10,0};
        upAdjust(array);
        System.out.println(Arrays.toString(array));

        int[] array2 = new int[]{7,1,3,10,5,2,8,9,6};
        buildHeap(array2);
        System.out.println(Arrays.toString(array2));

        int[] array3 = new int[]{7,1,3,10,5,2,8,9,6,100};
        heapSort(array3);
        System.out.println(Arrays.toString(array3));
    }
}

```

### 优先级队列

不要被`队列`二字带偏，优先级队列的重点在优先级上。比如线程有优先级，优先级大的先执行。运算符也是有优先级的。怎样来实现这种优先级呢？用 `堆` 就可以。这里使用小顶堆，堆顶就是最小值，这里带入优先级的概念，值越小优先级越大。那么，从堆顶拿出来的节点就是优先级最大的，剩余的节点重新构造成小顶堆，保证堆顶是最小值。

#### 代码实现

`大顶堆`实现优先队列。每次只取最大值，取出一个堆顶，剩余节点重新构建大顶堆。

```java

import java.util.Arrays;

public class PriorityQueue {
    private int[] array;
    private int size;

    public PriorityQueue() {
        this.array = new int[32];
    }

    private void enQueue(int key){
        if(size >= array.length){
            resize();
        }
        array[size++] = key;
        upAdjust();
    }

    private int deQueue(){
        int heapTop = array[0];
        array[0] = array[--size];
        downAdjust();
        return heapTop;
    }

    private void resize() {
        int doubleSize = size << 2;
        this.array = Arrays.copyOf(this.array,doubleSize);
    }

    // 自下而上
    private void upAdjust(){
        // 拿到最后一个节点
        int childIndex = this.size - 1;
        // 最后一个非叶子节点
        int parentIndex = (childIndex - 1) / 2;
        int temp = array[childIndex];

        while (childIndex > 0 && temp > array[parentIndex]){
            array[childIndex] = array[parentIndex];
            childIndex = parentIndex;
            parentIndex = (childIndex - 1) / 2;
        }
        array[childIndex] = temp;
    }

    // 自上而下
    private void downAdjust(){
        int parentIndex = 0;
        int temp = array[0];
        int childIndex = 1;

        while (childIndex < size){
            // 有右节点，且右节点大于左节点。拿到两个子节点的最大值。
            if (childIndex + 1 < size && array[childIndex] < array[childIndex + 1]){
                childIndex++;
            }

            if(temp >= array[childIndex]){
                break;
            }
            array[parentIndex] = array[childIndex];
            parentIndex = childIndex;
            childIndex = 2 * parentIndex + 1;
        }
        array[parentIndex] = temp;
    }

    public static void main(String[] args){
        PriorityQueue priorityQueue = new PriorityQueue();
        priorityQueue.enQueue(30);
        priorityQueue.enQueue(100);
        priorityQueue.enQueue(50);
        priorityQueue.enQueue(300);
        priorityQueue.enQueue(130);
        priorityQueue.enQueue(430);
        System.out.println(priorityQueue.deQueue());
        System.out.println(priorityQueue.deQueue());
        System.out.println(priorityQueue.deQueue());

    }
}

```

### References

1. [visual heap](https://visualgo.net/zh/heap)
2. [binary heap](https://mp.weixin.qq.com/s/cq2EhVtOTzTVpNpLDXfeJg)
3. [heap sort](https://mp.weixin.qq.com/s/8Bid1naBLtEjPoP-R4HkBg)
4. [priority queue](https://mp.weixin.qq.com/s/4hXBw7sZ-NKs_asOQxS7gA)
5. [full binary tree vs complete binary tree](https://web.cecs.pdx.edu/~sheard/course/Cs163/Doc/FullvsComplete.html)
6. [heap review](https://web.cecs.pdx.edu/~sheard/course/Cs163/Doc/HeapReview.html)
