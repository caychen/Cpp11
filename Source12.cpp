#include <iostream>
#include <cstdarg>
#include <string>

//回顾可变参数（变长函数）
template<typename T>
T SumOfValues(int nCount, ...)
{
	T Sum = 0;
	va_list vl;
	va_start(vl, nCount);
	for (int i = 0; i < nCount; ++i)
		Sum += va_arg(vl, T);
	va_end(vl);
	return Sum;
}

//变长模板类
//其中Elements称作是模板参数包，即编译器可以将多个模板参数打包成单个的模板参数包Elements
template<typename... Elements> 
class Tuple{};

//解包及包扩展
//解包：为了使用模板参数包，所以需要推导具体的模板类型，这个过程就叫做解包
//包扩展：完成解包的过程需要一个包扩展表达式
typedef int X;	//X随意
typedef double Y;//Y随意
template<typename T1, typename T2>class Base{};
template<typename... A>class Template : private Base<A...>{};
Template<X, Y> xy;
//过程：
//为类模板声明一个参数包A...，而使用参数包A...则是在Template的私有基类Base<A...>中
//最后一个表达式则声明了一个基类为Base<X,Y>的模板类Template<X,Y>的对象xy
//其中X,Y两个模版参数先是打包成参数包A，而后又在包扩展表达式A...中被还原


//变长模板类定义：递归
//通过定义递归的模板偏特化定义，可以使得模板参数包在实例化能够层层展开
//		直到参数包中的参数逐渐耗尽或者到达某个数量的边界为止。
template<typename... Elements> class tuple;//变长模板声明
template<typename Head, typename... Tail>//递归的偏特化定义
class tuple<Head, Tail...> : private Tuple<Tail...>
{
	Head head;
};
//边界条件
template<> class tuple<>{};

//非类型参数的变长模板类
template<long... nums> struct Multiply;	//变长模板声明
template<long first, long... nums>	//递归的偏特化定义
struct Multiply<first, nums...>
{
	static const long val = first * Multiply<nums...>::val;
};
//边界条件
template<>struct Multiply<>{ static const long val = 1; };


//变长模板函数：例子Printf
void Printf(const char* s)
{
	while (*s)
	{
		if (*s == '%' && *++s != '%')
			throw "invalid format string: missing arguments";
		std::cout << *s++;
	}
}
template<typename T , typename... Args>
void Printf(const char* s, T value, Args... args)
{
	while (*s)
	{
		if (*s == '%' && *++s != '%')
		{
			std::cout << value;
			return Printf(++s, args...);
		}
		std::cout << *s++;
	}
}

int main()
{
	//可变参数
	int iSum = SumOfValues<int>(4, 1, 2, 3, 4);
	std::cout << iSum << std::endl;

	double fSum = SumOfValues<double>(3, 3.2, 6.3, 8.1);
	std::cout << fSum << std::endl;

	//非类型参数的模板例子
	std::cout << Multiply<2, 3, 4>::val << std::endl;	//24
	std::cout << Multiply<22, 44, 66, 88>::val << std::endl;	//5622144


	//变长函数模板例子
	//Hello world
	Printf("Hello %s\n", std::string("world"));
	//The sum of (12 + 21) is 33
	Printf("The sum of (%d + %d) is %d\n", 12, 21, 12 + 21);
	return 0;
}