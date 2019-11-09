# Introduction
Codes for ALG(Prof. Yun Xu) Exercise and Other Practices
# Nearest Vertex_Pair:
* 找好边界条件
```
BestPair temp1 = Get_Nearest_Vertex(vertexset, vertexset_ysort, begin, Mid_Num);
BestPair temp2 = Get_Nearest_Vertex(vertexset, vertexset_ysort, Mid_Num + 1, end);
```
如果这么写就是把Mid_Num和end视为一个合理元素，否则会遗漏Mid_Num
* 剪枝
如果不采用每次生成中位数的方式，而是实现排好序，那么可能的合并区间只能出现在右面边界点的左delta部分和左面边界点的右delta的右面的并，根据这一性质可以实现有效剪枝。
# Red Black Tree:
### problem to solve:
* return *this 执行后自动调用析构函数
>Watch out that if you try to use return *this; on a function whose return type is Type and not Type&, C++ will try to make a copy of the object and then immediately call the destructor, usually not the intended behaviour. So the return type should be a reference as in your example.(from stackoverflow)
* 初始化后key没有按照BST的顺序排列
### Attention:
* 成员尽量用指针表示，如果需要其指针值的话，否则成员函数不改变NODE的值，但是可能改变指针值
 
