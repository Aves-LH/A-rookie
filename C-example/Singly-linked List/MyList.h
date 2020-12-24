#ifndef __MYLIST_H__
#define __MYLIST_H__

#include "pch.h"

enum MODE{front, back};

struct NODE
{
	int nNum;
	NODE **ppHead;
	
	NODE *pNext;
};

NODE* CreateNode(NODE **ppHead);
int Create(NODE **ppHead);
int Add(NODE *pHead);
int Insert(NODE *pNode, MODE mode = back, NODE *pPrev = NULL);

NODE* Find(NODE *pHead, int nFind, NODE **ppPrev);
int Delete(NODE *pNode, NODE *pPrev);
int Modify(NODE *pNode, int nChange);

int Show(NODE *pHead);
#endif 
