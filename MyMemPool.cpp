#include "stdafx.h"
#include "MyMemPool.h"
#include "assert.h"
#include <string>

CMyMemPool::CMyMemPool(unsigned int uiSize)
{
	uiBufSize = uiSize;			//�����ڴ�صĴ�С
	pBuf = new char[uiSize];	//�����ڴ��
	assert(pBuf);				//��һ�����ԣ�ȷ���ڴ�����ɹ�
	memset(pBuf, 0, uiSize);	//�ڴ����
	firstMemItem = 0;			//��ʼʱû�������κνڵ�
}

CMyMemPool::~CMyMemPool()
{
	if (pBuf)
		delete[] pBuf;
}

char * CMyMemPool::MemAlloc(unsigned int uiSize)
{
	if (firstMemItem == 0)	//��һ�����ڴ���������ڴ�ڵ�
	{
		firstMemItem = (CMemItem *)pBuf;
		firstMemItem->prev = 0;
		firstMemItem->next = 0;
		firstMemItem->uiSize = uiSize;
		return firstMemItem->getBuf();	//�ڴ�ڵ��ʵ��λ��
	}
	else	//���ǵ�һ�����룬�����Ƿ��п�϶���Բ���ڵ�
	{
		//�����ж��ڴ�ص��ײ��Ƿ����
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
				
				//������˳���ܷ����������
				temp->next->prev = newMemItem;
				temp->next = newMemItem;

				return newMemItem->getBuf();
			}
			temp = temp->next;
		}

		//û�п�϶���룬ֱ��������洴���ڵ�
		CMemItem *newMemItem = 0;
		if ((temp->getBuf() + temp->uiSize + 12 + uiSize) <= (pBuf + uiBufSize))
		{

			//((char*)temp + 12 + temp->uiSize + 12 + uiSize)
			//(char*)(temp + 12 + temp->uiSize + 12 + uiSize)	����д�����ܴ�
			newMemItem = (CMemItem*)(temp->getBuf() + temp->uiSize);
			newMemItem->uiSize = uiSize;
			newMemItem->next = 0;
			newMemItem->prev = temp;

			temp->next = newMemItem;

			return newMemItem->getBuf();
		}
		else	//�ڴ�ص��ڴ治��������
			return 0;

	}

}

void CMyMemPool::MemFree(char* pMem)	//pMem ��ָ���ڴ�ڵ��еĿ����ڴ����ʼλ��
{
	//�ͷ��ڴ棬��ȫ��һ
	//���� �ж� pMem �Ƿ����ڴ�ڵ�

	CMemItem *freeMemItem = (CMemItem*)(pMem - 12);
	
	if (freeMemItem == firstMemItem)	//ժ����һ���ڴ�ڵ�
	{
		firstMemItem = firstMemItem->next;
		firstMemItem->prev = 0;
	}
	else
	{
		CMemItem *temp = firstMemItem;
		while (temp)
		{
			if (temp == freeMemItem)	//�ҵ�ɾ���Ľڵ�
			{
				if (freeMemItem->next)	//ɾ���м�ڵ�
				{
					freeMemItem->prev->next = freeMemItem->next;
					freeMemItem->next->prev = freeMemItem->prev;
				}
				else  //ɾ�����һ���ڵ�
					freeMemItem->prev->next = 0;
			}
			temp = temp->next;
		}
	}
		

}