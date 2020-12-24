#include <iostream>
using namespace std;

#include "MyString.h"

typedef void (*PFN)();

struct CMD_MAP
{
	const char *pCmdName;
	const char *pCmdInfo;
	PFN pFn;
};

int Help(const CMD_MAP *pCmdMap);
int CmdProc(const CMD_MAP *pCmdMap);
