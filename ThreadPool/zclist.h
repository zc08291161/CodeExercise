#ifndef ZC_LIST_H
#define ZC_LIST_H
#include<list>
#include<pthread.h>

typedef struct ZC_NODE
{
	int iSeriNo;
	char string[20];
	void *value;
}ZC_Node_S;

 


class ZC_List
{
	public:
	ZC_List();
	void ZL_Wait4Cond();
	ZC_Node_S* ZL_GetNode();
	void ZL_Mutex4listLock();
	void ZL_Mutex4listUnLock();
	int ZL_GetReqListNum();
	
	private:
	pthread_mutex_t  _mutex4list;
	pthread_cond_t   _cond4list;
	std::list<ZC_Node_S> _reqlist;
};


	
#endif

