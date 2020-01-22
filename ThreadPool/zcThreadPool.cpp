#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <unistd.h>
#include "zcThreadPool.h"


ZC_ThreadPool::ZC_ThreadPool(int iMaxNum, int iDefaultNum, TP_FUN pFun, ZC_List  *gList)
{
	_iEnableMaxNum = iMaxNum;
	_iDefaultNum = iDefaultNum;
	_iFreeNum = 0;
	_iCurrentNum = 0;
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

/* auto kill half waiting thread TODO::kill only if wait for sometime already */
#if 0
void ZC_ThreadPool::TP_MonitorAutoKill()
{ 
	pthread_mutex_lock(&this->_mutex4FreeNum);
	int iKillNum = _iFreeNum/2;
	pthread_mutex_unlock(&this->_mutex4FreeNum);
	for (int i = 0; i<iKillNum; i++)
	{
		printf("try to kill thread(%d)", (unsigned int)this->_tpArray[_iCurrentNum-1-i]);
		if (!pthread_cancel(this->_tpArray[_iCurrentNum-1-i]))
		{
			_iFreeNum--;		
			printf("pthread_cancle Loop(%d)TPNum(%d) Ok\n",i, _iCurrentNum-1-i);
		}
	}
	_iCurrentNum -= iKillNum;
	return;
}
#endif

void ZC_ThreadPool::TP_MonitorAutoKill()
{ 
	printf("try to kill thread(%d)\n", (unsigned int)this->_tpArray[2]);
	if (!pthread_cancel(this->_tpArray[2]))
	{
		_iFreeNum--;		
		printf("pthread_cancle Loop(%d)TPNum(%d) Ok\n",2, 2);
	}
	return;
}


void* ZC_ThreadPool::TP_MonitorHandle(void *args)
{
	ZC_ThreadPool *p=(ZC_ThreadPool *)args;

	while(p->_bTpAlive)
	{
		sleep(1);
		if (p->_gList->ZL_GetReqListNumSafe() != 0)
		{
			printf("Monitor BroadCast\n");
			p->_gList->ZL_BroadCastCond();
		}
		
		if (p->_iFreeNum > 0)
		{
			p->TP_MonitorAutoKill();
		}
		
	}
}

void* ZC_ThreadPool::TP_WorkHandle(void *args)
{
	ZC_ThreadPool *p=(ZC_ThreadPool *)args;
	while(true)
	{
		if(0 == p->_gList->ZL_GetReqListNumSafe())
		{
			pid_t pid;
			pthread_t tid;
			pid = getpid();
			tid = pthread_self();
			printf("pid(%d)tid(%d)wait \n", (unsigned int)pid, (unsigned int)tid);
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
				pZcNode = p->_gList->ZL_GetNodeSafe();
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
	}
}


int ZC_ThreadPool::TP_Start()
{
	pthread_t MonitorTP;
	pthread_create(&MonitorTP,0,TP_MonitorHandle,(void*)this);

	this->_tpArray = (pthread_t*)malloc(_iDefaultNum * sizeof(pthread_t));
	if (NULL == this->_tpArray)
	{
		printf("alloc fail");
		return -1;
	}
	for(int i = 0; i<_iDefaultNum; i++)
	{
		_iCurrentNum++;
		pthread_create(&this->_tpArray[i], 0, TP_WorkHandle, (void*)this);
		printf("thread tid(%d)create\n", (unsigned int)this->_tpArray[i]);
	}

	//printf("pthread_join begin");
	//pthread_join(MonitorTP, NULL);
	//printf("pthread_join end");

	return 0;
}










