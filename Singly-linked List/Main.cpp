#include "pch.h"
#include "MyList.h"

int main()
{
	NODE *pHead = NULL;

	cout << "Create" << endl;
	Create(&pHead);
	Show(pHead);

	cout << "Add" << endl;
	Add(pHead);
	Show(pHead);

	cout << "Insert" << endl;
	Insert(pHead, front, NULL);
	Show(pHead);

	cout << "Insert" << endl;
	Insert(pHead);
	Show(pHead);

	NODE *pPrev = NULL;
	cout << "Find  Insert" << endl;
	NODE *pTemp = Find(pHead, 12, &pPrev);
	Insert(pTemp);
	Show(pHead);

	cout << "----------------------After Delete---------------------" << endl;
	Delete(pTemp, pPrev);
	Show(pHead);

	cout << "----------------------After Modify---------------------" << endl;
	pTemp = Find(pHead, 45, &pPrev);
	Modify(pTemp, 999);
	Show(pHead);
}
