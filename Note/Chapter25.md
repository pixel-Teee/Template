tuple通过位置来引用，而不是通过元素的名字。



在编译期间决定tuple的类型，在运行期间进行操控。



我们可以通过metafunction来存储类型的集合到tuple中。



# Basic Tuple Design

## Storage

Tuple存储每个类型在模板参数列表中。



设计思路是将一个元素个数大于0的Tuple，比如Tuple<int, double, std::string>

**看成是int和一个Tuple<double, std::string>。**



Tuple&lt;std::string&gt;可以看作是Tuple<>和std::string。

![image-20220222101525321](../Images/25.1.1.png)

![image-20220222102058199](../Images/25.1.1.a.png)

这里使用了类模板的特例化，因为函数模板没有特例化的性质，不然要使用标签派发来区分。

## Construction

![image-20220222104628961](../Images/25.1.2.png)

如果不加这个限制条件的话，那么传入一个tuple到构造函数的话，**会选择第一个，传给VHead**，造成类型不匹配。



```c++
template<typename... Types>
	auto makeTuple(Types&&... elems)
	{
		return Tuple<std::decay_t<Types>...>(std::forward<Tuples>(elems)...);
	}
```

函数模板相对于类模板的优势就是，会自动推导，可以裹一层，这样就不需要指定类型了。



std::decay主要是为了**解决原生数组和字面值常量的退化**，退化到指针。



# Tuple Algorithms



Tuple算法，它们需要编译时间和运行时间的开销。



应用一个算法到tuple上，会产生一个完全不同的类型，需要编译时间的计算。

而像其它同质容器，tuple也需要在运行时进行计算。



typelist算法被应用到元组类型经常被用来**决定结果元组类型。**

**也就是生成相应的新元组类型。**



# Optimizing Tuple

优化的方面：

1.运行时间(存储，执行时间)

2.编译时间(有多少**数量**的模板被实例化)



## Tuples and the EBCO



大多数非空tuple会在一个空tupe上面终止，数据成员总是要占一个字节的。



我们可以使用EBCO，空基类优化，通过从Tail Tuple继承，**而不是直接作为数据成员。**







































