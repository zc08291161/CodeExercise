#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <unistd.h>
#include "zcThreadPool.h"


ZC_ThreadPool::ZC_ThreadPool(int iMaxNum, int iDefaultNum, TP_FUN pFun, ZC_List  *gList)
{
	_iEnableMaxNum = iMaxNum;
	_iDefaultNum = iDefaultNum;
	this->pFun = pFun;
	this->_gList = gList;
	_bTpAlive = true;
	return;
}


int ZC_ThreadPool::TP_GetCurrentNum()
{
	return _iCurrentNum;
}

void ZC_ThreadPool::TP_ChangeCurrentNum(int iNum)
{
	_iCurrentNum = iNum;
	return;
}

void* ZC_ThreadPool::TP_MonitorHandle(void *args)
{
	ZC_ThreadPool *p=(ZC_ThreadPool *)args;

	while(p->_bTpAlive)
	{
		printf("TP_MonitorHandle begin loop\n");
		sleep(1);
		
	}
}

void* ZC_ThreadPool::TP_WorkHandle(void *args)
{
	ZC_ThreadPool *p=(ZC_ThreadPool *)args;
	while(true)
	{
		/* lock zclist*/
		p->_gList->ZL_Mutex4listLock();
		
		if(0 == p->_gList->ZL_GetReqListNum())
		{
			printf("TP_WorkHandle TPNum(%d)\n", p->_iCurrentNum);
			/* add free num*/
			pthread_mutex_lock(&p->_mutex4FreeNum);
			p->_iFreeNum++;
			pthread_mutex_unlock(&p->_mutex4FreeNum);
			/* wait zclist req add */
			p->_gList->ZL_Wait4Cond();
		}
		else
		{
			do
			{
				ZC_Node_S *pZcNode;
				pZcNode = p->_gList->ZL_GetNode();
				if (NULL == pZcNode)
				{
					printf("No Req InList Err\n");
					break;
				}
				else
				{
					p->pFun((void*)pZcNode);
					
				}
			}while(0);
			
		}
		p->_gList->ZL_Mutex4listUnLock();
	}
}

int ZC_ThreadPool::TP_Start()
{
	pthread_t MonitorTP;
	pthread_create(&MonitorTP,0,TP_MonitorHandle,(void*)this);

	this->_tpArray = (pthread_t*)malloc(_iDefaultNum * sizeof(pthread_t));
	
	for(int i = 0; i<_iDefaultNum; i++)
	{
		_iCurrentNum++;
		pthread_create(&this->_tpArray[i], 0, TP_WorkHandle, (void*)this);
	}

	//printf("pthread_join begin");
	//pthread_join(MonitorTP, NULL);
	//printf("pthread_join end");
	sleep(1000);
	return 0;
}










