#include <iostream>

//对齐支持
//C++11标准定义alignof来查看数据的对齐方式
//Microsoft定义为__alignof来查看数据的对齐方式
struct EmptyStruct{};
class EmptyClass{};
class EmptyClass1;

struct HowManyBytes
{
	char a;
	int b;
};

struct ColorVector1
{
	double r;
	double g;
	double b;
	double a;
};

//C++11标准提供修饰符alignas设定struct/class/union的对齐方式
//Microsoft使用_declspec(align(#))来设定struct/class/union对齐方式
struct _declspec(align(32)) ColorVector2//直接将ColorVector设定在32字节的地址边界上，其起始地址必须是32的倍数
	//等价_declspec(align(32)) struct ColorVector
{
	double r;
	double g;
	double b;
	double a;
};

int main()
{
	std::cout << "sizeof(EmptyStruct): " << sizeof(EmptyStruct) << std::endl;//1
	std::cout << "sizeof(EmptyClass): " << sizeof(EmptyClass) << std::endl;//1
	std::cout << "__alignof(EmptyClass): " << __alignof(EmptyClass) << std::endl;//1

	//error,类型不完整，编译失败
	//std::cout << "__alignof(EmptyClass1): " << __alignof(EmptyClass1) << std::endl;//1

	std::cout << "sizeof(char): " << sizeof(char) << std::endl;//1
	std::cout << "sizeof(int): " << sizeof(int) << std::endl;//4
	std::cout << "sizeof(HowManyBytes): " << sizeof(HowManyBytes) << std::endl;//8

	std::cout << "offset of char a: " << offsetof(HowManyBytes, a) << std::endl;//0
	std::cout << "offset of int b: " << offsetof(HowManyBytes, b) << std::endl;//4

	std::cout << "__alignof(HowManyBytes): " << __alignof(HowManyBytes) << std::endl;//4
	std::cout << "__alignof(ColorVector1): " << __alignof(ColorVector1) << std::endl;//8
	std::cout << "__alignof(ColorVector2): " << __alignof(ColorVector2) << std::endl;//32
	std::cout << "sizeof(ColorVector2): " << sizeof(ColorVector2) << std::endl;//32

	std::cout << "in ColorVector1 offset of double r: " << offsetof(ColorVector1, r) << std::endl;//0
	std::cout << "in ColorVector1 offset of double g: " << offsetof(ColorVector1, g) << std::endl;//8
	std::cout << "in ColorVector1 offset of double b: " << offsetof(ColorVector1, b) << std::endl;//16
	std::cout << "in ColorVector1 offset of double a: " << offsetof(ColorVector1, a) << std::endl;//24

	std::cout << std::endl;
	std::cout << "in ColorVector2 offset of double r: " << offsetof(ColorVector2, r) << std::endl;//0
	std::cout << "in ColorVector2 offset of double g: " << offsetof(ColorVector2, g) << std::endl;//8
	std::cout << "in ColorVector2 offset of double b: " << offsetof(ColorVector2, b) << std::endl;//16
	std::cout << "in ColorVector2 offset of double a: " << offsetof(ColorVector2, a) << std::endl;//24

	int a;
	long long b;
	auto &c = b;
	char d[1024] = { 0 };
	//对内置类型
	std::cout << __alignof(int) << std::endl;//4

	//对变量、引用或者数组
	std::cout << __alignof(a) << std::endl//4
		<< __alignof(b) << std::endl//8
		<< __alignof(c) << std::endl//8		引用与其引用的数据对齐值相同
		<< __alignof(d) << std::endl;//1	数组的对齐值由其元素决定

	return 0;
}