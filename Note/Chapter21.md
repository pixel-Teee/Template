# EBCO

一个空类：只包含类型成员，**非虚函数成员**，静态数据成员。



![image-20220221235556390](../Images/21.1.png)

EmptyToo继承Empty，EmptyThree继承EmptyToo。

那么Empty对于EmptyToo如果是个空基类的话，就会被优化掉，这就是EBCO名字的由来。



