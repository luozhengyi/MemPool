// MemPool.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MyMemPool.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CMyMemPool MemPool(100);

	char *p1 = MemPool.MemAlloc(10);
	char *p2 = MemPool.MemAlloc(10);
	char *p3 = MemPool.MemAlloc(10);
	char *p4 = MemPool.MemAlloc(10);
	char *p5 = MemPool.MemAlloc(10);

	MemPool.MemFree(p2);
	MemPool.MemAlloc(10);

	MemPool.MemFree(p1);
	MemPool.MemAlloc(10);

	return 0;
}

