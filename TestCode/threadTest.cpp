#include"pch.h"
#include<stdio.h>
#define HAVE_STRUCT_TIMESPEC
#include<pthread.h>
#include<iostream>
using namespace std;

#pragma comment(lib,"x86/pthreadVC2.lib")

#define NUM_THREADS 5

//创建线程接口 pthread_create ；cout 线程不安全，输出与预期不一致。

//可多线程调用的函数 类上静态函数，常规函数。类上普通函数目前还没找到方法调用

//多线程调用带参数的函数 
//	对传入的参数进行强制类型转换，由无类型指针变为相应指针，然后再读取
//	循环调用时，不能使用临时循环变量i作为参数，因为可能主进程在i还未赋值时，线程已经开始跑啦！~,改为使用循环外数组。qest??。

//join的语义
//  join功能表示主程序可以等线程结束后再去做某事，实现了主程序和线程同步功能。
//设置线程可join的方法
//  通过pthread_create的第二个参数pthread_attr_t设置，注意这个参数先初始化再set，用完后还要destory 否则内存泄漏。
//  join的子线程可以通过pthread_exit设置返回状态；通过pthread_join返回


// 互斥锁 mutex-mutual exclusion 
// 类型：pthread_mutex_t
// 作用  互斥访问某一代码段
// 使用时机 出现同时写的情况，势必会需要锁机制； 
// 初始化 pthread_mutex_init
// 加锁/解锁 pthread_mutex_lock/pthread_mutex_unlock
// 释放资源 	pthread_mutex_destroy

// 信号量 
// 类型：pthread_cond_t
// 作用  结合互斥锁，互斥访问多个代码段
// 使用时机 出现同时写的情况，势必会需要锁机制； 
// 初始化 	pthread_cond_init
// wait/发送 pthread_cond_signal/pthread_mutex_unlock
// 释放资源 	pthread_cond_destroy


class Hello
{
public:

	static void* say_hello(void* args)//除了多了static关键字，别无异样；
	{
		//cout << "hello..." << endl;
		printf("hello...\n");
		return nullptr;
	}
};

void* say_hello1(void* args)
{
	int i = *((int*)args);//对传入的参数进行强制类型转换，由无类型指针变为整形数指针，然后再读取；
	//cout << "hello in " << i << endl;
	//pthread_mutex_lock(&sum_mutex);
	printf("hello...%d\n",i);
	//pthread_mutex_unlock(&sum_mutex);
	return nullptr;
}

int testmain1()
{
	pthread_t tids[NUM_THREADS];
	cout << "hello in main..." << endl;
	int indexes[NUM_THREADS];//用个数组来保存i的值，就不会变了
	int i = 0;
	for ( i = 0; i < NUM_THREADS; ++i)
	{
		indexes[i] = i;
		int ret = pthread_create(&tids[i], NULL, say_hello1, &i);//传入的时候必须强制转换为void* 类型，即无类型指针
		//cout << "Current pthread id =" << static_cast<>(tids[i]) << endl;//这里学会使用tids数组打印创建的进程id信息；
		if (ret != 0)
		{
			cout << "pthread_create error: error_code=" << ret << endl;
		}
	}

	pthread_exit(NULL);
	return 1;
}

void* say_hello2(void* args)
{
	cout << "hello in thread " << *((int *)args) << endl;
	int status = 10 + *((int *)args);//将参数加10
	pthread_exit((void*)status);//由于线程创建时候提供了joinable参数，这里可以在退出时添加退出的信息：status供主程序提取该线程的结束信息；
	return nullptr;
}

int testmain2()
{
	pthread_t tids[NUM_THREADS];
	int indexes[NUM_THREADS];

	pthread_attr_t attr;//要想创建时加入参数，先声明
	pthread_attr_init(&attr);//再初始化
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	//声明、初始化后第三步就是设置你想要指定线程属性参数，这个参数表明这个线程是可以join连接的，
	//join功能表示主程序可以等线程结束后再去做某事，实现了主程序和线程同步功能，这个深层理解必须通过图示才能解释；参阅其他资料吧
	for (int i = 0; i < NUM_THREADS; ++i)
	{
		indexes[i] = i;
		int ret = pthread_create(&tids[i], &attr, say_hello2, (void *)&(indexes[i]));//这里四个参数都齐全了，更多的配置仍需查阅资料；
		if (ret != 0)
		{
			cout << "pthread_create error: error_code=" << ret << endl;
		}
	}

	pthread_attr_destroy(&attr);//参数使用完了就可以销毁了，必须销毁哦，防止内存泄露；

	void *status;
	for (int i = 0; i < NUM_THREADS; ++i)
	{
		int ret = pthread_join(tids[i], &status);//前面创建了线程，这里主程序想要join每个线程后取得每个线程的退出信息status；
		if (ret != 0)
		{
			cout << "pthread_join error: error_code=" << ret << endl;
		}
		else
		{
			cout << "pthread_join get status: " << (long)status << endl;
		}
	}
	return 1;
}


int sum = 0;//定义个全局变量，让所有线程进行访问，这样就会出现同时写的情况，势必会需要锁机制；  
pthread_mutex_t sum_mutex;

void* say_hello3(void* args)
{
	pthread_mutex_lock(&sum_mutex);//修改sum就先加锁，锁被占用就阻塞，直到拿到锁再修改sum；  
	cout << "hello in thread " << *((int *)args) << endl;
	cout << "before sum is " << sum << " in thread " << *((int *)args) << endl;
	sum += *((int *)args);
	cout << "after sum is " << sum << " in thread " << *((int *)args) << endl;
	pthread_mutex_unlock(&sum_mutex);//完事后解锁，释放给其他线程使用;  

	pthread_exit(0);//退出随便扔个状态码  
	return nullptr;
}

