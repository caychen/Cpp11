#include <iostream>
#include <vector>
#include <algorithm>	//for_each...
#include <functional>	//plus<int>()...
#include <numeric>	//accumulate...

//lambda函数
/*
	lambda函数语法定义：
		[capture](parameters) mutable -> return-type{statement}
	1、[capture]：捕捉列表，
	2、(parameters)：参数列表，与普通函数参数列表一致，如果不需要参数列表传递，可以同()一起省略
	3、mutable：mutable修饰符，默认情况下，lambda函数总是一个const表达式，mutable可以取消其常量性，
		在使用该修饰符时，参数列表不能省略，即使参数为空
	4、->return_type：返回类型，用追踪返回类型形式声明函数的返回类型。若不需要返回值的时候，可以连同符号->一起省略
		此外，在返回类型明确的情况下，也可以省略该部分，让编译器对返回类型进行推导
	5、{statement}：函数体，与普通函数体一样，除了使用参数之外，还可以使用所有捕获的变量


	捕捉列表的简单形式：
	[var]：表示值传递方式捕捉变量var
	[=]：表示值传递方式捕捉所有父作用域的变量，包括this
	[&var]：表示引用传递捕捉变量var
	[&]：表示引用传递捕捉所有父作用域的变量，包括this
	[this]：表示值传递方式捕捉当前的this指针

	复杂的组合形式：注意不允许重复捕捉变量
	[=,&var1,&var2]：表示以引用传递的方式捕捉变量var1和var2，值传递的方式捕捉其他所有变量
	[&,var1,this]：表示以值传递的方式来捕捉变量var1和this，引用传递方式捕捉其他所有变量

	//error
	[=,var1]:这里的=已经以值传递方式捕捉了所有的变量，再捕捉var1变量重复
	[&,&var1]:这里的&已经以引用传递捕捉了所有的变量，再捕捉var1变量重复
*/

//仿函数
class _functor
{
public:
	_functor() = default;
	int operator()(int a, int b)
	{
		return (a + b);
	}
};

int CommonFun(int times)
{
	//在程序运行时需要修改x的值，所以不能声明为const
	int x = 0;
	try
	{
		for (int i = 0; i < times; ++i)
			x += i;
	}
	catch (...)
	{
		x = 0;
	}
	return x;
}

int LambdaFun(int times)
{
	//在lambda函数中，只需使用其函数返回值，所以可以声明为const来保证起常量性
	const int y = [=]
	{
		int val = 0;
		try
		{
			for (int i = 0; i < times; ++i)
				val += i;
		}
		catch (...)
		{
			val = 0;
		}
		return val;
	}();
	return y;
}

void OneCond(int val)
{
	//传统for循环查找是否有val值
	for (auto i = nums.begin(); i != nums.end(); ++i)
	{
		if (*i == val)
			break;
	}

	//内置仿函数equal_to
	std::find_if(nums.begin(), nums.end(), std::bind2nd(std::equal_to<int>(), val));

	//使用lambda函数
	std::find_if(nums.begin(), nums.end(), [=](const int& i)
	{
		return i == val;
	});
}

void Add(const int val)
{
	auto print = [&]
	{
		for (auto n : nums)
			std::cout << n << "\t";
		std::cout << std::endl;
	};

	//传统for循环
	for (auto i = nums.begin(); i != nums.end(); ++i)
	{
		*i += val;
	}
	print();

	//for_each以及内置仿函数，此处的plus<int>()只是将加法结果返回，而不会再次应用到vector的nums上
	std::for_each(nums.begin(), nums.end(), std::bind2nd(std::plus<int>(), val));
	print();

	//对上述的改进，将加法结果再次传送到vector的nums中
	std::transform(nums.begin(), nums.end(), nums.begin(), std::bind2nd(std::plus<int>(), val));
	print();

	//lambda函数及for_each
	std::for_each(nums.begin(), nums.end(), [=](int &i)
	{
		i += val;
	});
	print();
}

std::vector<int> nums = { 1, 2, 9, 10, 21, 45, 37, 81 };
std::vector<int> LargeNums;
const int ubound = 10;
inline void LargeNumsFun(int i)
{
	if (i > ubound)
		LargeNums.push_back(i);
}

class LargeNumsClass
{
public:
	LargeNumsClass(int b) :bound(b){}
	void operator()(int val) const
	{
		if (val > bound)
			LargeNums.push_back(val);
	}
private:
	int bound;
};

