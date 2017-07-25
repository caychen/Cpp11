#include <iostream>
#include <cstdlib>
#include <thread>
#include <atomic> //C++11新头文件	for  atomic_***原子数据类型
#include <windows.h>	// for Sleep

//原子操作及原子数据类型
/*
	typedef enum memory_order {
	memory_order_relaxed,
	memory_order_consume,
	memory_order_acquire,
	memory_order_release,
	memory_order_acq_rel,
	memory_order_seq_cst
	} memory_order;

	memory_order_relaxed：不对执行顺序作任何的保证，即原子操作顺序不一定
	memory_order_consume：本线程中，所有后续的读操作必须在本条原子操作完成后执行
	memory_order_acquire：本线程中，所有之前的写操作完成后才能执行本条原子操作
	memory_order_release：同时含有memory_order_consume和memory_order_acquire标记
	memory_order_acq_rel：本线程中，所有后续的有关本原子类型的操作，必须在本条原子操作完成后执行
	memory_order_seq_cst: 全部存储都按照顺序执行（默认值）

	1、原子存储操作(store)：可以使用memory_order_relaxed、memory_order_release、memory_order_seq_cst
	2、原子读取操作(load)：可以使用memory_order_relaxed、memory_order_consume、memory_order_acquire、memory_order_seq_cst
	3、RWM操作(read-write-modify)：同时读写操作，可以使用全部memory_order枚举值
*/

std::atomic<int> a{ 0 };
std::atomic<int> b{ 0 };
void Thread1()
{
	int t = 1;
	a.store(t, std::memory_order::memory_order_relaxed);
	b.store(2, std::memory_order::memory_order_relaxed);
}

void Thread2()
{
	while (b.load(std::memory_order::memory_order_relaxed) != 2)
		;
	std::cout << a.load(std::memory_order::memory_order_relaxed) << std::endl;
}

std::atomic<int> c{ 0 };
std::atomic<int> d{ 0 };
//memory_order_release和memory_order_acquire结合使用，称为该内存顺序为release-acquire内存顺序
void Thread3()
{
	int t = 1;
	c.store(t, std::memory_order_relaxed);
	d.store(2, std::memory_order_release);	//本原子操作前所有的写原子操作必须完成
}

void Thread4()
{
	//本原子操作必须完成才能执行之后的读原子操作
	while (d.load(std::memory_order_acquire) != 2)
		;
	std::cout << c.load(std::memory_order_relaxed) << std::endl;	//1
}

//快速退出：quick_exit与at_quick_exit
//对应于exit和atexit

//终止函数：
//异常退出：terminate和abort
//正常退出：exit		注册函数atexit
//			C++11新增：quick_exit		注册函数at_quick_exit
/*
	exit函数会正常调用自动变量的析构函数，并且还会调用atexit注册的函数，
		而注册函数被调用的顺序与注册的顺序刚好相反
	C++11中引入快速退出的quick_exit函数，该函数并不执行析构函数而只是使程序终止
*/
#define QUICK_EXIT_FLAG	
void OpenDevice(){ std::cout << "device is opened." << std::endl; }
void ResetDeviceStatus(){ std::cout << "device status is reset." << std::endl; }
void CloseDevice(){ std::cout << "device is closed." << std::endl; }

struct A{
	~A(){ std::cout << "Destruct A." << std::endl; }
};

int main()
{
	std::cout << "=========原子类型例1========" << std::endl;
	std::thread t1(Thread1);
	std::thread t2(Thread2);

	t1.join();
	t2.join();

	std::cout << "=========原子类型例2========" << std::endl;
	std::thread t3(Thread3);
	std::thread t4(Thread4);

	t3.join();
	t4.join();

#if defined(EXIT_FLAG)
	/*输出结果
		device is opened.
		device status is reset.
		device is closed.
	*/
	std::cout << "=========exit========" << std::endl;
	atexit(CloseDevice);
	atexit(ResetDeviceStatus);
	OpenDevice();
	exit(0);

#elif defined(QUICK_EXIT_FLAG)
	/*输出结果，其中不会调用a的析构函数
		device is closed.
	*/
	std::cout << "=========快速退出qucik_exit========" << std::endl;
	A a;
	at_quick_exit(CloseDevice);
	quick_exit(0);
#else

#endif
	return 0;
}