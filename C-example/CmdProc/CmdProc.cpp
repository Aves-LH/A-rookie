#include "CmdProc.h"

int Help(const CMD_MAP *pCmdMap)
{
	if(NULL != pCmdMap)
	{
		const CMD_MAP *pTemp = pCmdMap;
		while(NULL != pTemp->pCmdName)
		{
			cout<<pTemp->pCmdName<<"		:		"<<pTemp->pCmdInfo<<endl;
			pTemp++;
		}
	}

	return 0;
}

int CmdProc(const CMD_MAP *pCmdMap)
{
	if(NULL != pCmdMap)
	{
		const CMD_MAP *pTemp = NULL;
		char szBuf[255] = "";

		while(true)
		{
			cout<<"Command> ";
			pTemp = pCmdMap;
			cin>>szBuf;

			while(NULL != pTemp->pCmdName)
			{
				if(StrCmp(szBuf, pTemp->pCmdName))
				{
					if(StrCmp("Help", szBuf))
					{
						Help(pCmdMap);
						break;
					}
					pTemp->pFn();
					break;
				}
				pTemp++;
			}

			if(NULL == pTemp->pCmdName)
			{
				cout<<"Input Error!"<<endl;
			}
		}
	}

	return 0;
}
