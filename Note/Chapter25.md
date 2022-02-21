# Optimizing Tuple

优化的方面：

1.运行时间(存储，执行时间)

2.编译时间(有多少**数量**的模板被实例化)



## Tuples and the EBCO



大多数非空tuple会在一个空tupe上面终止，数据成员总是要占一个字节的。



我们可以使用EBCO，空基类优化，通过从Tail Tuple继承，而不是直接作为数据成员。





































