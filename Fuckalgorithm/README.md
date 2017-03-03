# 测试用例输入输出约定

[TOC]

## 细胞聚合与分裂模型

假设有一个细胞聚合体    

- 会吞掉周围的(1\*1)的小细胞
- 会把自身的小细胞聚合成大细胞体，大细胞体的单位是由周围**N * N**的小细胞组合而成, 并且永远以生成最大的细胞体优先
- 能按照一定方向吐出自己的细胞

## 输入表达
    
- 输入尺寸为(M*N)的地图(地图里有一只细胞聚合体)
- 输入吞掉的小细胞的位置
- 输入吐出细胞的方向

### 地图表达

> M N   
> a_1, a_2, ... , a_m   
> b_1, b_2, ... , b_m   
> ..., ... , ... , ...     
> n_1, n_2, ... , n_m

- 第 1 行为两个整数M N，分别表示为地图场景的的`width`和`height`
- 第 2 行至第 N+1 行，每一行的数量为M，每个位置表示当前(x, y)坐标的状态
- 每个位置的value有：
	- `0` 表示当前格点不存在cell
	- `1`表示当前格点存在无法被聚合的cell
	- 从`2`至之后的数字开始表示被聚合的cell的聚合体的size
- 假定此地图只有一个细胞聚合体

### 插入点表达

> (x, y)

表示插入点在地图中的坐标

### cast up方向表达

扔出细胞的方向为8个方向  
使用
> "leftup", "leftdown", "rightup"， "rightdown"，"upleft", "upright", "downleft", "downright"

表示投出cell的8个方向

## 输出表达

返回经过吞并或吐出之后的聚合体的形状

## 输入输出格式

###### 地图 

`vector<vector<int>>`
###### 点 

```
class point {
public:
	size_t x, y;
}
```
######聚合体各个结构信息

```
class blockInfo {    
public:    
	int id;    
	int size;   
	point leftup;   
	point rightdown;   
	point central;   
};
vector<blockInfo>    
```


##条件
无

## 算法原理

整个聚合过程可以分为以下几步：  

	1. 搜索整个细胞聚合体，找到当前聚合体内最大的聚合体(正方形)，会得到一个解集S = { max(size(v)) }
	2. 根据游戏玩法条件，从解集S中筛选出所需的一个解  
	3. 将这个解得正方形从原地图中标记排除，从剩下的地图中再次寻找最大的聚合体
	4. 重复步骤1~3，直到得出的解集的size为1为止

吞并细胞过程：

	1. 将插入细胞的位置标记为`1`
	2. 将标记后的新聚合体扔进聚合算法，得到新聚合体

投出细胞过程：

	1. 根据聚合体的最大长`m`和宽`n`，构造一个(m*n)的搜索平面
	2. 根据投出细胞的方向，按照特定的起始位置和扫描路径搜索整个聚合体，直到找到值>=1的位置
	3. 将当前位置标记为`0`
	4. 将标记后的新聚合体扔进聚合算法，得到新聚合体

### 搜索位图连通域中最大正方形的方法

使用动态规划的思想可以在`O(n)`的时间复杂度内(一次问题空间遍历)找出最大正方形:    
核心思路为：**每个位置能构成的最大正方形是其上、左和左上能够成的最大正方形的最小值+1**

	1. 根据聚合体的最大长`m`和宽`n`，构造一个(m*n)的搜索平面state[n][m] （定义动规的状态state[n][m]）    
	2. 遍历搜索平面，值为0的点(x,y)填充state[y][x] = 0， 值>=1的点(x,y)填充state[y][x] = 1  (设定state的初值)     
	3. 遍历state，对每个state[x][y]>=1的点，按照`state[y][x] = min(state[y-1][x-1], state[y][x-1], state[y-1][x]) + 1`的公式填充state[y][x] (定义状态转移方程)
	4. state[y][x]最大的点为最大正方形的左下角坐标(x,y)，state[y][x]的值为最大正方形size 

### 包含Core的玩法

在原有的逻辑基础上，细胞的core会对条件有以下约束：

