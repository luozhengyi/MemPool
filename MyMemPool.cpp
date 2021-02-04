#include "stdafx.h"
#include "MyMemPool.h"
#include "assert.h"
#include <string>

CMyMemPool::CMyMemPool(unsigned int uiSize)
{
	uiBufSize = uiSize;			//保存内存池的大小
	pBuf = new char[uiSize];	//申请内存池
	assert(pBuf);				//做一个断言，确保内存申请成功
	memset(pBuf, 0, uiSize);	//内存清空
	firstMemItem = 0;			//初始时没有申请任何节点
}

CMyMemPool::~CMyMemPool()
{
	if (pBuf)
		delete[] pBuf;
}

char * CMyMemPool::MemAlloc(unsigned int uiSize)
{
	if (firstMemItem == 0)	//第一次在内存池中申请内存节点
	{
		firstMemItem = (CMemItem *)pBuf;
		firstMemItem->prev = 0;
		firstMemItem->next = 0;
		firstMemItem->uiSize = uiSize;
		return firstMemItem->getBuf();	//内存节点的实际位置
	}
	else	//不是第一次申请，遍历是否有空隙可以插入节点
	{
		//首先判断内存池的首部是否可用
		if (pBuf + 12 + uiSize <= (char*)firstMemItem)
		{
			CMemItem *newMemItem = (CMemItem *)pBuf;

			newMemItem->uiSize = uiSize;
			newMemItem->prev = 0;
			newMemItem->next = firstMemItem;

			firstMemItem->prev = newMemItem;

			firstMemItem = newMemItem;
			return firstMemItem->getBuf();
		}

		CMemItem* temp = firstMemItem;
		while (temp->next != 0)
		{
			if (((char*)temp->next - 12 - uiSize) >= (temp->getBuf() + temp->uiSize))
			{
				CMemItem *newMemItem = (CMemItem *)(temp->getBuf() + temp->uiSize);
				newMemItem->uiSize = uiSize;
				newMemItem->prev = temp;
				newMemItem->next = temp->next;
				
				//这两句顺序不能反，否则出错
				temp->next->prev = newMemItem;
				temp->next = newMemItem;

				return newMemItem->getBuf();
			}
			temp = temp->next;
		}

		//没有空隙插入，直接在最后面创建节点
		CMemItem *newMemItem = 0;
		if ((temp->getBuf() + temp->uiSize + 12 + uiSize) <= (pBuf + uiBufSize))
		{

			//((char*)temp + 12 + temp->uiSize + 12 + uiSize)
			//(char*)(temp + 12 + temp->uiSize + 12 + uiSize)	两种写法差距很大
			newMemItem = (CMemItem*)(temp->getBuf() + temp->uiSize);
			newMemItem->uiSize = uiSize;
			newMemItem->next = 0;
			newMemItem->prev = temp;

			temp->next = newMemItem;

			return newMemItem->getBuf();
		}
		else	//内存池的内存不够分配了
			return 0;

	}

}

void CMyMemPool::MemFree(char* pMem)	//pMem 是指向内存节点中的可用内存的起始位置
{
	//释放内存，安全第一
	//首先 判断 pMem 是否是内存节点

	CMemItem *freeMemItem = (CMemItem*)(pMem - 12);
	
	if (freeMemItem == firstMemItem)	//摘除第一个内存节点
	{
		firstMemItem = firstMemItem->next;
		firstMemItem->prev = 0;
	}
	else
	{
		CMemItem *temp = firstMemItem;
		while (temp)
		{
			if (temp == freeMemItem)	//找到删除的节点
			{
				if (freeMemItem->next)	//删除中间节点
				{
					freeMemItem->prev->next = freeMemItem->next;
					freeMemItem->next->prev = freeMemItem->prev;
				}
				else  //删除最后一个节点
					freeMemItem->prev->next = 0;
			}
			temp = temp->next;
		}
	}
		

}