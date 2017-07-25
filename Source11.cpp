#include <iostream>
#include <iterator> //for ostream_iterator

//常量表达式
//1、运行时常量性与编译时常量性即const与constexpr
const int GetConst(){ return 1; }	//运行时常量
constexpr int GetConstexpr(){ return 1; }	//编译时常量

//constexpt表达式函数的要求：
//1、函数体只用单一的return返回语句
//2、函数必须有返回值，不能是void函数
//3、在使用前必须已有定义
//4、return返回语句表达式中不能使用非常量表达式的函数、全局数据，且必须是一个常量表达式

//要求3：
constexpr int f();	//constexpr表达式函数声明
int a = f();
const int b = f();
constexpr int c = f();	//error，在使用前必须有定义
constexpr int f(){ return 1; }	//constexpr表达式函数定义
constexpr int e = f();	//ok，使用前已定义

//要求4：
constexpr int g(){ return GetConst(); } //error，无法在return返回语句中使用非常量表达式的函数
constexpr int h(){ return GetConstexpr(); } //ok，return语句中是一个常量表达式

int p = 3;
constexpr int m(){ return p; }	//error，常量表达式函数中无法返回全局数据


//常量表达式值：使用constexpr修饰的变量即为常量表达式值，
//常量表达式值必须被一个常量表达式赋值，在使用前必须被初始化
constexpr int aa = 1;
constexpr int bb = aa;//ok

//对于自定义的数据类型，要想成为常量表达式值的时候，需要自定义常量构造函数，只需在构造函数前加上constexpr关键字
//要求约束：
//1、函数体必须为空
//2、有成员变量，必须要初始化
//3、初始化列表只能由常量表达式来赋值
class ConstexprClass
{
public:
	constexpr ConstexprClass(int x) :i(x){}
	int i;
};

class Date
{
public:
	constexpr Date(int y, int m, int d) :year(y), month(m), day(d){}
	constexpr int GetYear(){ return year; }
	constexpr int GetMonth(){ return month; }
	constexpr int GetDay(){ return day; }

private:
	int year;
	int month;
	int day;
};

//常量表达式可以用于模版函数
//当声明为常量表达式的模版函数后，如果某个该模版函数的实例化结果不满足常量表达式需求的话，constexpr被自动忽略
struct NotLiteral
{
	NotLiteral(){ i = 5; }
	int i;
};

struct Literal
{
	constexpr Literal(){}
};

template<typename T>
constexpr T ConstExprFun(T t)
{
	return t;
}

//constexpr递归问题
constexpr int Fib(int n)
{
	return (n == 1 || n == 2 ? 1 : (Fib(n - 1) + Fib(n - 2)));
}
//模板元
template<long num = 10>
struct Fibo
{
	static const long result = Fibo<num - 1>::result + Fibo<num - 2>::result;
};
template<>
struct Fibo<0>{ static const long result = 0; };
template<>
struct Fibo<1>{ static const long result = 1; };
template<>
struct Fibo<2>{ static const long result = 1; };


int main()
{
	int arr[GetConst()] = { 0 };//无法使用运行时常量来初始化数组
	enum{ e1 = GetConst(), e2 };//也无法使用运行时常量作为枚举值

	int arr2[GetConstexpr()] = { 0 };//ok

	constexpr ConstexprClass mc(0); //必须使用常量表达式赋值
	//constexpr ConstexprClass mc = { 0 };等价上一条
	//constexpr ConstexprClass mc{ 0 }; 等价上一条

	//gcc-5.1报错："error: passing 'const Date' as 'this' argument discards qualifiers [-fpermissive]"
	constexpr Date d(1990, 11, 28);
	constexpr int year = d.GetYear();
	constexpr int month = d.GetMonth();
	constexpr int day = d.GetDay();
	std::cout << "year:" << year << std::endl
		<< "month" << month << std::endl
		<< "day" << day << std::endl;


	//结构体NotLiteral不是定义了常量表达式构造函数的类型，因此不能够声明为常量表达式值
	//而模版函数一旦以NotLiteral作为参数，那么其constexpr关键字就会被忽略
	NotLiteral nl;
	NotLiteral nl1 = ConstExprFun(nl);
	constexpr NotLiteral nl2 = ConstExprFun(nl);//编译失败
	constexpr int a = ConstExprFun(1);

	constexpr Literal l1;
	Literal l2 = ConstExprFun(l1);//ok
	constexpr Literal l3 = ConstExprFun(l1);//ok

	//constexpr函数递归
	int fibs[] = { Fib(11), Fib(12), Fib(13), Fib(14), Fib(15) };
	std::copy(std::begin(fibs), std::end(fibs),
		std::ostream_iterator<int>(std::cout, " "));//89 144 233 377 610 
	
	std::cout << std::endl;
	//使用模板元实现递归编程
	int fibs2[] = { Fibo<11>::result, Fibo<12>::result, Fibo<13>::result, Fibo<14>::result, Fibo<15>::result };
	std::copy(std::begin(fibs2), std::end(fibs2),
		std::ostream_iterator<int>(std::cout, " "));//89 144 233 377 610 

	return 0;
}