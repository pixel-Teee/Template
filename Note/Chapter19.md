# Fixed Traits

![image-20220225111132978](../Images/19.1.1.png)

这里只有微妙的决定，是如何创建一个零值对于确定的类型，去开始我们的累加。



我们使用值初始化，使用{}，意味着局部对象要么被默认构造函数初始化，**或者被零值初始化(比如nullptr对于指针，而false对于bool值)。**



![image-20220225111717410](../Images/19.1.2.png)

计算字符数组的时候，**由于范围太小，而导致越界。**



一种解决方式：

```c++
accum<int>(name, name + 5);
```



另一种解决方式，创建一种联系在每个类型T以及相应的类型，**这种类型被用来持有累加的值。**

叫做T的trait。



![image-20220225112300897](../Images/19.1.3.png)

使用模板特例化，由于不会使用主模板，因此，我们没有它的定义。



这种模板叫做萃取模板，因为它持有它的参数类型的特性。

![image-20220225112834573](../Images/19.1.4.png)

修改后的函数。



# Value Traits

trait表示额外的类型信息，和一个给定"main"类型相关。

**不只是限制于类型。**常量和其它类的值也可以联系起来。



我们初始的accum模板，使用了默认构造函数对于返回值，去**初始化结果变量，我们希望的像零一样的值。**



AccT不一定有默认构造函数，我们可以添加一个value trait到我们的AccumulationTraits。



![image-20220225115011647](../Images/19.1.5.png)

我们的新trait提供一个零元素作为一个常量，可以在编译期间计算。



![image-20220225115239765](../Images/19.1.6.png)

C++允许去初始化一个静态常量数据成员在它的类里面，**如果它有一个整型或者枚举类型。**



constexpr静态数据成员允许浮点数。

![image-20220225115614773](../Images/19.1.7.png)



对于用户自定义的数据类型，一种解决方式就是，初始化别写类里面。

```c++
template<>
struct AccumulationTraits<BigInt>{
    using AccT = BigInt;
    static BigInt const zero;//只声明
};

BigInt const AccumulationTraits<BigInt>::zero = BigInt{0};
```



C++17支持内联：

```C++
template<>
struct AccumulationTraits<BigInt>{
	using AccT = BigInt;
	inline static BigInt const zero = BitInt{0};
};
```



C++17，支持内联成员函数。

![image-20220225132139712](../Images/19.1.8.png)

![image-20220225132322414](../Images/19.1.9.png)

但是获取的形式需要发生改变。



# Parameterized Traits



之前的Trait被叫做fixed，因为一旦trait被确定，那么它不能在算法中替换。



我们可以增加一个模板参数AT，**同时有一个默认值被trait模板决定。**



![image-20220225133220411](../Images/19.2.0.png)

现在trait可以进行替换了。



一般用户都不需要提供第二个参数。



# Traits versus Policies and Policy Classes



accumulation的操作不仅可以累加，也可以是累乘，对字符串来说，可以是连接，**这个操作可以称之为policy，对于我们的累加过程来说。**



![image-20220225134206534](../Images/19.2.png)

SunPolicy是个策略类，策略类是封装算法的。

一个类实现一个或多个策略通过一致的接口，这里是accumulate。



![image-20220225134917927](../Images/19.2.1.png)

累乘的策略类，但是乘出来结果是0，这给我们一个启发，就是决定初始值应该交由策略类。

但是不一定所有事情都要由trait和policy决定，**STL的accumulate的第三个参数就是初始值。**



# Traits and Policies: What's the Difference?



policies是trait的一种特殊情况。



Trait表示模板参数自然额外的属性。

Policies表示可配置的行为，对于通用的类型和函数。



# Member Templates versus Template Template Parameters

成员模板对比模板模板参数。



我们可以把策略实现为一个类模板，然后使用模板模板参数。



![image-20220225135847187](../Images/19.2.2.png)

模板模板参数。



![image-20220225135953983](../Images/19.2.3.png)

这里实例化这个类，**用其它模板参数来实例化模板模板参数。**



一般来说，trait排在policy后，写的时候，因为用户一般会重写这个策略的行为。



# Accumulation with General Iterators



处理通用迭代器，而不是普通的指针。

但是普通的指针依然可以使用，因为STL提供了一个**iterator traits。**



![image-20220225140700302](../Images/19.2.4.png)

通用迭代器。



std::iterator_traits结构封装了所有相关迭代器的属性。

由于部分特例化的存在，**这些trait可以方便地使用在指针上。**



![image-20220225140921208](../Images/19.2.5.png)

STL的trait的实现。



# Type Functions



类型函数。



使用模板，我们可以额外地定义类型函数。

