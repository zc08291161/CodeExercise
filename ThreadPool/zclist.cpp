#include"zclist.h"
#include<string>
#include<pthread.h>


ZC_List::ZC_List()
{
	pthread_cond_init(&this->_cond4list, NULL);
	pthread_mutex_init(&this->_mutex4list, NULL);
	pthread_mutex_init(&this->_mutex4cond, NULL);
	
	_reqSerioNo = 0;
	this->_reqlist.clear();
}
void ZC_List::ZL_Wait4Cond()
{
	pthread_mutex_lock(&this->_mutex4cond);
	pthread_cond_wait(&this->_cond4list, &this->_mutex4cond);
	pthread_mutex_unlock(&this->_mutex4cond);
	return;
}

void ZC_List::ZL_Mutex4CondUnlock()
{
	pthread_mutex_unlock(&this->_mutex4cond);
	return;
}

void ZC_List::ZL_BroadCastCond()
{
	pthread_cond_broadcast(&this->_cond4list);
	return;
}

void ZC_List::ZL_Mutex4listLock()
{
	pthread_mutex_lock(&this->_mutex4list);
	return;
}

void ZC_List::ZL_Mutex4listUnLock()
{
	pthread_mutex_unlock(&this->_mutex4list);
	return;
}	

int ZC_List::ZL_GetReqListNumSafe()
{
	int iTmp = 0;
	this->ZL_Mutex4listLock();
	iTmp = _reqlist.size();
	this->ZL_Mutex4listUnLock();
	return iTmp;
}

/* the seriNum must not same TODO::write the biggest Num in file*/
int ZC_List::GetZcNodeSeriNum()
{
	return _reqSerioNo++;
}

/* TODO:: value should be put*/
ZC_Node_S* ZC_List::ZL_MakeZcNode(std::string str)
{
	ZC_Node_S *ptr = (ZC_Node_S*)malloc(sizeof(ZC_Node_S));
	if (NULL == ptr)
	{
		return NULL;
	}
	ptr->iSeriNo = GetZcNodeSeriNum();
	ptr->str = str;
	return ptr;
}	

ZC_Node_S* ZC_List::ZL_GetNodeSafe()
{
	ZC_Node_S *ptr = NULL;
	ZL_Mutex4listLock();
	if (_reqlist.size() != 0)
	{
		ptr = &_reqlist.back();
		_reqlist.pop_back();
		printf("Node Num(%d), Node Str(%s) get\n", ptr->iSeriNo, ptr->str.c_str());
	}
	
	ZL_Mutex4listUnLock();
	
	return ptr;
}

void ZC_List::ZL_PutZcNode(ZC_Node_S* pZcNode)
{
	ZL_Mutex4listLock();
	printf("Node Num(%d), Node Str(%s) put\n", pZcNode->iSeriNo, pZcNode->str.c_str());
	_reqlist.push_back(*pZcNode);
	ZL_Mutex4listUnLock();
}	


