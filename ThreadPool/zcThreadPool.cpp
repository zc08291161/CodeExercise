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

int ZC_ThreadPool::TP_GetFreeTpNum()
{
	int iTpNum = 0;
	int iFreeNum = 0;
	/* */
	pthread_mutex_lock(&this->_mutex4FreeNum);
	iTpNum = this->vTh.size();
	for(int i = 0; i<iTpNum; i++)
	{
		if (this->vTh[i].bIsFree)
		{
			iFreeNum++;
		}
	}
	
	pthread_mutex_unlock(&this->_mutex4FreeNum);
	return iFreeNum;
}


void ZC_ThreadPool::TP_CleanUp(void *args)
{
	pthread_t tid;
	tid = pthread_self();

	ZC_ThreadPool *p=(ZC_ThreadPool *)args;
	printf("TP_CleanUp In tid(%d)clean \n", (unsigned int)tid);
	p->_gList->ZL_Mutex4CondUnlock();
	
	return;
}

/* auto kill half waiting thread TODO::kill only if wait for sometime already */
void ZC_ThreadPool::TP_MonitorAutoKill()
{ 
	int iFreeNum = TP_GetFreeTpNum();
	int iKillNum = iFreeNum/2;
	
	for (int i = 0; i<iKillNum; i++)
	{
		/*this lock is for vector vTh pop*/
		pthread_mutex_lock(&this->_mutex4FreeNum);
		Th_cb_S *pTh_Cb = &(this->vTh.back());
		printf("try to kill thread(%d)FreeNum(%d)CurrentNum(%lu)\n", 
			pTh_Cb->tid,iFreeNum,this->vTh.size());
		if (!pthread_cancel(*pTh_Cb->pth))
		{		
			printf("pthread_cancle thread(%d) Ok\n",pTh_Cb->tid);
		}
		this->vTh.pop_back();
		pthread_mutex_unlock(&this->_mutex4FreeNum);
		
		free(pTh_Cb->pth);
	}
	_iCurrentNum -= iKillNum;
	return;
}

#if 0
void ZC_ThreadPool::TP_MonitorAutoKill()
{ 
	/*
	for (int i = 0; i<6; i++)
	{
		printf("try to kill thread(%d)", (unsigned int)this->_tpArray[i]);
		if (!pthread_cancel(this->_tpArray[i]))
		{		
			printf("pthread_cancle Loop(%d) Ok\n",i);
		}
	}
	*/
	/*
	printf("try to kill thread(%d)\n", (unsigned int)this->_tpArray[0]);
	if (!pthread_cancel(this->_tpArray[0]))
	{		
		printf("pthread_cancle Loop(%d) Ok\n",0);
	}
	*/
	printf("try to kill thread(%d)\n", (unsigned int)this->_tpArray[1]);
	if (!pthread_cancel(this->_tpArray[1]))
	{		
		printf("pthread_cancle Loop(%d) Ok\n",1);
	}

	return;
}
#endif

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
		
		if (p->TP_GetFreeTpNum() > 0)
		{
			p->TP_MonitorAutoKill();
		}
	}
}


int ZC_ThreadPool::TP_FindThread8Tid(unsigned int tid)
{
	int iLoop = this->vTh.size();
	for(int i = 0; i<iLoop; i++)
	{
		if (this->vTh[i].tid == tid)
		{
			return i;
		}
	}
	return -1;
}


void* ZC_ThreadPool::TP_WorkHandle(void *args)
{
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();

	ZC_ThreadPool *p=(ZC_ThreadPool *)args;
	pthread_cleanup_push(TP_CleanUp, (void*)args);
	
	while(true)
	{
		if(0 == p->_gList->ZL_GetReqListNumSafe())
		{
			printf("pid(%d)tid(%d)wait \n", (unsigned int)pid, (unsigned int)tid);
			/* modify free state */
			pthread_mutex_lock(&p->_mutex4FreeNum);
			int iThread = p->TP_FindThread8Tid(tid);
			p->vTh[iThread].bIsFree = true;
			pthread_mutex_unlock(&p->_mutex4FreeNum);
			
			/* wait zclist req add */
			p->_gList->ZL_Wait4Cond();
		}
		else
		{
			do
			{
				/* modify free state*/
				pthread_mutex_lock(&p->_mutex4FreeNum);
				int iThread = p->TP_FindThread8Tid(tid);
				p->vTh[iThread].bIsFree = false;
				pthread_mutex_unlock(&p->_mutex4FreeNum);
				
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
	pthread_cleanup_pop(0);
}


int ZC_ThreadPool::TP_Start()
{
	pthread_t MonitorTP;
	pthread_create(&MonitorTP,0,TP_MonitorHandle,(void*)this);
	
	for(int i = 0; i<_iDefaultNum; i++)
	{
		_iCurrentNum++;
		/* alloc thread and thread control block */
		pthread_t *pthread = (pthread_t*)malloc(sizeof(pthread_t));
		if (NULL == pthread)
		{
			printf("pthread alloc fail");
			return -1;
		}
		
		pthread_create(pthread, 0, TP_WorkHandle, (void*)this);
		Th_cb_S *pTh_Cb = (Th_cb_S*)malloc(sizeof(Th_cb_S));
		if (NULL == pTh_Cb)
		{
			printf("pTh_Cb alloc fail");
			return -1;
		}
		
		pTh_Cb->bIsFree = 0;
		pTh_Cb->tid = (unsigned int)(*pthread);
		pTh_Cb->pth = pthread;
		this->vTh.push_back(*pTh_Cb);
		
		printf("thread tid(%d)create\n", (unsigned int)(*pthread));
	}
	
	
	//printf("pthread_join begin");
	//pthread_join(MonitorTP, NULL);
	//printf("pthread_join end");

	return 0;
}










