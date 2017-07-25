#include <iostream>

struct init
{
	int a = 1;
	//1、快速初始化成员变量
	//在成员快速初始化阶段时，不能使用（）来初始化，
	//对于非静态成员变量只能使用=或者｛｝来进行快速初始化
	std::string s{ "sss" };	//std::string s("aaa");---->error
	double d{ 3.0 };		//double d(3.0);---->error
};

class Mem
{
public :
	Mem(int i) :m(i){}
private:
	int m{0};
};

class Group
{
public :
	Group() = default;
	Group(int a) : data(a){}
	Group(Mem m) : mem(m){}
	Group(int a, Mem m, std::string s) :data(a), mem(m), name(s){}

private:
	int data = 1;
	Mem mem{ 0 };
	std::string name{ "123" };
};

class Person
{
public:
	int hand = 0;
	static Person* all;
	static int b;
};
Person* Person::all = nullptr;
int Person::b; //默认为0

//2、final用于修饰类或者函数，用于修饰的类则不具有继承性；用于修饰的函数则无法重载
class Base
{
	virtual void fun() = 0;
	virtual void fun3() = 0;
};

class Derived1 : public Base
{
	void fun() final{}
	virtual void fun2() = 0;

	//3、override函数重载
	//3-1、派生类在虚函数声明时使用override,则该函数必须重载基类中的同名函数，包括参数类型，参数列表，参数顺序等需要一致
	//否则编译出错
	//3-2、对非虚函数进行重载也会出错
	//void fun4() override {}	---->error
};

class Derived2 final: public Derived1
{
	//2-1
	//void fun(){}	//error ，Derived2的基类Derived1的fun函数采用final修饰，对于派生类来说就无法重载
	void fun2() override{}
};

//2-2
//基类Derived2才有final修饰，即Derived2类不可继承,否则编译出错
//class Derived3 : public Derived2
//{

//};

//4、模版函数的默认模版参数,
//4-1、规则：（1）对于类模版：在有多个默认模版参数声明为指定默认值时，遵照“从右往左”的规则指定
//			 （2）对于函数模版来说则不需要，可以根据函数参数类型推导模版类型
template<typename T = int>
class DefClass{};

template<typename T = int>
void DefFunc(){}

//error ,类模版默认参数未遵照从右往左的规则
template<typename T1 = int ,typename T2> class DefClass2{};
//ok
template<typename T1, typename T2 = int> class DefClass3{};

//ok
template<typename T1 = int, typename T2> void DefFunc2();
//ok
template<typename T1, typename T2 = int> void DefFunc3();


int main()
{
	Person p;
	std::cout << sizeof(p.hand) << std::endl;
	std::cout << sizeof(Person::all) << std::endl;
	std::cout << sizeof(((Person*)0)->hand) << std::endl;
	std::cout << sizeof(p.all) << std::endl;
	std::cout << p.b << std::endl;
	return 0;
}