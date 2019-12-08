#ifndef __MYLIST_H__
#define __MYLIST_H__

enum MODE{front, back};

struct NODE
{
	int nNum;
	NODE **m_ppHead;

	NODE *pNext;
};

int ShowList(NODE *pHead);
NODE* CreateNode(NODE **ppHead);
int CreateList(NODE **ppHead);
int Add(NODE *pHead);
int Insert(NODE *pNode,MODE mode = back, NODE *pPrev = NULL);
NODE* Find(NODE *pHead, int nFind, NODE **ppPrev);
int Delete(NODE *pNode, NODE *pPrev);
int Modify(NODE *pNode, int nChange);

#endif 