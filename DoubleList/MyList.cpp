#include "pch.h"
#include "MyList.h"

NODE* CreateNode()
{
	NODE *pNew = new NODE;

	if (NULL != pNew)
	{
		cout << "Input : ";
		cin >> pNew->m_nNum;
		pNew->pPrev = NULL;
		pNew->pNext = NULL;
	}

	return pNew;
}

NODE* Create()
{
	return CreateNode();
}

int Add(NODE *pHead)
{
	if (NULL == pHead)
	{
		return -1;
	}

	NODE *pNew = CreateNode();

	if (NULL == pNew)
	{
		return -2;
	}

	NODE *pTemp = pHead;
	while (NULL != pTemp->pNext)
	{
		pTemp = pTemp->pNext;
	}
	pTemp->pNext = pNew;
	pNew->pPrev = pTemp;

	return 0;
}

int Insert(NODE **ppHead, NODE *pPos, MODE mode)
{
	if (NULL == *ppHead || NULL == ppHead)
	{
		return -1;
	}

	if (NULL == pPos)
	{
		return -3;
	}

	NODE *pNew = CreateNode();

	if (NULL == pNew)
	{
		return -1;
	}

	if (mode == front)
	{
		if (pPos == *ppHead)
		{
			*ppHead = pNew;
			pNew->pNext = pPos;
			pPos->pPrev = pNew;
		}
		else
		{
			pPos->pPrev->pNext = pNew;
			pNew->pPrev = pPos->pPrev;
			pPos->pPrev = pNew;
			pNew->pNext = pPos;
		}
	}
	else
	{
		if (NULL == pPos->pNext)
		{
			pPos->pNext = pNew;
			pNew->pPrev = pPos;
		}
		else
		{
			pPos->pNext->pPrev = pNew;
			pNew->pNext = pPos->pNext;
			pNew->pPrev = pPos;
			pPos->pNext = pNew;
		}
	}

	return 0;
}


NODE* Find(NODE *pHead, int nFind)
{
	NODE *pTemp = NULL;
	if (NULL != pHead)
	{
		pTemp = pHead;

		while (NULL != pTemp)
		{
			if (nFind == pTemp->m_nNum)
			{
				break;
			}
			pTemp = pTemp->pNext;
		}
	}

	return pTemp;
}


int Delete(NODE **ppHead, NODE *pPos)
{
	if (NULL == *ppHead || NULL == ppHead)
	{
		return -1;
	}

	if (NULL == pPos)
	{
		return -3;
	}

	if (pPos == *ppHead)
	{
		*ppHead = pPos->pNext;
		if (NULL != (*ppHead))
		{
			(*ppHead)->pPrev = NULL;
		}
	}
	else
	{
		if (NULL == pPos->pNext)
		{
			pPos->pPrev->pNext = NULL;
		}
		else
		{
			pPos->pPrev->pNext = pPos->pNext;
			pPos->pNext->pPrev = pPos->pPrev;
		}
	}

	return 0;
}

int Clear(NODE **ppHead)
{
	if (NULL == *ppHead || NULL == ppHead)
	{
		return -1;
	}

	NODE *pTemp = *ppHead;
	while (NULL != pTemp)
	{
		(*ppHead) = pTemp->pNext;
		delete pTemp;
		pTemp = *ppHead;
	}

	return 0;
}


int Modify(NODE *pHead)
{
	if (NULL == pHead)
	{
		return -1;
	}

	int nPos = 0;
	int nChange = 0;

	cout << "Where you want to modify: ";
	cin >> nPos;

	cout << "What to modify: ";
	cin >> nChange;

	NODE *pTemp = Find(pHead, nPos);
	pTemp->m_nNum = nChange;

	return 0;
}


int Show(NODE *pHead)
{
	if (NULL == pHead)
	{
		return -1;
	}

	NODE *pTemp = pHead;
	while (NULL != pTemp)
	{
		cout << "Data: " << pTemp->m_nNum << endl;
		pTemp = pTemp->pNext;
	}

	return 0;
}
