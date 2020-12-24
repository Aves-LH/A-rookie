#include <stdio.h>
#include <stdarg.h>
#include "MyIO.h"


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



char* GetBuf(char *pBuf)
{
    if(NULL != pBuf)
    {
        char *pTemp = pBuf;
        while(1)
        {
            *pTemp = getchar();
            if(' ' == *pTemp || '\n' == *pTemp)
            {
                break;
            }
            pTemp++;
        }
        *pTemp = '\0';
    }

    return pBuf;
}

int StrLen(const char *pStr)
{
    int nLen = -1;
    if(NULL != pStr)
    {
        nLen = 0;
        for(; '\0' != pStr[nLen]; nLen++);
    }

    return nLen;
}

int AtoI(int nRes, char *pStr)
{
    if(NULL != pStr)
    {
        int nNum = 1;
        int nLen = StrLen(pStr);
        int i = 0;
        int j = 0;

        if('-' == pStr[0])
        {
            for(i = 1; '\0' != pStr[i]; i++)
            {
                nNum = pStr[i] - '0';
                for(j = nLen - 2; j > 0; j--)
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
            for(i = 0; '\0' != pStr[i]; i++)
            {
                nNum = pStr[i] - '0';
                for(j = nLen - 1; j > 0; j--)
                {
                    nNum *= 10;
                }
                nLen--;
                nRes += nNum;
            }
            return nRes;
        }
        
    }
}

void Scanf(const char *pFormat, ...)
{
    if(NULL != pFormat)
    {
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
                        char *pTemp = va_arg(ap, char*);
                        *pTemp = getchar();
                    }
                    break;
                    case 'd':
                    {
                        int nNum = 0;
                        char *pBuf = NULL;
                        char szBuf[255] = "";
                        pBuf = GetBuf(szBuf);
                        int *pTemp = va_arg(ap, int*);
                        nNum = AtoI(nNum, pBuf);
                        *pTemp = nNum;
                    }
                    break;
                    case 's':
                    {
                        char *pTemp = va_arg(ap, char*);
                        GetBuf(pTemp);
                    }
                    break;
                }
            }
            pFormat++;
        }
    }
}
