#include <iostream>

//默认函数控制
/*
	构造函数
	拷贝构造函数
	拷贝赋值函数
	移动构造函数
	移动拷贝函数
	析构函数
	=============
	operator ,
	operator &
	operator &&
	operator *
	operator ->
	operator ->*
	operator new
	operator delete
*/

class TwoCstor
{
public:
	TwoCstor() = default;

	//1、禁止使用拷贝构造函数，一旦缺省版本被删除，重载该函数也是非法的
	TwoCstor(const TwoCstor& t) = delete;

	//此处未使用"=default"
	TwoCstor& operator=(const TwoCstor& t);
};
//在类外使用"=default"来指明使用缺省版本
TwoCstor& TwoCstor::operator=(const TwoCstor& t) = default;

class ConvertTo
{
public:
	ConvertTo(int){}
	//2、删除参数为char的版本
	ConvertTo(char) = delete;
};

class ConvType
{
public:
	ConvType(int){}
	explicit ConvType(char) = delete;
};

//3、显式删除隐式数据类型转换
void func1(ConvertTo c){}
void func2(ConvType c){}

//4、显式删除普通的函数
void func3(int){}
void func3(char) = delete;

//5、显式删除在堆上分配class对象
class NoHeapAlloc
{
public:
	void* operator new(std::size_t) = delete;
};

int main()
{
	//如果TwoCstor() = default;改成TwoCstor(){},则is_trivial和is_pod则为0
	std::cout << std::is_trivial<TwoCstor>::value << std::endl;	//1
	std::cout << std::is_standard_layout<TwoCstor>::value << std::endl;	//1
	std::cout << std::is_pod<TwoCstor>::value << std::endl;	//1

	//1、
	TwoCstor t1;		//ok
	TwoCstor t2(t1);	//error，禁止使用拷贝构造函数

	//2、
	ConvertTo c1(1);	//ok
	ConvertTo c2('a');	//error，参数为char类型的构造函数被删除
	func1(1);	//ok
	func1('a');	//error，无法隐式类型转换

	//3、不建议explicit关键字与显式删除合用，避免两者同时使用
	ConvType cc1(1);
	ConvType cc2('a');	//error，参数为char类型的版本被删除，无法构造
	func2(1);
	func2('a');	//可以隐式转换为func(ConvType(97));

	//4、
	func3(1);	//ok
	func3('a');	//error，一般函数参数为char类型的被删除

	//5、
	NoHeapAlloc nha;	//ok
	NoHeapAlloc *pNha = new NoHeapAlloc();	//error，禁止使用操作符new

	return 0;
}