#ifndef __MYMEMPOOL_H__
#define __MYMEMPOOL_H__

//�ڴ�ڵ���(��ǰ���ݽṹ��ʹ�õ���struct)
class CMemItem
{
public:
	CMemItem(){ this->prev = 0; this->next = 0; this->uiSize = 0; };
	CMemItem *prev;		//ǰ��ָ��
	CMemItem *next;		//����ָ��
	unsigned int uiSize;	//���ڵ����ʹ�õ��ڴ�Ĵ�С
	char *getBuf(){ return (char*)this + 12; };	//��������ʹ���ڴ���׵�ַ
};



//�ڴ�ع����࣬��������������Դ(�ڴ�)
class CMyMemPool
{
public:
	CMyMemPool(unsigned int uiSize);	//����һ������������������ٸ��ֽڵ��ڴ��
	~CMyMemPool();
public:
	char * MemAlloc(unsigned int uiSize);	//���ڴ������һ���ڴ�ڵ�
	void MemFree(char* pMem);		//�ͷ�һ���ڴ�ڵ�
private:
	char *pBuf;		//ָ�����뵽����Դ(�ڴ��)
	CMemItem *firstMemItem;	//��һ���ڴ�ڵ�
	unsigned int uiBufSize;	//�������뵽�Ķ��ڴ�صĴ�С
};

#endif //__MYMEMPOOL_H__