- core的值为`-1`，整个聚合体内只有一个core
- core的尺寸为1\*1至4\*4， 其中小于4\*4的core可拆分
- 每次进行聚合过程之前，如果当前core的尺寸小于4\*4，则优先在**以core为中央的最大为7\*7的平面**搜索最大为4\*4的正方形
- 每次吞并细胞的位置，若在**以core为中央的最大为7\*7**的范围内并且core的尺寸小于4\*4，则优先聚合core
- 搜索吐出细胞的位置，若搜索到`-1`并且core的尺寸小于4\*4，则吐出此位置，若core尺寸等于4\*4则跳过
- 聚合过程所求得的解集，优先选出最靠近core位置的解 (未实现)

## 性能分析

- 一次吞并或吐出的动作，所花费的时间复杂度为`O(nlogn)`，其中`n`代表遍历一次聚合体的问题空间大小   
- 每一次DP的时间复杂度为`O(n)`
- 每一次聚合最大细胞，就将问题空间收敛了最大的答案，因此递归搜索最大正方形的过程可以看做`O(logn)`
- 由于随着细胞增长，其衰减速度会渐渐大于增值速度，根据[blockor.io](http://blockor.io)的最大排行榜来看，`n`的最大值为10000个细胞左右，因此，在最大问题空间内，每次动作的运算次数为40000次左右


##输入用例

**输入例1**  
> 4 4  
> 2 2 1 0  
> 2 2 1 1  
> 0 1 0 0  
> 0 1 1 0  
> (2, 2)

**输出例1**
> 2 2 1 0  
> 2 2 1 1  
> 0 2 2 0  
> 0 2 2 0  
  
---
**输入例2**   

> 6 5  
> 2 2 0 1 1  
> 2 2 0 1 0  
> 3 3 3 1 0  
> 3 3 3 1 0  
> 3 3 3 1 0  
> 1 1 1 0 0  
> (3, 5)  

**输出例**  
> 2 2 0 1 1  
> 2 2 0 1 0  
> 4 4 4 4 0  
> 4 4 4 4 0  
> 4 4 4 4 0  
> 4 4 4 4 0

## 实际测试用例

```
插入点测试
input:
2 2 0 1 1 
2 2 1 1 0 
0 1 0 1 0 
0 1 1 1 0 
1 1 0 0 0 
id:1 size:2 leftup:(0, 0) rightdown:(1, 1)
input:
1 1 0 1 1 
1 3 3 3 0 
0 3 3 3 0 
0 3 3 3 0 
1 1 0 0 0 
id:1 size:3 leftup:(1, 1) rightdown:(3, 3)

input:
2 2 0 1 1 
2 2 1 1 0 
0 2 2 1 0 
0 2 2 0 0 
1 1 0 0 0 
id:1 size:2 leftup:(0, 0) rightdown:(1, 1)
id:2 size:2 leftup:(1, 2) rightdown:(2, 3)
input:
1 1 0 1 1 
1 3 3 3 0 
0 3 3 3 0 
0 3 3 3 0 
1 1 0 0 0 
id:1 size:3 leftup:(1, 1) rightdown:(3, 3)

input:
2 2 1 0 
2 2 1 1 
0 1 0 0 
0 1 1 0 
id:1 size:2 leftup:(0, 0) rightdown:(1, 1)
input:
2 2 1 0 
2 2 1 1 
0 2 2 0 
0 2 2 0 
id:1 size:2 leftup:(0, 0) rightdown:(1, 1)
id:2 size:2 leftup:(1, 2) rightdown:(2, 3)

input:
2 2 1 0 
2 2 1 1 
0 1 0 0 
0 0 0 0 
id:1 size:2 leftup:(0, 0) rightdown:(1, 1)
input:
2 2 1 0 
2 2 1 1 
0 1 1 0 
0 0 0 0 
id:1 size:2 leftup:(0, 0) rightdown:(1, 1)

input:
0 1 1 0 
1 2 2 0 
0 2 2 0 
0 0 0 0 
id:1 size:2 leftup:(1, 1) rightdown:(2, 2)
input:
1 1 1 0 
1 2 2 0 
0 2 2 0 
0 0 0 0 
id:1 size:2 leftup:(1, 1) rightdown:(2, 2)

input:
2 2 0 1 1 
2 2 0 1 0 
3 3 3 1 0 
3 3 3 1 0 
3 3 3 1 0 
1 1 1 0 0 
id:1 size:2 leftup:(0, 0) rightdown:(1, 1)
id:2 size:3 leftup:(0, 2) rightdown:(2, 4)
input:
2 2 0 1 1 
2 2 0 1 0 
4 4 4 4 0 
4 4 4 4 0 
4 4 4 4 0 
4 4 4 4 0 
id:1 size:4 leftup:(0, 2) rightdown:(3, 5)
id:2 size:2 leftup:(0, 0) rightdown:(1, 1)

input:
3 3 3 3 3 3 0 
3 3 3 3 3 3 0 
3 3 3 3 3 3 0 
0 0 0 3 3 3 0 
0 0 1 3 3 3 0 
2 2 1 3 3 3 0 
2 2 1 0 0 0 0 
id:1 size:3 leftup:(0, 0) rightdown:(2, 2)
id:2 size:3 leftup:(3, 0) rightdown:(5, 2)
id:3 size:3 leftup:(3, 3) rightdown:(5, 5)
id:4 size:2 leftup:(0, 5) rightdown:(1, 6)
input:
2 2 4 4 4 4 0 
2 2 4 4 4 4 0 
1 1 4 4 4 4 0 
0 0 4 4 4 4 0 
0 0 2 2 2 2 0 
2 2 2 2 2 2 0 
2 2 1 0 0 0 0 
id:1 size:4 leftup:(2, 0) rightdown:(5, 3)
id:2 size:2 leftup:(0, 5) rightdown:(1, 6)
id:3 size:2 leftup:(0, 0) rightdown:(1, 1)
id:4 size:2 leftup:(2, 4) rightdown:(3, 5)
id:5 size:2 leftup:(4, 4) rightdown:(5, 5)

input:
2 2 0 2 2 
2 2 -1 2 2 
0 1 1 1 0 
id:1 size:2 leftup:(0, 0) rightdown:(1, 1)
id:2 size:2 leftup:(3, 0) rightdown:(4, 1)
id:-1 size:1 leftup:(2, 1) rightdown:(2, 1)
input:
1 -1 -1 -1 1 
1 -1 -1 -1 1 
0 -1 -1 -1 0 
id:-1 size:3 leftup:(1, 0) rightdown:(3, 2)

input:
0 0 
0 -1 
id:-1 size:1 leftup:(1, 1) rightdown:(1, 1)
input:
0 0 
1 -1 
id:-1 size:1 leftup:(1, 1) rightdown:(1, 1)

投出点测试
input:
1 1 0 1 1 
1 3 3 3 0 
0 3 3 3 0 
0 3 3 3 0 
1 1 0 0 0 
id:1 size:3 leftup:(1, 1) rightdown:(3, 3)
output:
1 1 0 1 1 
1 3 3 3 0 
0 3 3 3 0 
0 3 3 3 0 
1 1 0 0 0 
id:1 size:3 leftup:(1, 1) rightdown:(3, 3)

input:
1 1 0 0 0 
1 3 3 3 0 
0 3 3 3 0 
0 3 3 3 0 
0 0 0 0 0 
id:1 size:3 leftup:(1, 1) rightdown:(3, 3)
output:
1 1 0 0 0 
1 3 3 3 0 
0 3 3 3 0 
0 3 3 3 0 
0 0 0 0 0 
id:1 size:3 leftup:(1, 1) rightdown:(3, 3)

input:
2 2 1 0 
2 2 1 1 
0 2 2 0 
0 2 2 0 
id:1 size:2 leftup:(0, 0) rightdown:(1, 1)
id:2 size:2 leftup:(1, 2) rightdown:(2, 3)
output:
2 2 1 0 
2 2 1 1 
0 2 2 0 
0 2 2 0 
id:1 size:2 leftup:(0, 0) rightdown:(1, 1)
id:2 size:2 leftup:(1, 2) rightdown:(2, 3)

input:
2 2 1 0 
2 2 1 1 
0 1 0 0 
0 0 0 0 
id:1 size:2 leftup:(0, 0) rightdown:(1, 1)
output:
2 2 1 0 
2 2 1 1 
0 1 0 0 
0 0 0 0 
id:1 size:2 leftup:(0, 0) rightdown:(1, 1)

input:
0 1 1 0 
1 2 2 0 
0 2 2 0 
0 0 0 0 
id:1 size:2 leftup:(1, 1) rightdown:(2, 2)
output:
0 1 1 0 
1 2 2 0 
0 2 2 0 
0 0 0 0 
id:1 size:2 leftup:(1, 1) rightdown:(2, 2)

input:
2 2 0 1 1 
2 2 0 1 0 
3 3 3 1 0 
3 3 3 1 0 
3 3 3 1 0 
1 1 1 0 0 
id:1 size:2 leftup:(0, 0) rightdown:(1, 1)
id:2 size:3 leftup:(0, 2) rightdown:(2, 4)
output:
2 2 0 1 1 
2 2 0 1 0 
3 3 3 1 0 
3 3 3 1 0 
3 3 3 1 0 
1 1 1 0 0 
id:1 size:2 leftup:(0, 0) rightdown:(1, 1)
id:2 size:3 leftup:(0, 2) rightdown:(2, 4)

input:
3 3 3 3 3 3 0 
3 3 3 3 3 3 0 
3 3 3 3 3 3 0 
0 0 0 3 3 3 0 
0 0 1 3 3 3 0 
2 2 1 3 3 3 0 
2 2 1 0 0 0 0 
id:1 size:3 leftup:(0, 0) rightdown:(2, 2)
id:2 size:3 leftup:(3, 0) rightdown:(5, 2)
id:3 size:3 leftup:(3, 3) rightdown:(5, 5)
id:4 size:2 leftup:(0, 5) rightdown:(1, 6)
output:
3 3 3 3 3 3 0 
3 3 3 3 3 3 0 
3 3 3 3 3 3 0 
0 0 0 3 3 3 0 
0 0 1 3 3 3 0 
2 2 1 3 3 3 0 
2 2 1 0 0 0 0 
id:1 size:3 leftup:(0, 0) rightdown:(2, 2)
id:2 size:3 leftup:(3, 0) rightdown:(5, 2)
id:3 size:3 leftup:(3, 3) rightdown:(5, 5)
id:4 size:2 leftup:(0, 5) rightdown:(1, 6)

input:
3 3 3 3 3 3 0 
3 3 3 3 3 3 0 
3 3 3 3 3 3 0 
0 0 0 3 3 3 0 
0 0 1 3 3 3 0 
2 2 1 3 3 3 0 
2 2 1 0 0 0 0 
id:1 size:3 leftup:(0, 0) rightdown:(2, 2)
id:2 size:3 leftup:(3, 0) rightdown:(5, 2)
id:3 size:3 leftup:(3, 3) rightdown:(5, 5)
id:4 size:2 leftup:(0, 5) rightdown:(1, 6)
output:
3 3 3 3 3 3 0 
3 3 3 3 3 3 0 
3 3 3 3 3 3 0 
0 0 0 3 3 3 0 
0 0 1 3 3 3 0 
2 2 1 3 3 3 0 
2 2 1 0 0 0 0 
id:1 size:3 leftup:(0, 0) rightdown:(2, 2)
id:2 size:3 leftup:(3, 0) rightdown:(5, 2)
id:3 size:3 leftup:(3, 3) rightdown:(5, 5)
id:4 size:2 leftup:(0, 5) rightdown:(1, 6)

input:
1 0 0 
-1 -1 0 
-1 -1 0 
id:-1 size:2 leftup:(0, 1) rightdown:(1, 2)
output:
1 0 0 
-1 -1 0 
-1 -1 0 
id:-1 size:2 leftup:(0, 1) rightdown:(1, 2)


```
