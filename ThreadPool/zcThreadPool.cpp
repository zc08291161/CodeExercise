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

/* 自动杀死一半的等待线程 TODO:后续更改为等待够一定时间之后，再杀死 */
void ZC_ThreadPool::TP_MonitorAutoKill()
{ 
	pthread_mutex_lock(&this->_mutex4FreeNum);
	int iKillNum = _iFreeNum/2;
	pthread_mutex_unlock(&this->_mutex4FreeNum);
	for (int i = 0; i<iKillNum; i++)
	{
		if (!pthread_cancel(this->_tpArray[_iCurrentNum-1-i]))
		{
			_iFreeNum--;		
			printf("pthread_cancle Loop(%d)TPNum(%d) Ok\n",i, _iCurrentNum-1-i);
		}
	}
	_iCurrentNum -= iKillNum;
	return;
}

void* ZC_ThreadPool::TP_MonitorHandle(void *args)
{
	ZC_ThreadPool *p=(ZC_ThreadPool *)args;

	while(p->_bTpAlive)
	{
		sleep(1);
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
	if (NULL == this->_tpArray)
	{
		printf("alloc fail");
		return -1;
	}
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










