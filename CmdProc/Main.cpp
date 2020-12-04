#include "MyString.h"
#include "CmdProc.h"

void Create();
void Add();
void Insert();
void Delete();
void Modify();
void Exit();

CMD_MAP g_CmdMap[] = {	{"Create", "Create a list!", Create},
			{"Add", "Add data to list!", Add},
			{"Insert", "Insert data to list!", Insert},
			{"Delete", "Delete data from list!", Delete},
			{"Modify", "Modify data in list!", Modify},
			{"Exit", "Exit", Exit},
			{"Help", "Help", (PFN)Help},
			{NULL, NULL, NULL}};

int main()
{

	CmdProc(g_CmdMap);

	return 0;
}


void Create()
{
	cout<<"Create!!!"<<endl;
}

void Add()
{
	cout<<"Add!!!"<<endl;
}

void Insert()
{
	cout<<"Insert!!!"<<endl;
}

void Delete()
{
	cout<<"Delete!!!"<<endl;
}

void Modify()
{
	cout<<"Modify!!!"<<endl;
}

void Exit()
{
	exit(0);
}

