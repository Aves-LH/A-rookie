// SingleList.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "MyList.h"

int main()
{
	NODE *pHead = NULL;

	CreateList(&pHead);
	ShowList(pHead);

	Add(pHead);
	ShowList(pHead);

	Insert(pHead, front, NULL);
	ShowList(pHead);

	Insert(pHead);
	ShowList(pHead);

	NODE *pPrev = NULL;
	NODE *pTemp = Find(pHead, 12, &pPrev);
	Insert(pTemp);
	ShowList(pHead);

	cout << "----------------------After Delete---------------------" << endl;
	Delete(pTemp, pPrev);
	ShowList(pHead);

	cout << "----------------------After Modify---------------------" << endl;
	pTemp = Find(pHead, 45, &pPrev);
	Modify(pTemp, 999);
	ShowList(pHead);

	return 0;
}


