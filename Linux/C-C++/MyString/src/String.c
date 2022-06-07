#include "String.h"

int StrLen(const char *pStr)
{
	int nLen = -1;
	if(NULL != pStr)
	{
		nLen = 0;
		while('\0' != *pStr++)
		{
			nLen++;
		}
	}

	return nLen;
}

char* StrCpy(char *pStrDest, const char *pStrSrc)
{
	if(NULL != pStrDest && NULL != pStrSrc)
	{
		int i = 0;
		for(i = 0; '\0' != (pStrDest[i] = pStrSrc[i]); i++);
	}
	return pStrDest;
}

int StrCmp(const char *pStr1, const char *pStr2)
{
	if(NULL != pStr1 && NULL != pStr2)
	{
		int i = 0;
		for(i = 0; '\0' != pStr1[i] || '\0' != pStr2[i]; i++)
		{
			if(pStr1[i] != pStr2[i])
			{
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

char* StrCat(char *pStrDest, const char *pStrSrc)
{
	if(NULL != pStrDest && NULL != pStrSrc)
	{
		while('\0' != *pStrDest++);
		pStrDest--;

		while('\0' != *pStrSrc)
		{
			*pStrDest++ = *pStrSrc++;
		}
		*pStrDest = '\0';
	}

	return pStrDest;
}
