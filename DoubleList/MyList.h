#ifndef __MYLIST_H__
#define __MYLIST_H__

enum MODE{front, back};

struct NODE
{
	int m_nNum;

	NODE *pPrev;
	NODE *pNext;
};

NODE* Create();
int Add(NODE *pHead);
int Insert(NODE **ppHead, NODE *pPos, MODE mode = back);

NODE* Find(NODE *pHead, int nFind);

int Delete(NODE **ppHead, NODE *pPos);
int Clear(NODE **ppHead);

int Modify(NODE *pHead);

int Show(NODE *pHead);

#endif 