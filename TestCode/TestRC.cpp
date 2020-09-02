#include "pch.h"
#include <functional>
#include <iostream>
using std::cout;
using std::endl;
class ScopeGuard
{
public:
	explicit ScopeGuard(std::function<void()> onExitScope)
		: onExitScope_(onExitScope), dismissed_(false)
	{ }

	~ScopeGuard()
	{
		if (!dismissed_)
		{
			onExitScope_();
		}
	}

	void Dismiss()
	{
		dismissed_ = true;
	}

private:
	std::function<void()> onExitScope_;
	bool dismissed_;

private: // noncopyable
	ScopeGuard(ScopeGuard const&);
	ScopeGuard& operator=(ScopeGuard const&);
};

#define SCOPEGUARD_LINENAME_CAT(name, line) name##line
#define SCOPEGUARD_LINENAME(name, line) SCOPEGUARD_LINENAME_CAT(name, line)

#define ON_SCOPE_EXIT(callback) ScopeGuard SCOPEGUARD_LINENAME(EXIT, __LINE__)(callback)

void test()
{
	const int arrLen = 10;
	int * pint = new int[arrLen];
/*	ON_SCOPE_EXIT(
		[]
	{
		cout << " de " << endl;
	}
	)*/;

	ScopeGuard tt(
		[&]()->int
	{
		//delete[]pint;
		cout << " de " << endl;
		return 1;
	});
	//tt.Dismiss();
	ON_SCOPE_EXIT(
		[&]()->int
		{
		delete[]pint; 
		cout << " de " << endl; 
		return 1;
		}
	);
	return ;
}

class MyScopeGuard
{
public:
	MyScopeGuard(std::function<void()> func) :m_pfunc(func),m_dismiss(false) {};
	~MyScopeGuard();
	void dismiss()
	{
		m_dismiss = false;
	}
private:
	MyScopeGuard& operator=( MyScopeGuard const&);
	MyScopeGuard(const MyScopeGuard&);

	private:
		std::function<void()>  m_pfunc;
		bool m_dismiss;
};


MyScopeGuard::~MyScopeGuard()
{
	if (m_dismiss)
	{
		m_pfunc();
	}
}

void testMyGuard()
{
	///int *pInt = new int[10];
	MyScopeGuard  MyScopeGuard111([&] {cout << "MyScopeGuard" << endl; });
	MyScopeGuard111.dismiss();;
}
typedef void(*pFunc)(void);
class MyScopeGuard1
{
public:
	MyScopeGuard1(pFunc ptr) :m_pfunc(ptr) {};
	~MyScopeGuard1();

private:
	MyScopeGuard1& operator=(MyScopeGuard1 const&);
	MyScopeGuard1(const MyScopeGuard&);

private:
	pFunc m_pfunc;

};


MyScopeGuard1::~MyScopeGuard1()
{
	m_pfunc();
}
void fun1(void)
{
	cout << "testMyGuard2" << endl;
};
void testMyGuard2()
{
	///int *pInt = new int[10];

	MyScopeGuard1  MyScopeGuard777(fun1);
}

//int main()
//{
//	testMyGuard();;
//}