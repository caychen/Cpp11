#include <iostream>

/**********************************************************/
//测试类型是否是平凡的
struct Trivial1{};
struct Trivial2
{ 
	int a, b; 
};

struct Trivial3
{ 
	Trivial1 t1; 
	Trivial2 t2; 
	Trivial2 ta[2]; 
};

struct Trivial4
{ 
	int x; 
	static int y; 
};

struct Trivial5
{
	Trivial5() = default;
};

struct NoTrivial1
{ 
	NoTrivial1() :z(4){} 
	int z;
};

struct NoTrivial2
{
	NoTrivial2();
	int w;
};
NoTrivial2::NoTrivial2() = default;

struct NoTrivial3
{
	virtual void f();
};

/**********************************************************/
//测试类型是否是标准布局的
struct SLayout1{};
struct SLayout2
{
private:
	int x;
	int y;
};

struct SLayout3 : SLayout1
{
	int x;
	int y;
	void f();
};

struct SLayout4 : SLayout1
{
	int x;
	SLayout1 y;
};

struct SLayout5 { static int y; };

struct SLayout6 : SLayout5{ int x; };

struct NoSLayout1 : SLayout1	//不满足(3)类中第一个非静态成员的类型与其基类不同
{
	SLayout1 x;
	int i;
};

struct NoSLayout2 : SLayout2{ int z; };	//不满足(2)

struct NoSLayout3 : NoSLayout2{};

struct NoSLayout4	//不满足(1)
{
public:
	int x;
private: 
	int y;
};

/**********************************************************/
//测试类型是否是POD的
union U{};
union U1{ U1(){} };
enum E{};
typedef double*	DA;
typedef void(*PF)(int, double);

/**********************************************************/
int main()
{
	//POD 类型：Plain Old Data
	//平凡的(trivial)和标准布局的(standard layout)

	//(1)平凡的
	//1、拥有平凡的默认构造函数和析构函数，在C++11中可以使用=default关键字显式声明缺省的构造函数，从而恢复“平凡化”
	//	注意：Trivial5和NoTrivial2的default区别
	//2、拥有平凡的拷贝构造函数和移动构造函数
	//3、拥有平凡的拷贝赋值函数和移动赋值函数
	//4、不能包含虚函数以及虚基类

	//template<typename T> struct std::is_trivial;
	//通过类模板is_trivial的value值来判断T类型是否是个平凡的类型
	std::cout << std::is_trivial<Trivial1>::value << std::endl;		//1
	std::cout << std::is_trivial<Trivial2>::value << std::endl;		//1
	std::cout << std::is_trivial<Trivial3>::value << std::endl;		//1
	std::cout << std::is_trivial<Trivial4>::value << std::endl;		//1
	std::cout << std::is_trivial<Trivial5>::value << std::endl;		//1
	std::cout << std::is_trivial<NoTrivial1>::value << std::endl;	//0
	std::cout << std::is_trivial<NoTrivial2>::value << std::endl;	//0
	std::cout << std::is_trivial<NoTrivial3>::value << std::endl;	//0

	std::cout << std::endl;
	//(2)标准布局的
	//1、所有非静态成员有相同的访问权限
	//2、在类或结构体继承时，如果满足以下两种情况之一：
	//		1)派生类中有非静态成员，且只有一个仅包含静态成员的基类
	//		2)基类有非静态成员，而派生类没有非静态成员
	//3、类中第一个非静态成员的类型与其基类不同
	//4、没有虚函数和虚基类
	//5、所有非静态成员均符合标准布局类型，其基类也符合标准布局。

	//template<typename T> struct std::is_standard_layout;
	//通过类模板is_trivial的value值来判断T类型是否是个标准布局的类型
	std::cout << std::is_standard_layout<SLayout1>::value << std::endl;		//1
	std::cout << std::is_standard_layout<SLayout2>::value << std::endl;		//1
	std::cout << std::is_standard_layout<SLayout3>::value << std::endl;		//1
	std::cout << std::is_standard_layout<SLayout4>::value << std::endl;		//1
	std::cout << std::is_standard_layout<SLayout5>::value << std::endl;		//1
	std::cout << std::is_standard_layout<SLayout6>::value << std::endl;		//1
	std::cout << std::is_standard_layout<NoSLayout1>::value << std::endl;	//0
	std::cout << std::is_standard_layout<NoSLayout2>::value << std::endl;	//0
	std::cout << std::is_standard_layout<NoSLayout3>::value << std::endl;	//0
	std::cout << std::is_standard_layout<NoSLayout4>::value << std::endl;	//0

	std::cout << std::endl;
	//POD类型:很多内置的类型都是POD的
	//template<typename T> struct std::is_pod;
	std::cout << std::is_pod<U>::value << std::endl;	//1
	std::cout << std::is_pod<U1>::value << std::endl;	//0
	std::cout << std::is_pod<E>::value << std::endl;	//1
	std::cout << std::is_pod<int>::value << std::endl;	//1
	std::cout << std::is_pod<DA>::value << std::endl;	//1
	std::cout << std::is_pod<PF>::value << std::endl;	//1

	return 0;
}