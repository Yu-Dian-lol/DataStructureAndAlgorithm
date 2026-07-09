# 数据结构与动态数组（按数据结构分类整理）

> MIT 6.006 Lecture 2 · 整理主线：**以每种数据结构为一块**，各自看"怎么存 + 支持哪些操作 + 复杂度 + 为什么"。
> 前置概念放最前面（够用即可），主体是三种结构，均摊分析并入 Dynamic Array。

---

## 0 · 前置概念（先垫地基，别当重点）

### 0.1 Interface vs Data Structure
- **Interface（ADT / API）= WHAT**：规定"支持哪些操作、每个操作什么含义"，不管怎么实现。
- **Data Structure = HOW**：具体"数据怎么存、操作怎么算"。
- 关系：**一个 Interface 可以被多种 Data Structure 实现**，性能各不同。下面三种结构，都是在实现同一个 **Sequence 接口**。

### 0.2 两大 Interface
| 接口 | 按什么存取 | 关心 |
|---|---|---|
| **Set** | 按 key | "是什么"（键） |
| **Sequence** | 按 position（x₀…x_{n-1}） | "在第几个"（索引） |

本讲三种结构都是 **Sequence** 的实现。Sequence 分两层：
- **Static sequence**：`get_at(i)` / `set_at(i,x)` / `len()` / `build` / `iter`
- **Dynamic sequence**：在上面基础上 **＋** `insert_at/delete_at`、`insert/delete_first/last`（会改变长度）

### 0.3 计算模型（Word RAM）——复杂度分析的前提
- 内存 = 一排 **w 位的字（word）**；数组 = 内存里一段**连续**的块。
- 连续 ⇒ `array[i] == memory[address(array) + i]` ⇒ **随机访问 O(1)**。
- 分配大小 n 的数组花 **O(n)** 时间；**space = O(time)**。
- 假设 **w ≥ log n**：因为要能寻址 n 个元素，一个 word 至少得装下一个索引（log₂n 位）。

> 这三条是下面所有复杂度结论的依据，记住"数组访问 O(1)"这个根就够了。

---

## 1 · Static Array（静态数组）

### 存储
连续内存，**大小固定 = n**，一个不多一个不少。

### 操作与复杂度
| 操作 | 复杂度 | 原因 |
|---|---|---|
| `get_at(i)` / `set_at(i)` / `len()` | **O(1)** | 地址直接算，随机访问 |
| `build(X)` / `iter_seq()` | **O(n)** | 要碰每个元素 |
| `insert_last` / `delete_last` | **O(n)** | 大小固定，得重分配 + 拷贝整个数组 |
| `insert_at(i)` / `delete_at(i)` | **O(n)** | 后面元素整体搬移一格 |

### 小结
**访问极快，但任何改变长度的操作都要 O(n)**（重分配或搬移）。这就是它撑不起动态序列的原因，引出 Dynamic Array。

---

## 2 · Dynamic Array（动态数组）★ 本讲核心

### 2.1 核心思想：放松约束
- Static array 强制 `size(array) == n` ⇒ 每次加元素都重分配 ⇒ O(n)。
- Dynamic array **放松为 `size = Θ(n)`**：预留空位、但只多常数倍。核心手段是 **满了就翻倍（×2）扩容**。

### 2.2 操作与复杂度
| 操作 | 复杂度 | 说明 |
|---|---|---|
| `get_at(i)` / `set_at(i)` | **O(1)** | 底层还是数组，随机访问不变 |
| `insert_last` / `delete_last` | **O(1) 均摊** | 大多数 O(1)，偶尔翻倍 O(n)，摊平后 O(1) |
| `insert_at(i)` / `delete_at(i)` | **O(n)** | 中间插删仍要搬移 |

### 2.3 insert_last 算法
```
insert_last(x):
    if n < size:                       # 有空位 → O(1)
        A[n] = x ; n += 1
    else:                              # 满了 → O(n)
        allocate new array of 2 * size # 翻倍
        copy all n elements over
        A[n] = x ; n += 1
```

### 2.4 为什么必须"翻倍"而不是"每次 +常数"
| 策略 | 重分配次数 | 总拷贝量 | 均摊 |
|---|---|---|---|
| 每次 +c（加常数） | n/c 次 | c+2c+…+n ≈ **O(n²)** | O(n) ❌ |
| ×2（翻倍） | log n 次 | 1+2+…+n ≈ **O(n)** | **O(1)** ✓ |

> 关键：翻倍让重分配**指数级稀疏**，拷贝总量收敛成等比数列 = O(n)。任何"乘法增长"（×1.5、×2）都行，"加法增长"不行。

### 2.5 ★扩容总代价的完整计算（把 `O(Σ2^i)` 补全）

从空数组做 n 次 `insert_last`，所有重分配拷贝加起来：

**Step 1** 重分配发生在 size = 1,2,4,…,n（都是 2 的幂），设 n=2^m，拷贝代价：
$$\sum_{i=0}^{m} 2^i,\quad m=\log_2 n$$

**Step 2** 等比数列求和（公比 2）：
$$\sum_{i=0}^{m} 2^i = \frac{2^{m+1}-1}{2-1} = 2^{m+1}-1$$

**Step 3** 代入 m=log₂n，用关键恒等式 **2^{log₂n} = n**：
$$2^{m+1}-1 = 2\cdot 2^{m}-1 = 2\cdot 2^{\log_2 n}-1 = 2n-1$$

