#include "zcTreeCommon.h"
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<list>
using namespace std;
list<int> gList;

int findFirstParentInList(list<int> Lint1, list<int> Lint2)
{
	int iSize1 = Lint1.size();
	int iSize2 = Lint2.size();
	if (iSize1 > iSize2)
	{
		for (int i = 0; i < iSize1 - iSize2; i++)
		{
			Lint1.pop_back();
		}
	}
	else
	{
		for (int i = 0; i < iSize2 - iSize1; i++)
		{
			Lint2.pop_back();
		}
	}
	while (Lint1.size() != 0)
	{
		if (Lint1.back() == Lint2.back())
		{
			return Lint1.back();
		}
		else
		{
			Lint1.pop_back();
			Lint2.pop_back();
		}
	}
}

/* 获取二叉树中含有特定值得路径 */
bool findValuePath(int iVal, TreeNode* root)
{
	if (root == NULL)
	{
		return false;
	}
	else
	{
		if (root->iVal == iVal)
		{
			gList.push_back(root->iVal);
			return true;
		}
		else
		{
			gList.push_back(root->iVal);
			bool IsFind = (findValuePath(iVal, root->leftChild) || findValuePath(iVal, root->rightChild));
			if (IsFind)
			{
				return true;
			}
			else
			{
				gList.pop_back();
				return false;
			}
		}
	}
}
/*获取最近的祖先，没有父亲指针 都是假设整棵树里面的各个值只有一个 */
int FindcommonPaWithNoPaPtr(int iVal1, int iVal2, TreeNode* root)
{
	gList.clear();
	bool IsFind = 0;
	IsFind = findValuePath(iVal1, root);
	if (IsFind == 0)
	{
		return -1;
	}
	list<int> gTmp1 = gList;
	gList.clear();
	IsFind = findValuePath(iVal2, root);
	if (IsFind == 0)
	{
		return -1;
	}
	list<int> gTmp2 = gList;
	return findFirstParentInList(gTmp1, gTmp2);
}

void testFindComPaWiNoPaPtr()
{
	TreeNode *root = NULL;
	int iPos = 0;
	/*
		    1
		2         3
	  4   5     7   9
	        6
	*/
	string Str = "1,2,4,#,#,5,#,6,#,#,3,7,#,#,9,#,#";
	root = DesequenceTree(Str, &iPos);
	int tmp = 0;
	tmp = FindcommonPaWithNoPaPtr(4, 6, root);

	if (tmp == 2)
	{
		cout << "testFindComPaWiNoPaPtr succ \n";
	}
}

