#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include "zclist.h"

typedef  void(*TP_FUN)(void *args);


class ZC_ThreadPool
{
	public:
		
	ZC_ThreadPool(int iMaxNum, int iDefaultNum, TP_FUN pFun, ZC_List  *gList);
	int TP_GetCurrentNum();
	void TP_ChangeCurrentNum(int iNum);
	int TP_Start();
	int TP_Destroy();
	static void* TP_MonitorHandle(void *args);
	static void* TP_WorkHandle(void *args);
	private:
	int _iEnableMaxNum;
	int _iCurrentNum;
	int _iDefaultNum;
	int _iFreeNum;
	pthread_mutex_t _mutex4FreeNum;
	TP_FUN pFun;
	ZC_List  *_gList;
	bool _bTpAlive;
	pthread_t *_tpArray;
};



#endif
