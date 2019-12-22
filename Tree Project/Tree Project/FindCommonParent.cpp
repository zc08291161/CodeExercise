#include "zcTreeCommon.h"
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

/*获取最近的祖先，没有父亲指针*/
int FindcommonPaWithNoPaPtr(int iVal1, int iVal2)
{
	

}

void testFindComPaWiNoPaPtr()
{
	TreeNode *root = NULL;
	int iPos = 0;
	string Str = "1,2,4,#,#,5,#,6,#,#,3,7,#,#,3";
	root = DesequenceTree(Str, &iPos);
	int tmp = 0;
	tmp = FindcommonPaWithNoPaPtr(4, 6);

	if (tmp == 2)
	{
		cout << "testFindComPaWiNoPaPtr succ";
	}
}

