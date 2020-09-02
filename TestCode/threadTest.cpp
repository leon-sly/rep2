#include"pch.h"
#include<stdio.h>
#define HAVE_STRUCT_TIMESPEC
#include<pthread.h>
#include<iostream>
using namespace std;

#pragma comment(lib,"x86/pthreadVC2.lib")

#define NUM_THREADS 5

//�����߳̽ӿ� pthread_create ��cout �̲߳���ȫ�������Ԥ�ڲ�һ�¡�

//�ɶ��̵߳��õĺ��� ���Ͼ�̬���������溯����������ͨ����Ŀǰ��û�ҵ���������

//���̵߳��ô������ĺ��� 
//	�Դ���Ĳ�������ǿ������ת������������ָ���Ϊ��Ӧָ�룬Ȼ���ٶ�ȡ
//	ѭ������ʱ������ʹ����ʱѭ������i��Ϊ��������Ϊ������������i��δ��ֵʱ���߳��Ѿ���ʼ������~,��Ϊʹ��ѭ�������顣qest??��

//join������
//  join���ܱ�ʾ��������Ե��߳̽�������ȥ��ĳ�£�ʵ������������߳�ͬ�����ܡ�
//�����߳̿�join�ķ���
//  ͨ��pthread_create�ĵڶ�������pthread_attr_t���ã�ע����������ȳ�ʼ����set�������Ҫdestory �����ڴ�й©��
//  join�����߳̿���ͨ��pthread_exit���÷���״̬��ͨ��pthread_join����


// ������ mutex-mutual exclusion 
// ���ͣ�pthread_mutex_t
// ����  �������ĳһ�����
// ʹ��ʱ�� ����ͬʱд��������Ʊػ���Ҫ�����ƣ� 
// ��ʼ�� pthread_mutex_init
// ����/���� pthread_mutex_lock/pthread_mutex_unlock
// �ͷ���Դ 	pthread_mutex_destroy

// �ź��� 
// ���ͣ�pthread_cond_t
// ����  ��ϻ�������������ʶ�������
// ʹ��ʱ�� ����ͬʱд��������Ʊػ���Ҫ�����ƣ� 
// ��ʼ�� 	pthread_cond_init
// wait/���� pthread_cond_signal/pthread_mutex_unlock
// �ͷ���Դ 	pthread_cond_destroy


class Hello
{
public:

	static void* say_hello(void* args)//���˶���static�ؼ��֣�����������
	{
		//cout << "hello..." << endl;
		printf("hello...\n");
		return nullptr;
	}
};

