#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include "zclist.h"
#include<vector>

typedef  void(*TP_FUN)(void *args);

typedef struct Thread_ContrlBlock
{
	pthread_t *pth;
	unsigned int tid;
	bool      bIsFree;
}Th_cb_S;

class ZC_ThreadPool
{
	public:
		
	ZC_ThreadPool(int iMaxNum, int iDefaultNum, TP_FUN pFun, ZC_List  *gList);
	int TP_GetCurrentNum();
	void TP_ChangeCurrentNum(int iNum);
	int TP_Start();
	int TP_Destroy();
	void TP_MonitorAutoKill();
	void TP_MonitorAutoAdd(int iReqNum);
	static void* TP_MonitorHandle(void *args);
	static void* TP_WorkHandle(void *args);
	static void TP_CleanUp(void *args);
	int TP_FindThread8Tid(unsigned int tid);
	int TP_GetFreeTpNum();
	int TP_AddThread();
	
	private:
	int _iEnableMaxNum;
	int _iCurrentNum;
	int _iDefaultNum;
	int _iFreeNum;
	pthread_mutex_t _mutex4TpCb;
	TP_FUN pFun;
	ZC_List  *_gList;
	bool _bTpAlive;
	std::vector<Th_cb_S> vTh;
	pthread_t *_tpArray;
};



#endif
