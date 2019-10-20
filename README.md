# Introduction
Codes for ALG(Prof. Yun Xu) Exercise and Other Practices
# Red Black Tree:
### problem to solve:
* return *this 执行后自动调用析构函数
>Watch out that if you try to use return *this; on a function whose return type is Type and not Type&, C++ will try to make a copy of the object and then immediately call the destructor, usually not the intended behaviour. So the return type should be a reference as in your example.(from stackoverflow)
* 初始化后key没有按照BST的顺序排列
### Attention:
* 成员尽量用指针表示，如果需要其指针值的话，否则成员函数不改变NODE的值，但是可能改变指针值
 