void Stat(std::vector<int> &iv)
{
	int errors, scores;
	auto print = [&]
	{
		std::cout << "Errors: " << errors << std::endl
			<< "Score: " << scores << std::endl;
	};

	errors = std::accumulate(iv.begin(), iv.end(), 0);//add
	scores = std::accumulate(iv.begin(), iv.end(), 100, std::minus<int>());//minus
	print();

	errors = 0;
	scores = 100;
	std::for_each(iv.begin(), iv.end(), [=](const int& i)mutable
	{
		errors += i;
		scores -= i;
	});
	print();

}

int main()
{
	int a = 3, b = 4;
	//仿函数
	_functor f;
	std::cout << f(a, b) << std::endl;	//7

	//lambda函数
	auto fun = [=]{return a + b; };
	std::cout << fun() << std::endl;	//7

	std::cout << "CommonFun(10) = " << CommonFun(10) << std::endl;	//45
	std::cout << "LambdaFun(10) = " << LambdaFun(10) << std::endl;	//45

	std::cout << "=============================" << std::endl;
	/*
		注意点：
		1、对于按值方式传递的捕捉列表，其传递的值在lambda函数定义的时候就已经决定了
		2、按引用传递的捕捉列表变量，其传递的值则为调用lambda函数时的值。

		总结：
		1、如果需要捕捉的值成为lambda函数的常量，通常使用按值传递的方式捕捉
		2、如果捕捉的值成为lambda函数运行时的变量（类似参数），则应该采用按引用传递的方式捕捉
		*/

	int j = 12;
	auto by_val_lambda = [j]{return j + 1; };
	auto by_ref_lambda = [&j]{return j + 1; };

	std::cout << by_val_lambda() << std::endl;	//13
	std::cout << by_ref_lambda() << std::endl;	//13

	j++;
	std::cout << by_val_lambda() << std::endl;	//13
	std::cout << by_ref_lambda() << std::endl;	//14


	std::cout << "=============================" << std::endl;

	//C++11标准中允许lambda表达式向函数指针的转换，但前提是lambda函数没有捕捉任何变量，且函数指针所示的函数原型，必须跟lambda函数有着相同的调用方式
	auto totalChild = [](int x, int y)->int{return x + y; };
	typedef int(*TwoFun)(int, int);
	typedef int(*OneFun)(int);

	TwoFun tf;
	tf = totalChild;	//ok，lambda函数转换为函数指针

	OneFun of;
	of = totalChild;	//error，参数不一致

	decltype(totalChild) allChild = totalChild;
	decltype(totalChild) total = tf;	//error，函数指针无法转换为lambda

	std::cout << "=============================" << std::endl;
	//lambda常量性与mutable关键字
	int val = 2;

	auto const_val_lambda = [val]{val = 3; };	//error
	std::cout << "val = " << val << std::endl;	//2

	val = 2;
	auto mutalb_val_lambda = [val]()mutable{val = 3; };
	mutalb_val_lambda();
	std::cout << "val = " << val << std::endl;	//2

	val = 2;
	auto const_ref_lambda = [&val]{val = 3; };
	const_ref_lambda();
	std::cout << "val = " << val << std::endl;	//3

	val = 2;
	auto const_param_lambda = [&val](int v){v = 3; };
	const_param_lambda(val);
	std::cout << "val = " << val << std::endl;	//2


	std::cout << "=================================" << std::endl;
	//传统for循环
	LargeNums.clear();
	for (auto iter = nums.begin(); iter != nums.end(); ++iter)
	{
		if (*iter > ubound)
			LargeNums.push_back(*iter);
	}

	//使用函数指针
	LargeNums.clear();
	std::for_each(nums.begin(), nums.end(), LargeNumsFun);

	//使用仿函数
	LargeNums.clear();
	std::for_each(nums.begin(), nums.end(), LargeNumsClass(ubound));

	//使用lambda函数
	LargeNums.clear();
	std::for_each(nums.begin(), nums.end(), [=](const int& val)
	{
		if (val > ubound)
			LargeNums.push_back(val);
	});


	std::cout << "===========================" << std::endl;
	std::vector<int> iv(10);
	std::generate(iv.begin(), iv.end(), []{return rand() % 10; });
	Stat(iv);

	return 0;
}