#include<stdio.h>
#include<stdlib.h>
#include "zcThreadPool.h"

void  zcTp_Test(void *args)
{
	printf("zcTp_Test begin");
	return;
}


int main()
{
	ZC_List  zcList1;
	ZC_ThreadPool zcTp1(10,7,zcTp_Test,&zcList1);
	zcTp1.TP_Start();

	return 0;
}
