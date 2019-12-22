#include"zcTreeNode.h"
#include<string>
using namespace std;

TreeNode* MakeRoot(int val);
void InsertLeftChild(TreeNode *Parent, int val);
void InsertRightChild(TreeNode *Parent, int val);

/* ≤‚ ‘–Ú¡–∑¥–Ú¡– */
extern string SequenceTree(TreeNode *root);
extern TreeNode* DesequenceTree(string str, int *pos);
extern void testSequenceTree();
extern void testFindComPaWiNoPaPtr();
