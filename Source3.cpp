#include <iostream>
#include <algorithm>
#include <functional>

#define  _NONOTE

//1、继承构造函数
class A
{
public:
	A(int i) {}
	A(double d) {}
	A(int i, double d){}
	void fun2(int i){ std::cout << "A:i = " << i << std::endl; }
};

class B :public A
{
public:
	//如果类B想拥有类A全部的构造函数的话，需要在类B的构造函数中一一实现，作为每一个类A的构造函数
	//为了避免在类B的构造函数中一一实现类A的构造函数而造成的麻烦，则使用using来声明完成
	//如果类B只是需要自己的构造函数，可以不用using，自定义构造函数即可
	using A::A;//让子类通过使用using来声明继承基类的构造函数，减少代码空间
	void fun(){}//派生类自定义成员函数

	//疑问？：
	//如果不对类B进行实现构造函数，即接下来的函数不定义，则对类似B b(2);的语句就无法编译成功，类B不是继承了类A的构造函数了嘛，不解？
	B(double d) :A(d){}

	//不止在构造函数的时候才可以using，对于函数同样实用
	//继承基类的同名函数
	//如果把using A::fun2;注释掉的话，类B只含有一个参数为double类型的fun2函数，相当于重写了基类A的fun2函数
	//而不注释的话，类B就含有两个fun2的重载函数，参数分别为int和double，根据参数的类型，正确的调用对应的fun2函数
#ifdef _NONOTE	
	using A::fun2;
#endif

	void fun2(double d){ std::cout << "B:d = " << d << std::endl; }
};


//2、委派构造函数
class Info
{
public:
	//默认原构造函数
	//Info() :type(1), c('a'){ InitRest(); }
	//Info(int i) :type(i), c('a'){ InitRest(); }
	//Info(char e) :type(1), c(e){ InitRest(); }

	//使用委派构造函数
	Info(){ InitRest(); }				//目标构造函数
	Info(int i) :Info(){ type = i; }	//委派构造函数
	Info(char e) :Info(){ c = e; }		//委派构造函数

	//注意点：
	//委派构造函数不能有初始化列表，所以构造函数中不能同时“委派”和使用初始化列表
	//如果委派构造函数要给变量赋初值，初始化代码必须放在函数体内
	Info(int i, bool bl) : Info(), b(bl){}	//error
	Info(bool bl, int i) :Info(i){ b = bl; }//ok

private:
	void InitRest(){}
	int type{ 0 };
	char c{ 'A' };
	bool b;
};

//建议：
//在使用委派构造函数的时候，建议抽象出最为“通用”的行为作为目标构造函数
//如类Ruler，将最直观最通用的Ruler(int i, char c);作为目标构造函数，其余可以作为委派构造函数
class Ruler
{
public:
	Ruler() :Ruler(0, 'a'){}
	Ruler(int i) :Ruler(i, 'a'){}
	Ruler(char c) :Ruler(0, c){}

private:
	Ruler(int i, char c) :type(i), chr(c){}
	int type;
	char chr;
};

//在委派构造函数中使用try，可以在目标构造函数中捕获异常，在委派构造函数函数中被捕获到。
class DCExcept
{
public:
	DCExcept(double d)
		try  DCExcept(1, d)
	{
			std::cerr << "Run Success" << std::endl;
	}
	catch (...)
	{
		std::cerr << "caught exception" << std::endl;
	}
private:
	DCExcept(int i, double d)
	{
		std::cout << "going to DCExcept(int ,double)" << std::endl;
		std::cout << "goint to throw" << std::endl;
		throw 0;
	}
private:
	int i;
	double d;
};

int main()
{
	A a(3);
	B b(2.0);

	//一旦使用了继承构造函数，编译器就不会为派生类生成默认的构造函数
	B b1;	//error

	//通过宏定义_NONOTE来作为using A::fun2;的开关
	//在不注释using A::fun2;的情况下，分别是A:i = 3，B:d = 3.5
	//在注释using A::fun2;的情况下，分别是B:d = 3，B:d = 3.5
	b.fun2(3);						  	  
	b.fun2(3.5);	

	return 0;
}
