#include "pch.h"
#include "MyIO.h"

char* GetString(char *pBuf)
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

int AtoI(int nSum, char *pStr)
{
	if (NULL != pStr)
	{
		int nLen = strlen(pStr);
		int nNum2 = nLen;
		int nNum = 0;

		if ('-' == pStr[0])
		{
			for (int i = 1; i < nNum2 && nLen > 0; i++, nLen--)
			{
				int nNum1 = 1;
				for (int j = 1; j < nLen - 1; j++)
				{
					nNum1 *= 10;
				}
				nNum = (pStr[i] - '0') * nNum1;
				nSum += nNum;
			}
		}
		else
		{
			for (int i = 0; i < nNum2 && nLen > 0; i++, nLen--)
			{
				int nNum1 = 1;
				for (int j = 1; j < nLen; j++)
				{
					nNum1 *= 10;
				}
				nNum = (pStr[i] - '0') * nNum1;
				nSum += nNum;
			}
		}
	}

	if ('-' == pStr[0])
	{
		return -nSum;
	}
	else
	{
		return nSum;
	}
}



void Scanf(const char *pFormat, ...)
{
	if (NULL != pFormat)
	{
		char **pTemp = (char **)&pFormat;
		pTemp++;


		while ('\0' != *pFormat)
		{
			if ('%' == *pFormat)
			{
				pFormat++;
				switch (*pFormat)
				{
				case 'd':
				{
					char szBuf[20] = "";
					char *pBuf = NULL;

					int nNum = 0;
					pBuf = GetString(szBuf);

					**(int **)pTemp = AtoI(nNum, pBuf);

					pTemp++;
				}
				break;
				case 'c':
				{
					**(char **)pTemp = getchar();

					pTemp++;
				}
				break;
				case 's':
				{
					GetString(*pTemp);

					pTemp++;
				}
				break;
				}
			}

			pFormat++;
		}
	}
}

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
		const char **pTemp = &pFormat;
		pTemp++;

		while ('\0' != *pFormat)
		{
			if ('%' == *pFormat)
			{
				pFormat++;
				switch (*pFormat)
				{
				case 'c':
				{
					putchar(**pTemp);
					pTemp++;
				}
				break;
				case 'd':
				{
					char szBuf[20] = "";
					int nNum = *(int *)pTemp;

					ItoA(nNum, szBuf);

					for (int i = 0; '\0' != szBuf[i]; i++)
					{
						putchar(szBuf[i]);
					}

					pTemp++;
				}
				break;
				case 's':
				{
					for (int i = 0; '\0' != (*pTemp)[i]; i++)
					{
						putchar((*pTemp)[i]);
					}

					pTemp++;
				}
				break;
				default:
				{
					pFormat--;
					putchar(*pFormat);
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
