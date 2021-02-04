#ifndef __MYMEMPOOL_H__
#define __MYMEMPOOL_H__

//内存节点类(以前数据结构中使用的是struct)
class CMemItem
{
public:
	CMemItem(){ this->prev = 0; this->next = 0; this->uiSize = 0; };
	CMemItem *prev;		//前向指针
	CMemItem *next;		//后向指针
	unsigned int uiSize;	//本节点可以使用的内存的大小
	char *getBuf(){ return (char*)this + 12; };	//返回真正使用内存的首地址
};



//内存池管理类，负责管理申请的资源(内存)
class CMyMemPool
{
public:
	CMyMemPool(unsigned int uiSize);	//传递一个参数，表明申请多少个字节的内存池
	~CMyMemPool();
public:
	char * MemAlloc(unsigned int uiSize);	//从内存池申请一个内存节点
	void MemFree(char* pMem);		//释放一个内存节点
private:
	char *pBuf;		//指向申请到的资源(内存池)
	CMemItem *firstMemItem;	//第一个内存节点
	unsigned int uiBufSize;	//保存申请到的堆内存池的大小
};

#endif //__MYMEMPOOL_H__
