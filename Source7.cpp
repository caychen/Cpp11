#include <iostream>
#include <vector>

//auto和decltype推导
double foo(){ return 0.0; }
int& bar(){ int a = 0; return a; }
float *bar1(){ float a = 0.0f; return &a; }

class White{};
class Black{};

template<typename T1, typename T2>
auto Sum(T1 t1, T2 t2) ->decltype(t1 + t2)
{
	auto s = t1 + t2;
	std::cout << typeid(s).name() << std::endl;
	return s;
}

void Overloaded(int){}
void Overloaded(char){}
int && RvalRe(){ return 1; }
const bool Func(int){ return true; }

int main()
{
	int x;
	int *y = &x;

	auto *a = &x;		//a: int*
	auto &b = x;		//b: int&
	auto c = y;			//c: int*
	auto *d = y;		//d: int*
	//auto *e = foo();	//编译失败，指针不能指向一个临时变量
	//auto &f = foo();	//编译失败，nonconst的左值引用不能和一个临时变量绑定
	auto g = bar();		//g: int
	auto &h = bar();	//h: int&

	//cv限制符，const和volatile
	//auto声明的普通变量不能从初始化表达式中带走cv限制符
	//而auto声明的引用和指针变量,则可以保持其原来对象的属性
	const auto a1 = foo();		//a1: const double
	const auto &b1 = foo();		//b1: const double&
	volatile auto *c1 = bar1();	//c1: volatile float*

	auto d1 = a1;				//d1: double
	auto &e1 = a1;				//e1: const double&	
	auto f1 = c1;				//f1: float*
	volatile auto &g1 = c1;		//g1: volatile float*&
	volatile auto h1 = c1;		//h1: volatile float*

	//错误的auto使用
	/*1、不能作为函数参数类型:
		void fun(auto i){}
		*/

	/*2、不能修饰非静态的成员变量
	struct f
	{
		auto var = 10;
	};*/

	/*3、不能在模版实例化时使用auto
	std::vector<auto> v = { 1 };
	*/

	/*4、不能用于修饰数组的类型
	int arr[3];
	auto x[3] = arr;
	*/

	White wh;
	Black bl;
	White wh2;

	//is_same模版函数成员类型value在编译时得到信息，而hash_code在运行时得到信息
	bool bFlags = std::is_same<White, Black>();	//false 0
	
	bFlags = (typeid(wh).hash_code() == typeid(bl).hash_code());
	std::cout << bFlags << std::endl;		//false 0

	bFlags = (typeid(wh).hash_code() == typeid(wh2).hash_code());
	std::cout << bFlags << std::endl;		//true	1

	/************************************************************************/
	//decltype
	std::vector<int> vec;
	typedef decltype(vec.begin()) vectype;
	for (vectype iv = vec.begin(); iv != vec.end(); ++iv)
	{	}
	for (decltype(vec)::iterator it = vec.begin(); it != vec.end(); ++it)
	{	}

	int i = 4;
	long l = 5;
	auto il = Sum<int, long>(i, l);
	std::cout << il << std::endl;
	std::cout << typeid(il).name() << std::endl;

	int aa;
	decltype(aa) bb;	//bb : int
	//decltype((aa)) cc;	//cc : int&， 编译失败	error

	//1、如果e是没有带括号的标记符表达式，或者类成员访问表达式，那么decltype(e)即为e所命名的实体类型；
	//2、否则，假设e的类型是T，如果e是一个将亡值(xvalue)，那么decltype(e)为T&&；
	//3、否则，假设e的类型是T，如果e是一个左值，则decltype(e)为T&；
	//4、否则，假设e的类型是T，则decltype(e)的类型为T。
	//
	//由推导规则，
	//（1）可以得知decltype(aa) bb；中aa为标记表达式，使用推导规则1，可知bb的类型为int；
	//（2）而decltype((aa)) cc;中(aa)不是标记表达式，却是一个左值，所以按照推导规则3，cc的类型为int&.

	int ii = 4;
	int arr[5] = { 0 };
	int *ptr = arr;
	struct S{ double d; }s;

	//规则1：
	decltype(arr) var1;			//int[5]，标记表达式
	decltype(ptr) var2;			//int*，标记表达式
	decltype(s.d) var3;			//double，标记表达式
	//decltype(Overloaded) var4;	//无法通过编译，函数是个重载函数

	//规则2：
	decltype(RvalRe()) var5 = 1;//int&&

	//规则3:
	decltype(true ? ii : ii) var6 = ii;	//int&，三元运算符，返回一个i的左值
	decltype((ii)) var7 = ii;			//int&，带圆括号的左值
	decltype(++ii) var8 = ii;			//int&，++ii返回ii的左值
	decltype(arr[3]) var9 = ii;			//int&，[]操作返回左值
	decltype(*ptr) var10 = ii;			//int&，*操作返回左值
	decltype("lval") var11 = "lval";	//const char(&)[5]，字符串字面常量为左值

	//规则4：
	decltype(1) var12;					//int，除字符串外字面值为右值
	decltype(ii++) var13;				//int，ii++返回右值
	decltype((Func(1))) var14;			//const bool，圆括号可忽略

	//使用模版类is_lvalue_reference的成员value来查看decltype的效果，1表示为左值引用，0则反之
	//使用模版类is_rvalue_reference的成员value来查看decltype的效果，1表示为右值引用，0则反之
	std::cout << std::is_lvalue_reference<decltype(true ? ii : ii)>::value << std::endl;	//1
	std::cout << std::is_lvalue_reference<decltype((ii))>::value << std::endl;				//1
	std::cout << std::is_lvalue_reference<decltype(arr[3])>::value << std::endl;			//1
	std::cout << std::is_lvalue_reference<decltype(*ptr)>::value << std::endl;				//1
	std::cout << std::is_lvalue_reference<decltype("lval")>::value << std::endl;			//1
	std::cout << std::is_lvalue_reference<decltype(++ii)>::value << std::endl;				//1
	std::cout << std::is_lvalue_reference<decltype(ii++)>::value << std::endl;				//0

	std::cout << std::is_rvalue_reference<decltype(RvalRe())>::value << std::endl;			//1
	std::cout << std::is_rvalue_reference<decltype(ii++)>::value << std::endl;				//0
	std::cout << std::endl;

	//与auto类型推导不能带走cv限制符不用，decltype能带走表达式的cv限制符，
	//不过，如果对象定义时使用const或者volatile限制符，使用decltype进行推导时，其成员不会继承const或volatile限制符
	//模版类is_const和is_volatile的成员value来查看类型是否是常量或是易失的
	const int ic = 0;
	volatile int iv;
	struct SS{ int i; };

	const SS sa = { 0 };
	volatile SS sb;
	volatile SS* ps = &sb;

	std::cout << std::is_const <decltype(ic)>::value << std::endl;		//1
	std::cout << std::is_volatile<decltype(iv)>::value << std::endl;	//1
	std::cout << std::is_const<decltype(sa)>::value << std::endl;		//1
	std::cout << std::is_volatile<decltype(sb)>::value << std::endl;	//1

	//成员不会继承const或者volatile限制符
	std::cout << std::is_const<decltype(sa.i)>::value << std::endl;		//0
	std::cout << std::is_volatile<decltype(ps->i)>::value << std::endl;	//0

	std::cout << std::endl;
	//cv限制符以及引用符号&，如果推导出来的类型已经有了这些属性，冗余的符号就会被忽略
	//注意：*符号不会被忽略
	int iii = 1;
	int &jjj = iii;
	int *ppp = &iii;
	const int kkk = 1;

	decltype(iii) & var15 = ii;	//var15：int&
	decltype(jjj) & var16 = ii;	//var16：int&，冗余的&被忽略

	std::cout << std::is_lvalue_reference<decltype(var15)>::value << std::endl;//1

	std::cout << std::is_rvalue_reference<decltype(var16)>::value << std::endl;//0
	std::cout << std::is_lvalue_reference<decltype(var16)>::value << std::endl;//1

	decltype(ppp) * var17 = &iii;	//无法通过编译
	decltype(ppp) * var18 = &ppp;	//var18的类型为int**

	auto *v3 = ppp;					//v3的类型为int*
	v3 = &iii;
	const decltype(kkk) var19 = 1;	//冗余的const，被忽略
	return 0;
}