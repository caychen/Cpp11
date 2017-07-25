#include <iostream>
#include <vector>
#include <map>

//1、显示转化操作符explicit
struct Rational1
{
	//没有explicit修饰的构造函数可以隐式调用，可以将某种类型隐式转化为类类型
	Rational1(int n = 0, int d = 1):num(n), den(d)
	{
		std::cout << "Rational(" << n << "/" << d << ")" << std::endl;
	}
	int num;		//被除数
	int den;		//除数
};

struct Rational2
{
	//而使用explicit修饰的构造函数则禁止隐式调用，禁止将某种类型隐式转化为类类型
	explicit Rational2(int n = 0, int d = 1) :num(n), den(d)
	{
		std::cout << "Rationa2(" << n << "/" << d << ")" << std::endl;
	}
	int num;		//被除数
	int den;		//除数
};

void Display1(Rational1 r)
{
	std::cout << "Numerator: " << r.num << ",Denominator: " << r.den << std::endl;
}
void Display2(Rational2 r)
{
	std::cout << "Numerator: " << r.num << ",Denominator: " << r.den << std::endl;
}

//C++11标准中，将explicit的使用范围扩展到了自定义的类型转换操作符上，以支持所谓的“显式类型转换”
class ConvertTo{};
class Convertable
{
public:
	//定义了一个显式转换到ConvertTo类型的类型转换符
	explicit operator ConvertTo () const{ return ConvertTo(); }
};
void Func(ConvertTo ct){}

void ExplicitTest()
{
	Convertable c;
	ConvertTo ct(c);
	//ConvertTo ct2 = c;		//拷贝构造初始化，编译失败
	ConvertTo ct3 = static_cast<ConvertTo>(c);//强制转化，ok
	//Func(c);				//拷贝构造初始化，编译失败
}

//2、列表初始化
//通过initializer_list来实现列表初始化
#include <initializer_list>
#include <vector>
#include <algorithm>	//for  for_each()

enum Gender{ male, female };
class Person
{
public:
	Person(std::initializer_list<std::pair<Gender, std::string>> ls)
	{
		auto lse = ls.begin();
		/*for (; lse != ls.end(); ++lse)
			pl.push_back(*lse);*/
		std::for_each(ls.begin(), ls.end(), [&](std::pair<Gender, std::string> pe){pl.push_back(pe); });
	}

	std::vector<std::pair<Gender, std::string>> pl;
};


void Fun(std::initializer_list<int> ls){}


class Mydata
{
public:
	Mydata& operator[](std::initializer_list<int> ls)
	{
		std::for_each(ls.begin(), ls.end(), [&](const int e){idx.push_back(e); });
		/*for (auto i = ls.begin(); i != ls.end(); ++i)
			idx.push_back(*i);*/
		return *this;
	}
	Mydata&	operator=(int v)
	{
		if (idx.empty() != true)
		{
			for (auto i = idx.begin(); i != idx.end(); ++i)
			{
				id.resize((*i > id.size() ? *i : id.size()));
				id[*i - 1] = v;
			}
			idx.clear();
		}
		return *this;
	}

	void Print()
	{
		std::for_each(id.begin(), id.end(), [](const int i) {std::cout << i << " "; });
		std::cout << std::endl;
	}
public:
	std::vector<int> idx;
	std::vector<int> id;
};

int main()
{
	Rational1 r1_1 = 11;		//Rational1(11/1)
	Rational1 r1_2(12);			//Rational1(12/1)

	//Rational2 r2_1 = 12;		//无法通过编译
	Rational2 r2_2(5);			//Rational2(5/1)

	Display1(1);				//Rational1(1/1)
	//Display2(2);				//无法通过编译
	Display2(Rational2(3));		//Rational2(3/1)


	//列表初始化,在｛｝之间的=号，加与不加效果相同
	int a[]{1, 2, 4};	//	int a[] = {1,2,4};
	std::vector<int> v{ 1, 2, 3 };
	std::map<int, float> m = { { 1, 1.0f }, { 2, 2.0f } };
	int c = { 3 + 4 };	//int c{3 + 4};		7

	//通过initializer_list实现列表初始化
	//例子1
	Person p = { { male, "Tom" }, { female, "Kitty" } };

	//例子2
	Fun({ 1, 2 });		//列表元素｛1，2｝
	Fun({});			//空列表

	//例子3
	Mydata d;
	d[{2, 3, 5}] = 7;
	d[{1, 4, 6, 9}] = 4;
	d.Print();

	return 0;
}