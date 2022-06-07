#include <stdio.h>
#include <stdarg.h>
#include "Print.h"


void ItoA(int nNum, char *pStr)
{
	if(NULL != pStr)
	{
		char szNum[255] = "";
		int i = 0;

		for(i = 0; 0 != nNum; i++)
		{
			szNum[i] = nNum % 10 + '0';
			nNum /= 10;
		}

		for(i = i - 1; i >= 0; i--, pStr++)
		{
			*pStr = szNum[i];
		}
		*pStr = '\0';
	}
}

int StrCpy(char *pStrDest, const char *pStrSrc)
{
	if(NULL != pStrDest || NULL != pStrSrc)
	{
		int i = 0;
		for(i = 0; '\0' != pStrSrc[i]; i++)
		{
			pStrDest[i] = pStrSrc[i];
		}
		pStrDest[i] = '\0';
	}	

	return 0;
}

void Print(const char *pFormat, ...)
{
	if(NULL != pFormat)
	{
		char *pTemp = NULL;
		va_list ap;
		va_start(ap, pFormat);

		while('\0' != *pFormat)
		{
			if('%' == *pFormat)
			{
				pFormat++;
				switch(*pFormat)
				{
					case 'c':
						{
							
							char ch = '\0';
							
							ch = va_arg(ap, int);
							va_end(ap);

							putchar(ch);
						}
						break;
					case 'd':
						{
							int nNum = 0;
							int i = 0;
							char szBuf[255] = "";
							nNum = va_arg(ap, int);
							
							ItoA(nNum, szBuf);

							for(i = 0; '\0' != szBuf[i]; i++)
							{
								putchar(szBuf[i]);
							}
							
						}
						break;
					case 's':
						{
							int i = 0;
							char szBuf[255] = "";
							StrCpy(szBuf, va_arg(ap, char *));

							for(i = 0; '\0' != szBuf[i]; i++)
							{
								putchar(szBuf[i]);
							}
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
		va_end(ap);
	}
}
