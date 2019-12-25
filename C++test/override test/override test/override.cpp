#include<iostream>
#include<string>
using namespace std;

class animal
{
public:
	string name;

	void eat();
};

void animal::eat()
{
	cout << "animal eat"<<endl;
}
class pig : public animal
{
public:
	void eat(int iNum);

};

void pig::eat(int iNum)
{
	cout << "pig eat" << iNum << endl;
}

int main()
{
	pig *P = new pig();
	P->eat(11);
	animal *A = new pig();
	A->eat();

	cin.get();
	return 0;
}