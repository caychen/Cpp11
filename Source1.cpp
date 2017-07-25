#include <iostream>
#include <string>
#include <assert.h>

//2、_Pragma(字符串字面量)
//_Pragma("once") ===== #pragma  once
//

//3、变长参数宏定义及__VA_ARGS__   
//以省略号作为参数列表的最后一个参数
#define LOG(...) {\
	fprintf_s(stderr, "%s: Line %d:\t", __FILE__, __LINE__);\
	fprintf_s(stderr, __VA_ARGS__);\
	fprintf_s(stderr, "\n");\
}

template<typename T, typename U>
void bit_copy(T &a, U& b)
{
	//4、static_assert:静态断言，在编译的时候就进行断言
	//static_assert(sizeof a == sizeof b, "Two type Must have same width");		compile error
	memcpy_s(&a, sizeof a, &b, sizeof b);
}

//4、注意点：由于static_assert是在编译期间进行断言，即static_assert的第一个参数必须是个常量表达式，
//	如果使用变量，则会导致出错
void positive(const int n)
{
	//使用变量的断言，还是使用assert
	//static_assert(n > 0, "value must > 0");		error
	assert(n > 0);
}

//5、noexcept ：用来替代throw()
//直接在函数声明后带上noexcept关键字或者接受一个常量表达式作为参数，可以转换为bool类型
//若值为true，则表示函数不抛出异常，反之则抛出异常
//不带常量表达式的noexcept相当于noexcept(true),即不抛出异常
//vs好像不支持noexcept
void Throw(){ throw 1; }
void NoBlockThrow(){ Throw(); }
void BlockThrow() throw(){ Throw(); }

int main()
{
	//1、__fun__:返回函数名（C++11）--->VS2015
	//std::cout << __func__ << std::endl;
	std::cout << __LINE__ << std::endl;//行号
	std::cout << __FILE__ << std::endl;//文件名

	LOG("x = %d", __LINE__);

	int a = 3;
	double b = 4.0;
	bit_copy(a, b);

	try//Throw()
	{
		Throw();
	}
	catch (...)
	{
		std::cerr << "Throw() error" << std::endl;
	}

	try//NoBlockThrow()
	{
		NoBlockThrow();
	}
	catch (...)
	{
		std::cerr << "NoBlockThrow() error" << std::endl;
	}

	try//BlockThrow()
	{
		BlockThrow();
	}
	catch (...)
	{
		std::cerr << "BlockThrow() error" << std::endl;
	}

	return 0;
}