#include<stdio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;
class A
{
public:
	int a;

};

class B :public A
{
public:
	int b[5];
};

int main1()
{
	B b ;
	A *a = static_cast<A*> (&b);
	cout << a << '\n' << &b;
	cout << (void *)reinterpret_cast<A *>(&b) << endl;
	system("pause");
	return 0;
}

class C : public A, public B
{

};

int main111()
{

	C c;
	cout << (void *)&c << endl;
	cout << (void *)reinterpret_cast<B *>(&c) << endl;

	cout << (void *)static_cast<B *>(&c) << endl;
	system("pause");
	return 0;
}

int main()
{

	C c;
	cout << (void *)&c << endl;
	cout << (void *)reinterpret_cast<B *>(&c) << endl;

	cout << (void *)static_cast<B *>(&c) << endl;
	system("pause");
	return 0;
}
