#include <iostream>
#include <vector>
#include <algorithm>

//追踪返回类型
template<typename T1, typename T2>
auto Sum(const T1 &t1, const T2 &t2) ->decltype(t1 + t2)
{
	return (t1 + t2);
}
template<typename T1, typename T2>
auto Mul(const T1 & t1, const T2 & t2) ->decltype(t1 * t2)
{
	return (t1 * t2);
}

class OuterType
{
	struct InnerType{ int i; };
	InnerType GetInnerType();
	InnerType it;
};
//这里OuterType::InnerType就可以省略为InnerType
auto OuterType::GetInnerType() -> InnerType
{
	return it;
}

int(*(*fp())())(){return nullptr; }

//auto(*)() -> int(*)()
//auto fp1() -> auto(*)() -> int(*)()
auto fp1() -> auto(*)() -> int(*)()
{
	return nullptr;
}

double foo(int a)
{
	return (double)a + 0.1;
}

int foo(double a)
{
	return (int)a;
}

template<typename T>
auto PreFoo(T t) -> decltype(foo(t))
{
	return foo(t);
}

int main()
{
	auto a = 3;		//int
	auto b = 4L;	//long
	auto c = 3.14;	//double
	auto d = Sum(a, b);
	auto e = Mul(d, c);
	std::cout << typeid(d).name() << std::endl;	//long
	std::cout << typeid(e).name() << std::endl;	//double

	std::cout << std::is_same<decltype(fp), decltype(fp1)>::value << std::endl;	//1

	std::cout << PreFoo(2) << std::endl;	//2.1
	std::cout << PreFoo(1.4) << std::endl;	//1

	//基于范围的for循环
	int ar[] = { 1, 2, 3, 4, 5 };
	std::vector<int> iv(ar, ar + 5);
	for (auto& i : iv)
		i *= 2;
	//2 4 6 8 10
	for (auto i : iv)
		std::cout << i << " ";
	//std::for_each(iv.begin(), iv.end(), [](const int i){std::cout << i << " "; });
	std::cout << std::endl;
	return 0;
}