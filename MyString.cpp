#include "MyString.h"

bool StrCmp(const char *pStr1, const char *pStr2)
{
	if(NULL != pStr1 && NULL != pStr2)
	{
		for(int i = 0; '\0' != pStr1[i] || '\0' != pStr2[i]; i++)
		{
			if(pStr1[i] != pStr2[i])
			{
				return false;
			}
		}
		return true;
	}

	return false;
}