**Step 4** 结论：总拷贝 = **2n − 1 = O(n)**；加上 n 次 O(1) 写入，总共 O(n)。

**Step 5** 均摊 = O(n)/n = **O(1) 每次**。

> 一句话：**等比 1+2+4+…+n ≈ 最后一项的 2 倍**（被最大项主导）；`2^{log n}` 看着吓人，其实就是 n。
>
> 数值验证：1+2+4=7=2·4−1 ✓；1+2+4+8=15=2·8−1 ✓。

### 2.6 Amortization（均摊分析）
**定义**：一个操作是 **T(n) 均摊**，若**任意 k 次**操作总时间 ≤ **k·T(n)**。
- 把"偶尔的贵操作"平摊到"大量便宜操作"上。
- `insert_last` **最坏单次 O(n)**（正好赶上翻倍），但**均摊 O(1)**。
- 注意：均摊 ≠ 平均（概率意义），它是对**任意序列**都成立的最坏保证。

### 2.7 delete_last 与缩容
- 一直删不缩容会浪费空间（违反 `size=Θ(n)`）。
- 策略：**n 掉到 size/4 时缩到 size/2**。
- **为什么 1/4 不是 1/2？** 防"抖动"：若半满就缩，在临界点反复 insert/delete 会不停重分配、每次 O(n)。留 1/4 缓冲带让扩/缩触发点错开，保证 delete 也 **O(1) 均摊**。

> Dynamic Array 就是 Python `list`、C++ `vector`、Java `ArrayList` 的底层。

---

## 3 · Linked List（链表）

### 存储
链式结点，每个结点 `[item | next]`，用指针串起来：
```
[item|next] → [item|next] → [item|next] → None
 head                              tail(可选)
```
维护 `head`、`len`，可选 `tail`。

### 操作与复杂度
| 操作 | 复杂度 | 原因 |
|---|---|---|
| `get_at(i)` / `set_at(i)` | **O(i)** | 不能直接寻址，从 head 走 i 步 |
| `insert_first` / `delete_first` | **O(1)** | 只改 head，不搬数据 |
| `insert_last` / `delete_last` | **O(1)\*** | 有 tail 时 O(1)；删末尾需双向链表才 O(1) |
| `insert_at(i)` / `delete_at(i)` | **O(i)** | 先走到位再改指针 |

### 变体
- **有 tail**：`insert_last` O(1)。
- **双向链表（doubly）**：`delete_last` 也 O(1)（能反向找前驱）。

### 小结
和 Array 正好互补：**访问慢（O(i)），但首部插删快（O(1)，只动指针）**。

---

## 4 · 三种结构横向对比总表（背这张）

| 操作 | Static Array | Dynamic Array | Linked List |
|---|---|---|---|
| `get_at / set_at(i)` | **O(1)** | **O(1)** | O(i) |
| `insert / delete_first` | O(n) | O(n) | **O(1)** |
| `insert / delete_last` | O(n) | **O(1) 均摊** | O(1)\*（有tail） |
| `delete_last` | O(1) | **O(1) 均摊** | O(n)/O(1)（双向） |
| `insert / delete_at(i)` | O(n) | O(n) | O(i) |
| 随机访问 | 快 | 快 | 慢 |
| 首尾插删 | 慢 | 末尾快 | 快 |

**选型逻辑（一句话）：**
- 操作以**随机访问 / 末尾追加**为主 → **Dynamic Array**（默认选它）。
- 操作以**首部/中间频繁插删、且常从已知结点出发**为主 → **Linked List**。
- Static Array 基本只当"大小已知、不变"的底层零件。

---

## 5 · 关键例子

### 例 1 · 从空插 8 个元素（看重分配多稀疏）
| 第 n 次 insert | 满? | 动作 | 拷贝代价 |
|---|---|---|---|
| 1 | 否 | 写 | 0 |
| 2 | 满 | 扩到2 | 1 |
| 3 | 满 | 扩到4 | 2 |
| 4 | 否 | 写 | 0 |
| 5 | 满 | 扩到8 | 4 |
| 6,7,8 | 否 | 写 | 0 |

拷贝总量 = 1+2+4 = **7 = 2·4−1 = O(n)** → 均摊 O(1)。重分配只在第 2、3、5、9…（2的幂+1）次发生，越来越稀疏。

### 例 2 · 翻倍 vs +1（插到 n=1000）
- 翻倍：约 log₂1000 ≈ **10 次**重分配，总拷贝 ≈ 2000 = O(n)。
- 每次+1：**1000 次**重分配，总拷贝 ≈ 500,000 = O(n²)。差 250 倍。

### 例 3 · array vs linked list（百万元素）
- `get_at(500000)`：array **O(1)** 一步；linked list **O(i)** 走 50 万步。
- `insert_first(x)`：array **O(n)** 搬百万个；linked list **O(1)** 只改 head。

---

## 6 · 速记

- **Interface = 要什么操作；Data Structure = 怎么实现。** 同接口可多实现。
- **Array：访问 O(1)，改长度贵。** Static 每次 O(n)；Dynamic 靠翻倍把 `insert_last` 做到 **O(1) 均摊**。
- **Linked List：访问 O(i)，首插删 O(1)。** 和 Array 互补。
- **均摊 O(1) 的证明核心**：等比 1+2+…+n = 2n−1 = O(n)，`2^{log n}=n`。
- **缩容在 1/4 触发**是为了防抖动。