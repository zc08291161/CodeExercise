#include"zclist.h"
#include<pthread.h>

ZC_List::ZC_List()
{
	pthread_cond_init(&this->_cond4list, NULL);
	pthread_mutex_init(&this->_mutex4list, NULL);
	this->_reqlist.clear();
}
void ZC_List::ZL_Wait4Cond()
{
	pthread_cond_wait(&this->_cond4list, &this->_mutex4list);
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

int ZC_List::ZL_GetReqListNum()
{
	return _reqlist.size();
}


ZC_Node_S* ZC_List::ZL_GetNode()
{
	return NULL;
}

