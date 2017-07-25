#include <iostream>
#include <memory>

//智能指针与垃圾回收

void CheckWeakPtr(std::weak_ptr<int> & wp)
{
	std::shared_ptr<int> sp = wp.lock();
	if (sp != nullptr)
		std::cout << "still " << *sp << std::endl;
	else
		std::cout << "pointer is invalid" << std::endl;
}

//智能指针
//unique_ptr、shared_ptr、weak_ptr
//1、unique_ptr：不能与其他unique_ptr类型的指针对象共享对象的内存
//		unique_ptr是一个删除了拷贝构造函数，保留了移动构造函数的指针封装类型
//		所以unique_ptr不支持拷贝构造，即不支持将一个unique_ptr类型的指针赋值（复制）给另外一个指针，
//		只能通过右值对unique_ptr对象进行移动构造，一旦构造成功，
//2、shared_ptr：允许多个指针共享拥有同一个堆分配对象的内存，
//		某个shared_ptr失效，不会影响其他shared_ptr对对象内存的引用
//		shared_ptr的reset函数只是导致引用计数器降低，不会导致堆内存的释放，
//		只有当引用计数器的值归零的时候，shared_ptr才会真正地释放所占的堆内存空间
//3、weak_ptr：可以指向shared_ptr指针指向的对象内存，但不拥有该内存
//		使用lock成员函数，则可返回指向内存的一个shared_ptr对象，并且在所指向的对象内存无效时，返回空指针nullptr
int main()
{
	std::unique_ptr<int> up1(new int(11));
	std::unique_ptr<int> up2 = up1; //error，unique_ptr不共享指针对象的内存
	std::cout << *up1 << std::endl;	//11
	std::cout << *up2 << std::endl;	//error，报错

	std::unique_ptr<int> up3 = std::move(up1);//将up1强制转换成up3的右值引用，则up3获得了对象内存的使用权，up1则失去所有权
	std::cout << *up1 << std::endl; //terminate，运行错误，up1已经失效，失去对指针的所有权
	std::cout << *up3 << std::endl; //11
	up3.reset();//显式释放内存
	up1.reset();//不会导致运行错误

	std::cout << *up3 << std::endl; //terminate，up3释放之后，无法再使用*来访问堆内存

	std::shared_ptr<int> sp1(new int(12));
	std::shared_ptr<int> sp2 = sp1; //ok，sp1和sp2共享对象内存
	std::cout << *sp1 << std::endl; //12
	std::cout << *sp2 << std::endl; //12
	sp1.reset();
	std::cout << *sp2 << std::endl; //12，sp1的释放，不会影响sp2的访问操作

	std::weak_ptr<int> wp = sp1;	//指向shared_ptr对象
	std::cout << *wp << std::endl;	//error，虽然指向了shared_ptr对象，但是weak_ptr不拥有所指向的对象内存
	CheckWeakPtr(wp); //still 12
	sp1.reset();
	CheckWeakPtr(wp); //still 12
	sp2.reset();
	//当连续两次shared_ptr的reset函数，导致对堆内存对象的引用计数将为0，就释放堆内存空间，所以再次调用lock函数后，返回一个nullptr指针
	CheckWeakPtr(wp); //pointer is invalid

	return 0;
}