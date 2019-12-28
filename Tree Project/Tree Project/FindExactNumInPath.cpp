#include "zcTreeCommon.h"
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<vector>


void FindExactNumInPath(int iNum, TreeNode* root, vector<int> vInt, vector<vector<int>>* vVint)
{
	if (NULL == root)
	{
		return ;
	}
	if (root->iVal > iNum)
	{
		return ;
	}
	if (root->iVal == iNum)
	{ 
		vInt.push_back(root->iVal);
		vVint->push_back(vInt);
		vInt.pop_back();
		return ;
	}
	if (root->iVal < iNum)
	{
		vInt.push_back(root->iVal);
		FindExactNumInPath(iNum - root->iVal, root->leftChild, vInt, vVint);
		FindExactNumInPath(iNum - root->iVal, root->rightChild, vInt, vVint);

		vInt.pop_back();
	}
	return ;
}

void testFindExactNumInPath()
{
	TreeNode *root = NULL;
	int iPos = 0;
	string Str = "10,5,4,#,#,7,#,#,12,#,#";
	/*
			10
		5        12
	4      7  
	*/
	root = DesequenceTree(Str, &iPos);
	vector<int> vInt;
	vector<vector<int>> vVint;

	FindExactNumInPath(22, root, vInt, &vVint);
	vector<vector<int>> Vtmp = { { 10, 5, 7 }, { 10, 12 } };
	if (Vtmp == vVint)
	{
		cout << "testFindExactNumInPath success";
	}


}
