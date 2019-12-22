/* 二叉树序列化反序列化，主要用于快速构建二叉树  */
#include "zcTreeNode.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
using namespace std;
/* 依靠前序遍历序列化二叉树，NULL的节点设置为"#" */
string SequenceTree(TreeNode *root)
{
	string Str;
	char buf[4] = { 0 };
	if (root == NULL)
	{
		Str += "#";
		return Str;
	}
	
	_itoa_s(root->iVal, buf, 10);
	Str += buf;
	Str += ",";
	Str += SequenceTree(root->leftChild);
	Str += ",";
	Str += SequenceTree(root->rightChild);
	return Str;
}


TreeNode* DesequenceTree(string str, int *pos)
{
	string str1 = str.substr(*pos);
	if ((str1.size() == 0) || (*pos >= str.size()))
	{
		return NULL;
	}
	TreeNode *root = new TreeNode();
	int tmpPos = str1.find(',');
	string tmpStr = str1.substr(0, tmpPos);
	if (tmpStr == "#")
	{
		root = NULL;
		*pos = *pos + 2;
		return root;
	}
	root->iVal = atoi(tmpStr.c_str());
	*pos = *pos + tmpPos+1;
	root->leftChild = DesequenceTree(str, pos);
	root->rightChild = DesequenceTree(str, pos);
	return root;
}



void testSequenceTree()
{
	string Str = "1,2,4,#,#,#,3,5,#,#,6,#,#";
	string StrTest;
	int pos = 0;

	TreeNode *root = DesequenceTree(Str, &pos);
	StrTest = SequenceTree(root);
	if (Str == StrTest)
	{
		cout << "testSequenceTree success";
	}
	return;





}