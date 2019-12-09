// DoubleList.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "MyList.h"

int main()
{
	NODE *pHead = Create();
	Show(pHead);

	Add(pHead);
	Show(pHead);

	NODE *pTemp = Find(pHead, 12);
	Insert(&pHead, pTemp, front);
	Show(pHead);

	pTemp = Find(pHead, 34);
	Insert(&pHead, pTemp);
	Show(pHead);

	cout << "----------------after modify-----------------------" << endl;
	pTemp = Find(pHead, 34);
	Modify(pTemp);
	Show(pHead);

	cout << "----------------after delete-----------------------" << endl;
	pTemp = Find(pHead, 23);
	Delete(&pHead, pTemp);
	Show(pHead);

	cout << "----------------after clear-----------------------" << endl;
	Clear(&pHead);
	Show(pHead);

	return 0;
}

