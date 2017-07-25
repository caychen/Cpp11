#include <iostream>

namespace T	//命名空间T
{
	enum Color{ Black, Gray, Green };
}

namespace S
{
	enum Size{ Small = 1, Middle, Large };
}

namespace //匿名命名空间（全局）
{
	enum Color{Black,Blue,Gray};
}

//枚举类：强类型枚举
int main()
{
	//弱enum的缺点：
	//1、enum成员在函数内全局可见，所以下面第二行就会报错，提示重复定义
	enum Type1{ Purple, Red, White };
	//enum Color{Purple,Gray,Green};//Purple redefinition

	//具名namespace,class/struct及union必须未通过"名字::成员名"的方式访问，称为强作用域类型
	//而enum则为非强作用域类型
	//对于未使用名字，而直接使用enum的成员名的，默认进入全局命名空间
	T::Color c = T::Green;
	if (c == Black)	//Black是匿名命名空间的Black，而c为命名空间T内的Green，两者属于不同命名空间
		std::cout << "..." << std::endl;
	
	//2、enum可以隐式转换为整型，导致不同类型的值可以相互比较
	c = T::Gray;
	S::Size s = S::Small;
	if (c == s)
		std::cout << "match" << std::endl;//match

	//强枚举类型 enum class/struct
	//优点：
	//1、强作用域，强枚举类型成员的名称不会被输出到其父作用域空间
	//2、转换限制，强枚举类型成员的值不可以与整型隐式地相互转换，可以使用显式地强制转换
	//3、可以指定底层类型，强枚举类型默认底层类型为int，但也可以显式地指定底层类型
	//		如：enum class Type : char{};
	//		就指定了Type是基于char类型的枚举类型

	enum class Type{ General, Light, Medium, Heavy };
	enum class Color{ Black = 1, Red, Blue, Green };
	//enum class Color中的Red和enum Type1中的Red不冲突，这是强枚举类型中强作用域的特点，强枚举类型的名字不会被输出到父作用域中

	Type1 tt = Red;//ok，弱枚举类型的成员前可以不使用所属的枚举类型的名字
	Color cl = Red;//error，强枚举类型的成员前必须使用所属的枚举类型的名字

	Type t = Type::Light;
	Color clr = Color::Black;
	t = clr;			//类型不匹配，编译失败
	if (t == clr)		//类型不匹配，编译失败
		std::cout << "111" << std::endl;
	if (Type::Heavy > t)//编译通过
		std::cout << "222" << std::endl;	//222

	if (t > 0)			//不允许隐式转换为int类型，编译失败
		std::cout << "333" << std::endl;
	if ((int)t > 0)		//编译通过
		std::cout << "444" << std::endl;	//444

	std::cout << std::is_pod<Type>::value << std::endl;		//1
	std::cout << std::is_pod<Color>::value << std::endl;	//1

	enum class C : char{ C1 = 1, C2 };
	enum class D : unsigned{ D1 = 1, D2, Dbig = 0xFFFFFFFF0U };

	std::cout << sizeof(C::C1) << std::endl;	//1
	std::cout << (unsigned)D::Dbig << std::endl;//4294967280
	std::cout << sizeof(D::D1) << std::endl;	//4
	std::cout << sizeof(D::Dbig) << std::endl;	//4

	//C++11对原有的enum枚举类型进行扩展，即指定底层类型
	enum Type2 : char{ CC1 = 1, CC2 };
	return 0;
}