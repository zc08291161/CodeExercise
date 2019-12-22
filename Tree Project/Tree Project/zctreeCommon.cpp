/*二叉树的基本操作*/
#include "zcTreeNode.h"
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

TreeNode* MakeRoot(int val)
{
	TreeNode *tmp = new TreeNode();
	tmp->iVal = val;
	tmp->leftChild = NULL;
	tmp->rightChild = NULL;
	tmp->ParentNode = NULL;
	return tmp;
}

void InsertLeftChild(TreeNode *Parent, int val)
{
	TreeNode *tmp = new TreeNode();
	tmp->iVal = val;
	tmp->leftChild = NULL;
	tmp->rightChild = NULL;
	tmp->ParentNode = NULL;
	Parent->leftChild = tmp;
	return;
}

void InsertRightChild(TreeNode *Parent, int val)
{
	TreeNode *tmp = new TreeNode();
	tmp->iVal = val;
	tmp->leftChild = NULL;
	tmp->rightChild = NULL;
	tmp->ParentNode = NULL;
	Parent->rightChild = tmp;
	return;
}



