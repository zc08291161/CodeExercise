#include<stdio.h>
#include<stdlib.h>
#include "zcThreadPool.h"
#include <unistd.h>

void  zcTp_Test(void *args)
{
	ZC_Node_S *pZcNode = (ZC_Node_S*)args;
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	sleep(1);
	printf("ZcNode handle:: pid(%d)tid(%d)handle Seriono(%d) Str(%s)\n", 
			(unsigned int)pid, (unsigned int)tid, 
			pZcNode->iSeriNo, pZcNode->str.c_str());
	return;
}


int main()
{
	ZC_List  zcList1;
	ZC_Node_S *pZcNode = NULL;
	pZcNode = (ZC_Node_S*)malloc(sizeof(ZC_Node_S)*11);
	
	for(int i = 0; i<10; i++)
	{
		std::string Str = "ZcTest" + std::to_string(i);
		pZcNode[i] = *zcList1.ZL_MakeZcNode(Str);	
		zcList1.ZL_PutZcNode(&pZcNode[i]);
	}
	
	ZC_ThreadPool zcTp1(10,7,zcTp_Test,&zcList1);
	zcTp1.TP_Start();
	
	sleep(5);
	for(int i = 0; i<10; i++)
	{
		std::string Str = "ZcTest1" + std::to_string(i);
		pZcNode[i] = *zcList1.ZL_MakeZcNode(Str);	
		zcList1.ZL_PutZcNode(&pZcNode[i]);
	}
	
	sleep(10000);
	return 0;
}