int testmain3()
{
	pthread_t tids[NUM_THREADS];
	int indexes[NUM_THREADS];
	//下三句是设置线程参数没啥可说的  
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_mutex_init(&sum_mutex, NULL);//这句是对锁进行初始化，必须的；  

	for (int i = 0; i < NUM_THREADS; ++i)
	{
		indexes[i] = i;
		int ret = pthread_create(&tids[i], &attr, say_hello3, (void *)&(indexes[i]));//5个进程去你们去修改sum吧哈哈；  
		if (ret != 0)
		{
			cout << "pthread_create error: error_code=" << ret << endl;
		}
	}

	pthread_attr_destroy(&attr);//删除参数变量  

	void *status;
	for (int i = 0; i < NUM_THREADS; ++i)
	{
		int ret = pthread_join(tids[i], &status);
		if (ret != 0)
		{
			cout << "pthread_join error: error_code=" << ret << endl;
		}
	}

	cout << "finally sum is " << sum << endl;

	pthread_mutex_destroy(&sum_mutex);//注销锁，可以看出使用pthread内置变量神马的都对应了销毁函数，估计是内存泄露相关的吧；  
	return 1;
}
#define BOUNDARY 5
int tasks = 10;

pthread_mutex_t tasks_mutex;//因为两个线程要修改一个全局变量，需要互斥量；
pthread_cond_t tasks_cond;//因为两个线程间有条件关系：当tasks>5时，hello2处理它，处理一次减少1；反之hello1处理，直到tasks减为零；

void* say_hello4(void* args)//tasks>5处理函数
{
	pthread_t pid = pthread_self();//打印当前线程id便于跟踪
	//cout << "[" /*<< pid*/ << "] hello in thread " << *((int*)args) << endl;

	bool is_signaled = false;//随便一个标志位
	while (1)//无限循环
	{
		pthread_mutex_lock(&tasks_mutex);//要修改了，加锁
		if (tasks > BOUNDARY)//>5才修改
		{
			cout << "[" /*<< pid*/ << "] take task:  " << tasks << " in thread " << *((int*)args) << endl;
			--tasks;//减少1
		}
		else if (!is_signaled)
		{
			cout << "[" /*<< pid*/ << "] pthread_cond_signal in thread " << *((int*)args) << endl;
			pthread_cond_signal(&tasks_cond);//表明已经不是>5了告诉hello1进程去处理：发送信号；
			is_signaled = true;//表示信号已经发送了
		}
		pthread_mutex_unlock(&tasks_mutex);//操作完解锁

		if (tasks == 0) break;//必须等待tasks全部减为零即hello1完成操作，才跳出循环结束这个进程
	}
	return nullptr;
}
void* say_hello5(void* args)//<=5处理函数
{
	pthread_t pid = pthread_self();
	//cout << "[" /*<< pid */<< "] hello in thread " << *((int*)args) << endl;

	while (1)
	{
		pthread_mutex_lock(&tasks_mutex);
		if (tasks > BOUNDARY + 1)//如果>5说明需要hello2处理，那么该线程就需要等待
		{
			cout << "["/* << pid*/ << "] pthread_cond_wait in thread " << *((int*)args) << endl;
			pthread_cond_wait(&tasks_cond, &tasks_mutex);//等待信号量生效，当hello2发出信号，这里就跳出wait，执行后续；
		}
		else
		{
			cout << "[" /*<< pid*/ << "] take task:  " << tasks << " in thread " << *((int*)args) << endl;
			--tasks;//<=5就--
		}
		pthread_mutex_unlock(&tasks_mutex);

		if (tasks == 0) break;//为零时退出，同hello2一样
	}
	return nullptr;

}
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <math.h>
using std::vector;
using std::map;
class Solution {
public:
	string countAndSay(int n) {
		int next = 0;
		int cnt = 1;
		string s("1");
		string newS;
		for (int i = 0; i < n; i++)
		{
			size_t j = 0;
			for (; j < s.size()-1; ++j)
			{
				if ( s[j] == s[j+1])
				{
					cnt++;
				}
				else
				{
					newS.append(std::to_string(cnt));
					newS.append(1,s[j]);
					++j;
					cnt =1;
				}
			}
			newS.append(std::to_string(cnt));
			newS.append(1, s[j]);
			cnt = 1;
			s = newS;
			newS.clear();
		}

		return s;
	}
};
int testmain()
{
	vector<char > v2{ 'h', 'e', 'l', 'l', 'o'};
	Solution s;
	string xxs =  s.countAndSay(5);
	return 1;
	pthread_attr_t attr;//线程创建为joinable的，使得主进程可以和两个线程同步，两个线程完成工作退出后，主进程再退出；
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_mutex_init(&tasks_mutex, NULL);//初始化互斥量
	pthread_cond_init(&tasks_cond, NULL);//初始化条件信号量

	pthread_t tid1, tid2;//用于保存两个线程的id号
	int index1 = 1;
	int ret = pthread_create(&tid1, &attr, say_hello5, (void *)&index1);
	if (ret != 0)
	{
		cout << "pthread_create error: error_code=" << ret << endl;
	}

	int index2 = 2;
	ret = pthread_create(&tid2, &attr, say_hello4, (void *)&index2);
	if (ret != 0)
	{
		cout << "pthread_create error: error_code=" << ret << endl;
	}



	pthread_join(tid1, NULL);//连接两个线程
	pthread_join(tid2, NULL);

	pthread_attr_destroy(&attr);//该销毁的销毁
	pthread_mutex_destroy(&tasks_mutex);
	pthread_cond_destroy(&tasks_cond);

	//正常退出
}