函数取一些类型作为参数，并且产生一个类型或者常量作为结果。



一个非常有用的类型函数是sizeof。



类模板可以被当成类型函数。

**类型函数的参数是模板类型参数，结果被抽取出来作为成员类型或者成员常量。**



![image-20220225141542556](../Images/19.3.png)

![image-20220225143116959](../Images/19.3.1.png)

打印类型名。



实现其实可以简化的。

**如果容器类型定义了一个成员类型value_type。**



![image-20220225143502893](../Images/19.3.2.png)

我们可以实现如上的形式。



建议提供成员类型定义对于类模板参数，所以它们可以在通用代码中轻松地访问。

**像标准容器模板一样。**



![image-20220225144245655](../Images/19.3.3.png)



类型函数的作用就是，参数化一个模板依据容器的类型而不需要额外提供别的类型。

![image-20220225145021149](../Images/19.3.4.png)

本来要提供返回值类型的，现在不需要，**只需要提供一个类型函数，抽取出容器的元素类型即可。**



**元素类型是被类型函数决定的。**



类型ElemenT被叫做trait class，**因为它被用来访问一个给定容器类型C的trait。**

萃取类，不只限于描述容器元素类型，也可以是任何种类的主参数。



![image-20220225150107444](../Images/19.3.5.png)

用别名模板简化。



# Transformation Traits



## Removing References

除了从特定角度提供能力访问主模板参数，**traits也可以执行转换在types上。**

例如增加或者移除引用或者const或者volatile修饰符。



![image-20220225150805107](../Images/19.3.6.png)

去除引用是非常有用的，**当函数的返回类型是引用类型的时候。**



标准库提供了一个std::remove_reference<>。



## Adding References

我们可以使用一个现有的类型，然后从它创建左值引用或者右值引用。

![image-20220225151842445](../Images/19.3.7.png)

引用折叠会应用在这上面。



![image-20220225152040041](../Images/19.3.8.png)

轻量级的结构，但是我们不能使用void在这上面。



但是类模板的特例化可以处理特殊情况。



![image-20220225152358145](../Images/19.3.9.png)

右值引用也同样。



C++标准库，**提供std::add_lvalue_reference<>和std::add_rvalue_reference<>。**



## Removing Qualifiers

![image-20220225152703580](../Images/19.3.10.png)

![image-20220225153048090](../Images/19.3.11.png)

变换可以组合。



这里用到了元函数转发，从继承Type成员，从RemoveConstT，这样就减少了定义。



标准库提供了相应的trait，**比如std::remove_volatile<>，std::remove_const<>，std::remove_cv<>。**



## Decay

![image-20220225155911809](../Images/19.3.12.png)

传值，退化了。



int const、int [7]、int(int)退化成int, int\*, int(\*)(int)。



![image-20220225160500949](../Images/19.3.13.png)

我们可以实现这种机制的trait。



![image-20220225161557662](../Images/19.3.14.png)

第二种是为了兼容C语言的varargs。

![image-20220225161625867](../Images/19.3.15.png)

使用自制的trait。



## Predicate Traits

谓词萃取。



我们可以发明一个类型函数，它依赖于多个参数。

**类型谓词(类型函数生成一个布尔值)。**



![image-20220225162334908](../Images/19.3.16.png)

C++标准库有一个对应的类型萃取，std::is_same<>。



## true_type和false_type

![image-20220225164001560](../Images/19.3.17.png)

现在IsSameT<T, int>的结果类型，隐式地转变成它的基类TrueType或者FalseType，不仅提供正确的value成员，

**也允许我们去派发到不同的函数实现或者部分类模板的特例化在编译期间。**



例如：

![image-20220225164040096](../Images/19.3.18.png)

这种技术叫做**标签派发。**注意，true和false是值，而TrueType和FalseType是裹了它们两个值的类模板，因此可以通过类型匹配，

选择不同的函数。



**继承一个类，然后能够隐式转换成这个类，从而调用不同的重载函数。**



注意，TrueType和FalseType是类型，IsSameT继承自这个类型，隐式地转换成它们。



为了尽可能通用，这里应该只有一种类型标识true，一种类型表示false，**代替每个模板库定义它自己类型对于布尔常量。**



标准库：

```c++
//C++11和C++14
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

//C++17
using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

//bool_constant定义如下
template<bool B>
using bool_constant = integral_constant<bool, B>;
```



## Result Type Traits

结果类型萃取。



另一个解决多个类型的关于类型函数的例子是result type traits。

**它们非常有用，当书写操作符模板的时候。**



![image-20220225174123326](../Images/19.3.19.png)

为了获取T1，T2两个的结果类型。