void* say_hello1(void* args)
{
	int i = *((int*)args);//�Դ���Ĳ�������ǿ������ת������������ָ���Ϊ������ָ�룬Ȼ���ٶ�ȡ��
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
	int indexes[NUM_THREADS];//�ø�����������i��ֵ���Ͳ������
	int i = 0;
	for ( i = 0; i < NUM_THREADS; ++i)
	{
		indexes[i] = i;
		int ret = pthread_create(&tids[i], NULL, say_hello1, &i);//�����ʱ�����ǿ��ת��Ϊvoid* ���ͣ���������ָ��
		//cout << "Current pthread id =" << static_cast<>(tids[i]) << endl;//����ѧ��ʹ��tids�����ӡ�����Ľ���id��Ϣ��
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
	int status = 10 + *((int *)args);//��������10
	pthread_exit((void*)status);//�����̴߳���ʱ���ṩ��joinable����������������˳�ʱ����˳�����Ϣ��status����������ȡ���̵߳Ľ�����Ϣ��
	return nullptr;
}

int testmain2()
{
	pthread_t tids[NUM_THREADS];
	int indexes[NUM_THREADS];

	pthread_attr_t attr;//Ҫ�봴��ʱ���������������
	pthread_attr_init(&attr);//�ٳ�ʼ��
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	//��������ʼ���������������������Ҫָ���߳����Բ��������������������߳��ǿ���join���ӵģ�
	//join���ܱ�ʾ��������Ե��߳̽�������ȥ��ĳ�£�ʵ������������߳�ͬ�����ܣ�������������ͨ��ͼʾ���ܽ��ͣ������������ϰ�
	for (int i = 0; i < NUM_THREADS; ++i)
	{
		indexes[i] = i;
		int ret = pthread_create(&tids[i], &attr, say_hello2, (void *)&(indexes[i]));//�����ĸ���������ȫ�ˣ��������������������ϣ�
		if (ret != 0)
		{
			cout << "pthread_create error: error_code=" << ret << endl;
		}
	}

	pthread_attr_destroy(&attr);//����ʹ�����˾Ϳ��������ˣ���������Ŷ����ֹ�ڴ�й¶��

	void *status;
	for (int i = 0; i < NUM_THREADS; ++i)
	{
		int ret = pthread_join(tids[i], &status);//ǰ�洴�����̣߳�������������Ҫjoinÿ���̺߳�ȡ��ÿ���̵߳��˳���Ϣstatus��
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


int sum = 0;//�����ȫ�ֱ������������߳̽��з��ʣ������ͻ����ͬʱд��������Ʊػ���Ҫ�����ƣ�  
pthread_mutex_t sum_mutex;

void* say_hello3(void* args)
{
	pthread_mutex_lock(&sum_mutex);//�޸�sum���ȼ���������ռ�þ�������ֱ���õ������޸�sum��  
	cout << "hello in thread " << *((int *)args) << endl;
	cout << "before sum is " << sum << " in thread " << *((int *)args) << endl;
	sum += *((int *)args);
	cout << "after sum is " << sum << " in thread " << *((int *)args) << endl;
	pthread_mutex_unlock(&sum_mutex);//���º�������ͷŸ������߳�ʹ��;  

	pthread_exit(0);//�˳�����Ӹ�״̬��  
	return nullptr;
}

int testmain3()
{
	pthread_t tids[NUM_THREADS];
	int indexes[NUM_THREADS];
	//�������������̲߳���ûɶ��˵��  
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_mutex_init(&sum_mutex, NULL);//����Ƕ������г�ʼ��������ģ�  

	for (int i = 0; i < NUM_THREADS; ++i)
	{
		indexes[i] = i;
		int ret = pthread_create(&tids[i], &attr, say_hello3, (void *)&(indexes[i]));//5������ȥ����ȥ�޸�sum�ɹ�����  
		if (ret != 0)
		{
			cout << "pthread_create error: error_code=" << ret << endl;
		}
	}

	pthread_attr_destroy(&attr);//ɾ����������  

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

	pthread_mutex_destroy(&sum_mutex);//ע���������Կ���ʹ��pthread���ñ�������Ķ���Ӧ�����ٺ������������ڴ�й¶��صİɣ�  
	return 1;
}
#define BOUNDARY 5
int tasks = 10;

pthread_mutex_t tasks_mutex;//��Ϊ�����߳�Ҫ�޸�һ��ȫ�ֱ�������Ҫ��������
pthread_cond_t tasks_cond;//��Ϊ�����̼߳���������ϵ����tasks>5ʱ��hello2������������һ�μ���1����֮hello1����ֱ��tasks��Ϊ�㣻

void* say_hello4(void* args)//tasks>5������
{
	pthread_t pid = pthread_self();//��ӡ��ǰ�߳�id���ڸ���
	//cout << "[" /*<< pid*/ << "] hello in thread " << *((int*)args) << endl;

	bool is_signaled = false;//���һ����־λ
	while (1)//����ѭ��
	{
		pthread_mutex_lock(&tasks_mutex);//Ҫ�޸��ˣ�����
		if (tasks > BOUNDARY)//>5���޸�
		{
			cout << "[" /*<< pid*/ << "] take task:  " << tasks << " in thread " << *((int*)args) << endl;
			--tasks;//����1
		}
		else if (!is_signaled)
		{
			cout << "[" /*<< pid*/ << "] pthread_cond_signal in thread " << *((int*)args) << endl;
			pthread_cond_signal(&tasks_cond);//�����Ѿ�����>5�˸���hello1����ȥ���������źţ�
			is_signaled = true;//��ʾ�ź��Ѿ�������
		}
		pthread_mutex_unlock(&tasks_mutex);//���������

		if (tasks == 0) break;//����ȴ�tasksȫ����Ϊ�㼴hello1��ɲ�����������ѭ�������������
	}
	return nullptr;
}
void* say_hello5(void* args)//<=5������
{
	pthread_t pid = pthread_self();
	//cout << "[" /*<< pid */<< "] hello in thread " << *((int*)args) << endl;

	while (1)
	{
		pthread_mutex_lock(&tasks_mutex);
		if (tasks > BOUNDARY + 1)//���>5˵����Ҫhello2������ô���߳̾���Ҫ�ȴ�
		{
			cout << "["/* << pid*/ << "] pthread_cond_wait in thread " << *((int*)args) << endl;
			pthread_cond_wait(&tasks_cond, &tasks_mutex);//�ȴ��ź�����Ч����hello2�����źţ����������wait��ִ�к�����
		}
		else
		{
			cout << "[" /*<< pid*/ << "] take task:  " << tasks << " in thread " << *((int*)args) << endl;
			--tasks;//<=5��--
		}
		pthread_mutex_unlock(&tasks_mutex);

		if (tasks == 0) break;//Ϊ��ʱ�˳���ͬhello2һ��
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
	pthread_attr_t attr;//�̴߳���Ϊjoinable�ģ�ʹ�������̿��Ժ������߳�ͬ���������߳���ɹ����˳������������˳���
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_mutex_init(&tasks_mutex, NULL);//��ʼ��������
	pthread_cond_init(&tasks_cond, NULL);//��ʼ�������ź���

	pthread_t tid1, tid2;//���ڱ��������̵߳�id��
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



	pthread_join(tid1, NULL);//���������߳�
	pthread_join(tid2, NULL);

	pthread_attr_destroy(&attr);//�����ٵ�����
	pthread_mutex_destroy(&tasks_mutex);
	pthread_cond_destroy(&tasks_cond);

	//�����˳�
}