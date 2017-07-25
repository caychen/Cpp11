#include <iostream>

//右值引用
//前瞻：浅拷贝和深拷贝
class ShollowCopyClass
{
public:
	ShollowCopyClass() :i(new int(0)){}
	ShollowCopyClass(const ShollowCopyClass& s)
		:i(s.i){}		//只是将类对象的指针i指向对象s的i，即指向了同一块内存空间
						//当其中一个对象释放后（delete），另一个对象的指针就变成了悬挂指针，在释放的时候就会中断
	~ShollowCopyClass(){ delete i; i = nullptr; }

private:
	int *i;
};

class DeepCopyClass
{
public:
	DeepCopyClass() :i(new int(0)){ std::cout << "Construct" << std::endl; }
	DeepCopyClass(const DeepCopyClass& d)
		:i(new int(*d.i)){
		std::cout << "Copy Construct" << std::endl;
	}	//重新分配新的内存空间，两个对象的指针指向不同的内存，在释放的时候就互不干扰

	//重载=运算符
	DeepCopyClass& operator=(DeepCopyClass& d1)
	{
		std::cout << "= Construct" << std::endl;
		return DeepCopyClass(d1);
	}
	~DeepCopyClass(){ delete i; i = nullptr; }

private: 
	int *i;
};


class HasPtrMem
{
public:
	HasPtrMem() :d(new int(3)){}
	
	//拷贝构造函数
	HasPtrMem(const HasPtrMem& h)
		:d(new int(*h.d)){}

	//移动构造函数
	HasPtrMem(HasPtrMem&& h)
		:d(h.d){
		h.d = nullptr;	//必须
		//在移动构造完成后，临时对象会立即被析构，
		//需要改变临时对象的指针成员，来避免临时对象析构的时候会将“偷”来的堆内存被析构掉
	}
	void AcceptRvalueRef(HasPtrMem&& h)
	{
		//std::move:强制将一个左值转化为右值引用，
		//如下，将左值h强制转化为对象hp的右值引用
		HasPtrMem hp = std::move(h);	//=====> static_cast<T&&>(lvalue)
	}

	//C++11 建议：
	//在一些简单的、不包含任何资源类型的类来说，是否实现移动语义就无关紧要，因为对于这样的类类型来说，移动就是拷贝，拷贝就是移动
	//而在包含资源类型的类中，拷贝构造和移动构造函数就应该同时提供（推荐），或者同时不提供

	~HasPtrMem(){ delete d; d = nullptr; }
	int *d;
};

//实现高性能的swap置换函数
//如果T是可移动的，整个代码就会按照移动语义来进行置换
//而如果T是不可移动的，则按照拷贝语义进行置换
template<typename T>
void SwapEx(T& a, T& b)
{
	T tmp(std::move(a));
	a = std::move(b);
	b = std::move(tmp);
}

//移动构造函数中的异常问题
//在移动构造过程中，抛出异常是比较危险的。
//如果在移动构造过程中，抛出异常，就会导致某些指针变成悬挂指针，所以在尽量不编写带有异常的移动构造函数，
//通常在移动构造函数中添加一个noexcept关键字，可以保证在移动构造中抛出的异常直接调用terminate程序终止运行，而不是造成悬挂指针的状态

//在标准库中可以使用std::move_if_noexcept的模版函数代替std::move函数
//如果在类的移动构造函数中没有noexcept关键字，返回一个左值引用从而使变量可以使用拷贝构造
//如果在类的移动构造函数中有noexcept关键字，则返回一个右值引用从而使变量可以使用移动构造
//if no noexcept---->copy
//if has noexcept---->move
class MayThrow
{
public:
	MayThrow(){}
	MayThrow(const MayThrow&)
	{
		std::cout << "MayThrow copy construct" << std::endl;
	}
	MayThrow(MayThrow&&)
	{
		std::cout << "MayThrow move construct" << std::endl;
	}
};

class NoThrow
{
public:
	NoThrow(){}
	NoThrow(const NoThrow&)
	{
		std::cout << "NoThrow copy construct" << std::endl;
	}
	NoThrow(NoThrow&&) noexcept /*注意在移动构造函数中加入noexcept关键字*/
	{
		std::cout << "NoThrow move construct" << std::endl;
	}
};

//完美转发forward（见深入理解C++11：章节3.3.6 完美转发）
//引用折叠
//在定义中一旦出现左值引用，引用折叠优先将其折叠为左值引用
void RunCode(int &m) { std::cout << "lvalue ref" << std::endl; }
void RunCode(int &&m) { std::cout << "rvalue ref" << std::endl; }
template<typename T>
void PerfectForward(T && t){ RunCode(std::forward<T>(t)); }


int main()
{
	ShollowCopyClass s1;
	ShollowCopyClass s2(s1);	//terminate
	
	DeepCopyClass d1;
	DeepCopyClass d2(d1);		//ok

	//用一个已存在的对象去构造一个不存在的对象(构造之前不存在), 就是拷贝构造运算.
	//用一个已存在的对象去覆盖另一个已存在的对象, 就是赋值拷贝运算.
	DeepCopyClass d3;
	DeepCopyClass d4 = d3;	//Copy construct
	d4 = d3;				//operator=()
	
	//左值(lvalue)：可以取地址，有名字的
	//右值(rvalue)：不可以取地址的，没有名字的
	//将亡值(xvalue):将要被移动的对象，被移为他用
	int c = 0;
	int &d = c;			//ok，左值引用
	int &&e = c;		//error，右值引用不能绑定到任何的左值（c有地址有名字，实际是个左值）
	int &&f = c + 1;	//ok，c+1是没有名字，也不能取地址，所以c+1是个右值
	

	MayThrow m;
	MayThrow mt = std::move_if_noexcept(m);		//MayThrow copy construct

	NoThrow n;
	NoThrow nt = std::move_if_noexcept(n);		//NoThrow move construct


	//完美转发例子
	int a, b;
	PerfectForward(a);				//lvalue ref
	PerfectForward(std::move(b));	//rvalue ref

	return 0;
}