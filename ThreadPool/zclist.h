#ifndef ZC_LIST_H
#define ZC_LIST_H
#include<list>
#include<pthread.h>
#include<atomic>
#include<string>

typedef struct ZC_NODE
{
	int iSeriNo;
	std::string str;
	void *value;
}ZC_Node_S;




class ZC_List
{
	public:
	ZC_List();
	void ZL_Wait4Cond();
	ZC_Node_S* ZL_GetNodeSafe();
	void ZL_Mutex4listLock();
	void ZL_Mutex4listUnLock();
	int ZL_GetReqListNumSafe();
	int GetZcNodeSeriNum();
	ZC_Node_S* ZL_MakeZcNode(std::string str);
	void ZL_PutZcNode(ZC_Node_S* pZcNode);
	void ZL_BroadCastCond();

	
	private:
	pthread_mutex_t  _mutex4list;
	pthread_mutex_t  _mutex4cond;
	pthread_cond_t   _cond4list;
	std::list<ZC_Node_S> _reqlist;
	std::atomic<long int> _reqSerioNo;
};


	
#endif

