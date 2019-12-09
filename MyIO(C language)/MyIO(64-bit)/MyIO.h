#ifndef __MYIO_H__
#define __MYIO_H__


void ItoA(int nNum, char *pStr);
int StrCpy(char *pStrDest, const char *pStrSrc);
void Print(const char *pFormat, ...);


char* GetBuf(char *pBuf);
int AtoI(int nRes, char *pStr);
int StrLen(const char *pStr);
void Scanf(const char *pFormat, ...);

#endif 