PlusResultT trait决定了被生成的类型，通过相加两个类型的值，使用+ operator。



![image-20220225174723414](../Images/19.3.20.png)

有些些小问题，就是要求T1和T2必须拥有可以访问，非删除的，默认构造函数。



### declval



C++标准库提供了std::declval<>，即使没有构造函数也能产生值。



declval的定义：

```c++
template<typename T>
add_rvalue_reference_t<T> declval() noexcept;
```



declval&lt;T&gt;产生类型T的一个值。

这个函数模板故意没有定义，就像sizeof、decltype一样。

![image-20220225180230647](../Images/19.3.21.png)

使用这两个，就能消除值初始化的要求了。



# SFINAE-Based Traits



SFINAE允许替换失败的时候，使用重载解析，**去选择一个不同的候选者。**



SFINAE不仅支持函数重载时的错误，也可以判断一个类型是否有特定的成员，支持一个特定的操作，或者一个类。



## SFINAE Out Function Overloads

![image-20220225181807945](../Images/19.4.png)

第一次SFINAE的尝试是判断一个类型是否能够默认构造。



对于一个类型T，一个表达式，比如T()必须是有效的。



使用重载解析，来实现SFINAE-base trait。**注意，SFINAE是函数重载才有的。**

![image-20220225182635020](../Images/19.4.1.png)



对于任何的T，总是，所有的成员函数被替换，对于一个不能默认构造的类型，代码会编译失败，而不是忽略第一个test()重载。

![image-20220225200035088](../Images/19.4.2.png)

但是这个代码不知道为啥也能编译通过。



## Alternative Implementation Strategies for SFINAE-based Traits



C++98就有基于SFINAE的trait，它实现的方式是通过声明两个不同的重载函数模板返回不同的类型。



但是之前的实现是通过判断返回类型的大小来选择不同的重载解析的。

```c++
template<...> static char test(void*);
template<...> static long test(...);

enum{value = sizeof(test<...>(0)) == 1}
```



因为没有constexpr和nullptr。



老的机子的long和char类型相同，但是我们可以用如下的方式进行解决。

```c++
using Size1T = char;
using Size2T = struct{char a[2];}

//我们可以定义test()重载如下
template<...> static Size1T test(void*);
template<...> static Size2T test(...);//callback
```



## Making SFINAE-base Traits Predicate Traits



![image-20220225202115484](../Images/19.4.3.png)



一个谓词trait，返回一个布尔值，应当返回一个从std::true_type或者std::false_type派生的值。

trait应当从帮助类中派生，从而产生需要的基类。



如果test()函数模板是有效的，它是更好的重载，那么IsDefaultConstructibleHelper::Type被初始化test()的返回值。

**作为一个结果，IsDefaultConstructibeT从std::true_type派生。**



# SFINAE Out Partial Specializations

第二种方式实现SFINAE-based traits，使用部分特例化。

![image-20220225203608085](../Images/19.4.4.png)

如果T无法构造，那么整个特例化版本就会被丢弃。



注意，SFINAE一开始是从函数重载引出的，**但是类的特例化可以实现这种效果。**



# Using Generic Lambdas for SFINAE

我们使用的一些技术，一些样板代码总是需要的去定义trait，重载并且调用两个test()成员函数或者实现多个部分特例化。



在C++17中，我们可以最小化样板代码，**通过描述这个情况检查在通用lambda里面。**



![image-20220225213019695](../Images/19.4.5.png)

isValid，它是一个constexpr变量，它的类型是lambda闭包类型。**lambda创建了一个对象，并且返回。**

isValid能够调用。



![image-20220225210500825](../Images/19.4.6.png)

用法如上。isDefaultConstructible是一个函数对象，类型的trait，**是否可以默认构造。**



isValid是个萃取工厂，一个组件，可以生成trait，检查它的可调用对象是否可用。



type helper变量模板表示一个类型作为值。值x从这种方式获取的可以还原成原先的类型，**使用decltype(valueT(x))。**



x是TypeT&lt;T&gt;类型的，valueT(x)抽取出T。



```c++
//用法，这里的参数就是args
isDefaultConstructible(type<int>);
isDefaultConstructibel(type<int&>);
```

![image-20220225214333997](../Images/19.4.7.png)

这里内部那个进行了值初始化，decltype(x)，返回一个std::true_type或者std::false_type。



回忆SFINAE如何工作的？替换需要发生在替换模板的立即语境。

![image-20220225220941929](../Images/19.4.8.png)

别名模板的使用。

![image-20220225221200044](../Images/19.4.9.png)

一旦理解了，这个就很好用，比如hasFirst这个lambda测试是否有first成员，返回true_type或者false_type。





































