#include <iostream>

//指针空值nullptr
/*
	规则：
		1、所有定义为nullptr_t类型的数据都是等价的，行为也完全一致
		2、nullptr_t类型数据可以隐式转换成任意一个指针类型
		3、nullptr_t类型数据不能转换为非指针类型，即使使用reinterpret_cast<nullptr_t>()的方法也不行
		4、nullptr_t类型数据不适用于算术运算表达式
		5、nullptr_t类型数据可以用于关系运算表达式，但仅能与nullptr_t类型数据或者指针类型数据进行比较，当且仅当关系运算符为==，<=，>=等时返回true。
*/
void fun(int)
{
	std::cout << "fun(int)" << std::endl;
}

void fun(char*)
{
	std::cout << "fun(char*)" << std::endl;
}

template<typename T> void g(T* t){}
template<typename T> void h(T t){}

int main()
{
	fun(0);				//fun(int)
	fun(nullptr);		//fun(char*)

	std::cout << "===========================" << std::endl;
	//规则例子

	//nullptr可以隐式转换为char*
	char *cp = nullptr;

	//nullptr不能隐式转换为int，任何类型也不能转换为nullptr_t
	int n1 = nullptr;	//error
	int n2 = reinterpret_cast<nullptr_t>(nullptr);	//error

	//nullptr与nullptr_t类型变量可以作比较，当使用==，<=，>=符号作比较时才返回true
	nullptr_t nptr;
	if (nptr == nullptr)	//nullptr_t nptr == nullptr
		std::cout << "nullptr_t nptr == nullptr" << std::endl;
	else
		std::cout << "nullptr_t nptr != nullptr" << std::endl;

	if (nptr < nullptr)		//nullptr_t nptr >= nullptr
		std::cout << "nullptr_t nptr < nullptr" << std::endl;
	else
		std::cout << "nullptr_t nptr >= nullptr" << std::endl;

	//不能转换为整型或者bool类型
	if (0 == nullptr);	//error
	if (nullptr);	//error

	//不能进行算术运算
	nullptr += 1;	//error
	nullptr * 5;	//error

	sizeof(nullptr);
	typeid(nullptr);
	throw(nullptr);

	//nullptr的类型是是nullptr_t，而不是指针
	//模板只能把nullptr当作一个普通的类型进行推导，并不会将其视为T*指针，如果需要推导出指针类型，必须显式转换类型
	g(nullptr);		//error
	g((float*)nullptr);

	h(0);
	h(nullptr);
	h((float*)nullptr);

	//nullptr与(void*)0的区别
	/*
		1、nullptr是一个编译时期的常量，它的名字是编译时期的一个关键字，能够为编译器所识别，
			而(void*)0只是一个强制转换表达式，其返回的也是一个void*指针类型。
		2、在C++语言中，nullptr到任何指针的转换都是隐式的，而(void*)则必须经过类型转换后才能使用。
	*/
	int *px = (void*)0;	//error，不能隐式地将无类型指针转换为int*类型的指针
	int *py = nullptr;	//ok
	return 0;
}