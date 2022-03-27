
### B-tree 特性

1. 所有的 leaves 都在同一层级。
2. B-Tree 被 minimum degree t 定义。t 依赖于 disk block size。
3. 除了 root，其余节点必须至少有 t - 1 个 key。root 节点至少有 1 个 key。
4. 所有的节点（包括 root）最多有 2t - 1 个 key。
5. 某一个节点的子节点的个数 = 这个节点的 key 的个数 + 1。
6. 一个节点的所有 key 从小到大排序。两个 key1 和 key2 之间的所有 child 都在 key1 和 key2 之间。
7. B-Tree 从 root 节点 grow(扩张) 和 shrink(收缩)。
8. search、insert、delete 时间复杂度是O(log n)。n 是 key 的总数。

### t=3 为例来理解 B-tree

1. 每一个节点(root 除外) 至少有 t - 1 = 2 个 key。
2. 每一个节点最多有 2t - 1 = 5 个 key。节点 key 数量 = 5，称为节点满了。
问题来了，节点 key 个数大于 5 了咋办？拆分。什么时候来拆分呢？在向下遍历节点时发现满了，立即进行拆分。

#### B-Tree insert operation

##### 拆分已满节点

![btree-full-root-split](https://github.com/stardustman/pictures/raw/main/img/btree-full-root-split.svg) #(btree-拆分已满节点)

1. 如上图，节点1 已经满了，对此时的 root 节点进行拆分。
2. 创建节点2，s 节点，作为新的 root 节点，同时设置 s.child[0] = root。
3. 创建节点2，z 节点，将原来 root 节点一半的 key，复制到 z 节点。
4. 将 z 节点设置为 s 节点的 child。s.child[1] = z。

##### 拆分已满节点(有子节点)

![ btree-full-root-split2](https://github.com/stardustman/pictures/raw/main/img/btree-full-root-split2.svg) #(btree-拆分已满节点2)

B-Tree 是向上扩张的(grow)。可以看到所有的 key 都是在 leaf 节点插入的。节点的 key 值的数量一旦等于 2t - 1 = 5 时，就会对该节点进行拆分。所以这也是每一个节点(root 除外)至少有 t-1 = 2 个 key 的原因。

#### B-Tree delete operation

## References

1. [BTree-visualization](https://www.cs.usfca.edu/~galles/visualization/BTree.html)
2. [BTree introduction](https://www.geeksforgeeks.org/introduction-of-b-tree-2/)
3. [BTree insertion](https://www.geeksforgeeks.org/insert-operation-in-b-tree/)
4. [BTree deletion](https://www.geeksforgeeks.org/delete-operation-in-b-tree/)
