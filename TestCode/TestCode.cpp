// TestCode.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <stack>
#include <thread>
#include <string>
#include <algorithm>
using std::string;

void threadFun()
{
	std::cout << "thread Fun\n " << std::endl;
}
// 多线程中的引用参数，std::thread t(&threadFun1, std::ref(str));
void threadFun1(string& str)
{
	std::cout << "thread Fun\n " << str << std::endl;
	str = "new string";
}

void threadFun2(string str)
{
	std::cout << "thread Fun\n " << str << std::endl;
}

class MyFunctor
{
public:
	void operator() ()
	{
		std::cout << "Functor Thread \n";
	};

};

int testStack()
{
	std::stack<int> intStack;

	for (int i = 1; i != 10; ++i)
	{
		intStack.push(i);
	}
	std::stack<int> intStack1(intStack);

	std::cout << "stack size：" << intStack.size() << '\n';
	while (!intStack.empty())
	{
		std::cout << ' ' << intStack.top() << '\n';
		intStack.pop();
	}
	

	intStack1.emplace(77);
	std::cout << "stack1 size：" << intStack1.size() << '\n';
	while (!intStack1.empty())
	{
		std::cout << ' ' << intStack1.top() << '\n';
		intStack1.pop();
	}
	for (int i = 1; i != 10; ++i)
	{
		intStack.push(i);
	}
	intStack.swap(intStack1);
	std::cout << "stack size：" << intStack.size() << '\n';
	std::cout << "stack1 size：" << intStack1.size() << '\n';

	

	return 1;

}
#include <queue>
int testQueue()
{
	std::queue<int> intQueue;
	for (int i = 0; i != 10; ++i)
	{
		intQueue.push(i);
	}
	while (!intQueue.empty())
	{
		std::cout << intQueue.front() << " ";
		intQueue.pop();
	}
	std::cout << intQueue.size() << " ";

	for (int i = 0; i != 10; ++i)
	{
		intQueue.push(i);
	}
	while (!intQueue.empty())
	{
		std::cout << intQueue.back() << " ";
		intQueue.pop();
	}
	std::cout << intQueue.size() << " ";
	return 1;
}
int testReverse()
{
	std::string str("abcde");
	str.reserve(str.size());
	std::cout << str.c_str();
	return 1;
}

namespace classSpace {

}
#include<map>
#include<vector>
int main()
{
	int data[100];
	std::cout << sizeof(data) << std::endl;
	memset(data,0,sizeof(data));

}



//int  main()
//{
//	//MyFunctor functor;
//	//std::thread t(functor);
//	//std::thread t((MyFunctor()));
//	string str = "funcWithParam";
//
//	//std::thread t(&threadFun1, std::ref(str));
//	//std::thread t(&threadFun2, std::move(str));
//	std::thread t([]() {std::cout << " lambda thread \n" << std::endl; });
//	//std::thread t1 = std::move(t);
//	std::cout << "Main Thread\n " << str << std::endl;
//	//t.join();
//	t.detach();
//	
//	//std::cout << t.get_id() << std::endl;
//	//std::cout << t1.get_id() << std::endl; 
//	//std::cout << std::thread::hardware_concurrency()<< std::endl;
//	//if (t.joinable())
//	//{
//	//	t.join();
//	//}
//	//if (t1.joinable())
//	//{
//	//	t1.join();
//	//}
//	//std::cout << __cplusplus << std::endl;
//	return 0;
//}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
