#ifndef __STRING_H__
#define __STRING_H__

#include <stdio.h>

int StrLen(const char *pStr);
char* StrCpy(char *pStrDest, const char *pStrSrc);
int StrCmp(const char *pStr1, const char *pStr2);
char* StrCat(char *pStrDest, const char *pStrSrc);

#endif
