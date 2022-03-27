## Banker's Algorithm

### 银行家算法是什么

`银行家`主要就是通过放贷来赚钱的。那最重要的问题是啥？当然是把钱借给还得起的人咯。试想，银行把钱都借给了还不起的人，那银行就完蛋了。假设有一批人（多个进程）来借钱（将要申请资源），但是银行剩下的钱满足不了任何人，那就直接拒绝借贷。当然了，有一部分已经借出的钱回收之后（回收已经分配的资源），又可以满足一批人中某些人的借贷需求。依次类推，银行可以判定能不能按照某个顺序来给这批人放贷。类似的思路延伸到计算机世界，同理。操作系统给多个进程分配资源，`能不能找到一个顺序给这些进程分配资源，并逐渐回收资源？`从而满足多个进程的资源需求。采用银行家放贷和收贷的思路（这里排除利息，放多少贷，收多少钱。呵呵，哪有此种好事？）来分配和回收系统资源就是所谓的`银行家算法`。

### 银行家算法解决什么问题

`resource allocation and deadlock avoidance algorithm`。银行家算法用来进行资源分配和避免死锁。

### 银行家算法怎样解决问题

P0、P1、P2、P3、P4 5个进程，视为需要钱的 5 个人。
A、B、C 三种系统资源。分别视为是银行三种贵金属：金块、银块、铜块。

#### allocation table 已放贷表

每个人已经借的金银铜个数列表

|  | A | B | C |
|:--:|:--:|:--:|:--:|
| P0 | 0 | 1 | 0 |
| P1 | 2 | 0 | 0 |
| P2 | 3 | 0 | 2 |
| P3 | 2 | 1 | 1 |
| P4 | 0 | 0 | 2 |

#### max table 最大借贷表

每个人需要金银铜最大个数列表

|  | A | B | C |
|:--:|:--:|:--:|:--:|
| P0 | 7 | 5 | 3 |
| P1 | 3 | 2 | 2 |
| P2 | 9 | 0 | 2 |
| P3 | 2 | 2 | 2 |
| P4 | 4 | 3 | 3 |

#### need table 还需放贷表

每个人还需要的金银铜个数列表。由 max table - allocation table 得到。

|  | A | B | C |
|:--:|:--:|:--:|:--:|
| P0 | 7 | 4 | 3 |
| P1 | 1 | 2 | 2 |
| P2 | 6 | 0 | 0 |
| P3 | 0 | 1 | 1 |
| P4 | 4 | 3 | 1 |

#### available table 剩余资源表

银行还剩下的金银铜个数。

| A | B | C |
|:--:|:--:|:--:|
| 3 | 3 | 2 |

#### 剩余系统资源分配顺序推演

1. 拿着 available table （剩余资源表），到 need table（还需放贷表） 比对。若 available table 的 A B C 满足 need table 的某一行（也就是 available table 每一列都大于等于 need table 的某一行的每一列）。说明可以分配给这个进程，标记这一行。同时回收这一行对应的 allocation table 中已经分配的 A B C。更新 available table 的 A B C。以此类推，继续比对 need table 中其他未标记的行。若最后 need table 所有行都有标记，说明存在一个给 P0、P1、P2、P3、P4 进程分配资源的顺序。
2. 拿着 available table 剩余资源表，不满足 need table 中任何一行。说明不存在一个给 P0、P1、P2、P3、P4 进程分配资源的顺序。系统有可能进入死锁。

#### 银行家算法图解分析

![银行家算法](https://github.com/stardustman/pictures/raw/main/img/banks-algorithm.svg)
存在的一个序列 P1 -> P3 -> P4 -> P0 -> P2

### 有没有其他方法解决此问题

### Banker's Algorithm implemention

```c
// Banker's Algorithm 
#include <stdio.h> 
int main() 
{ 
    // P0, P1, P2, P3, P4 are the Process names here 
  
    int n, m, i, j, k; 
    n = 5; // Number of processes 
    m = 3; // Number of resources 
    int alloc[5][3] = { { 0, 1, 0 }, // P0    // Allocation Matrix 
                        { 2, 0, 0 }, // P1 
                        { 3, 0, 2 }, // P2 
                        { 2, 1, 1 }, // P3 
                        { 0, 0, 2 } }; // P4 
  
    int max[5][3] = { { 7, 5, 3 }, // P0    // MAX Matrix 
                      { 3, 2, 2 }, // P1 
                      { 9, 0, 2 }, // P2 
                      { 2, 2, 2 }, // P3 
                      { 4, 3, 3 } }; // P4 
  
    int avail[3] = { 3, 3, 2 }; // Available Resources 
  
    int finished[n], ans[n], index = 0; 
    for (k = 0; k < n; k++) { 
        finished[k] = 0; 
    } 
    int need[n][m]; 
    for (i = 0; i < n; i++) { 
        for (j = 0; j < m; j++) 
            // 计算 need matrix
            need[i][j] = max[i][j] - alloc[i][j]; 
    } 
    int y = 0; 
    for (k = 0; k < 5; k++) { 
        for (i = 0; i < n; i++) { 
            if (finished[i] == 0) { 
  
                int flag = 0; 
                for (j = 0; j < m; j++) { 
                    if (need[i][j] > avail[j]){ 
                        flag = 1; 
                         break; 
                    } 
                } 
  
                if (flag == 0) { 
                    ans[index++] = i; 
                    for (y = 0; y < m; y++) 
                        avail[y] += alloc[i][y]; 
                    finished[i] = 1; 
                } 
            } 
        } 
    } 
  
    printf("Following is the SAFE Sequence\n"); 
    for (i = 0; i < n - 1; i++) 
        printf(" P%d ->", ans[i]); 
    printf(" P%d", ans[n - 1]); 
  
    return (0); 
  
} 
```

## References

1. [银行家算法-wiki](https://zh.wikipedia.org/wiki/%E9%93%B6%E8%A1%8C%E5%AE%B6%E7%AE%97%E6%B3%95)
2. [bankers-algorithm](https://www.geeksforgeeks.org/bankers-algorithm-in-operating-system-2/)
