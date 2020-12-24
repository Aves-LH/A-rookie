#include "pch.h"
#include "MyIO.h"

void ItoA(int nNum, char *pStr)
{
	if (NULL != pStr)
	{
		char szNum[255] = "";
		int i = 0;

		for (i = 0; 0 != nNum; i++)
		{
			szNum[i] = nNum % 10 + '0';
			nNum /= 10;
		}

		for (i = i - 1; i >= 0; i--, pStr++)
		{
			*pStr = szNum[i];
		}
		*pStr = '\0';
	}
}

void Print(const char *pFormat, ...)
{
	if (NULL != pFormat)
	{
		char *pTemp = (char *)&pFormat;
		pTemp += 4;

		while ('\0' != *pFormat)
		{
			if ('%' == *pFormat)
			{
				pFormat++;
				switch (*pFormat)
				{
				case 'c':
				{
					putchar(*pTemp);
					pTemp += 4;
				}
				break;
				case 'd':
				{
					char szBuf[255] = "";
					int nNum = 0;

					ItoA(*(int *)pTemp, szBuf);

					for (int i = 0; '\0' != szBuf[i]; i++)
					{
						putchar(szBuf[i]);
					}

					pTemp += 4;
				}
				break;
				case 's':
				{
					for (int i = 0; '\0' != (*(char **)pTemp)[i]; i++)
					{
						putchar((*(char **)pTemp)[i]);
					}

					pTemp += 4;
				}
				break;
				default:
				{
					pFormat--;
					putchar(*pTemp);
				}
				break;
				}
			}
			else
			{
				putchar(*pFormat);
			}
			pFormat++;
		}
	}
}

char* GetBuf(char *pBuf)
{
	if (NULL != pBuf)
	{
		//*pBuf = getchar();
		char *pTemp = pBuf;

		while (true)
		{
			*pTemp = getchar();
			if (' ' == *pTemp || '\n' == *pTemp)
			{
				break;
			}
			pTemp++;
		}
		*pTemp = '\0';
	}
	return pBuf;
}

int AtoI(int nRes, const char *pStr)
{
	if (NULL != pStr)
	{
		int nNum = 1;
		int nLen = StrLen(pStr);

		if ('-' == pStr[0])
		{
			for (int i = 1; '\0' != pStr[i]; i++)
			{
				nNum = pStr[i] - '0';
				for (int j = nLen - 2; j > 0; j--)
				{
					nNum *= 10;
				}
				nLen--;
				nRes += nNum;
			}
			return -nRes;
		}
		else
		{
			for (int i = 0; '\0' != pStr[i]; i++)
			{
				nNum = pStr[i] - '0';
				for (int j = nLen - 1; j > 0; j--)
				{
					nNum *= 10;
				}
				nLen--;
				nRes += nNum;
			}
			return nRes;
		}
	}
	return 0;
}

void Scanf(const char *pFormat, ...)
{
	if (NULL != pFormat)
	{
		char *pTemp = (char *)&pFormat;
		pTemp += 4;

		while ('\0' != *pFormat)
		{
			if ('%' == *pFormat)
			{
				pFormat++;
				switch (*pFormat)
				{
				case 'c':
				{
					**(char **)pTemp = getchar();
					pTemp += 4;
					getchar();
				}
				break;
				case 'd':
				{
					char szBuf[255] = "";
					int nNum = 0;
					char *pBuf = GetBuf(szBuf);

					**(int **)pTemp = AtoI(nNum, pBuf);

					pTemp += 4;
				}
				break;
				case 's':
				{
					GetBuf(*(char **)pTemp);

					pTemp += 4;
				}
				break;
				}
			}

			pFormat++;
		}
	}
}
