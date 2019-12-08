#include "pch.h"
#include "MyList.h"

int ShowList(NODE *pHead)
{
	if (NULL == pHead)
	{
		return -1;
	}

	NODE *pTemp = pHead;
	while (NULL != pTemp)
	{
		cout << "*************************" << endl;
		cout << "Number:" << pTemp->nNum << endl;
		pTemp = pTemp->pNext;
	}

	return 0;
}

NODE* CreateNode(NODE **ppHead)
{
	NODE *pNew = new NODE;

	if (NULL != pNew)
	{
		cout << "Input: ";
		cin >> pNew->nNum;
		pNew->pNext = NULL;
		pNew->m_ppHead = ppHead;
	}

	return pNew;
}

int CreateList(NODE **ppHead)
{
	if (NULL == ppHead && NULL != *ppHead)
	{
		return -1;
	}

	*ppHead = CreateNode(ppHead);

	if (NULL == *ppHead)
	{
		return -1;
	}

	return 0;
}

int Add(NODE *pHead)
{
	if (NULL == pHead)
	{
		return -1;
	}

	NODE *pNew = CreateNode(pHead->m_ppHead);

	if (NULL != pNew)
	{
		NODE *pTemp = pHead;
		while (NULL != pTemp->pNext)
		{
			pTemp = pTemp->pNext;
		}
		pTemp->pNext = pNew;
	}

	return 0;
}

int Insert(NODE *pNode, MODE mode, NODE *pPrev)
{
	if (NULL == pNode)
	{
		return -1;
	}

	NODE *pNew = CreateNode(pNode->m_ppHead);

	if (NULL != pNew)
	{
		if (mode == front)
		{
			if (NULL == pPrev)
			{
				*(pNode->m_ppHead) = pNew;
			}
			else
			{
				pPrev->pNext = pNew;
			}
			pNew->pNext = pNode;
		}
		else
		{
			if (NULL == pNode->pNext)
			{
				pNode->pNext = pNew;
			}
			else
			{
				pNew->pNext = pNode->pNext;
				pNode->pNext = pNew;
			}
		}
	}

	return 0;
}

NODE* Find(NODE *pHead, int nFind, NODE **ppPrev)
{
	NODE *pTemp = NULL;
	if (NULL != pHead)
	{
		pTemp = pHead;

		while (NULL != pTemp)
		{
			if (nFind == pTemp->nNum)
			{
				break;
			}

			if (NULL != ppPrev)
			{
				*ppPrev = pTemp;
			}

			pTemp = pTemp->pNext;
		}
	}

	return pTemp;
}

int Delete(NODE *pNode, NODE *pPrev)
{
	if (NULL == pNode)
	{
		return -1;
	}

	if (NULL == pPrev)
	{
		*(pNode->m_ppHead) = pNode->pNext;
	}
	else
	{
		if (NULL == pNode->pNext)
		{
			pPrev->pNext = NULL;
		}
		else
		{
			pPrev->pNext = pNode->pNext;
		}
	}
	delete pNode;

	return 0;
}

int Modify(NODE *pNode, int nChange)
{
	if (NULL == pNode)
	{
		return -1;
	}

	pNode->nNum = nChange;

	return 0;
}