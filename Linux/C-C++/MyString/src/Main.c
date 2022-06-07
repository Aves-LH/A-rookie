#include <stdio.h>

#include "String.h"

int main()
{
	char szBuf[255] = "qweasd";
	char sz[255] = "asdzxc";

	printf("szBuf: %s\n", szBuf);
	printf("sz: %s\n", sz);

	printf("-----------StrLen-------------\n");
	printf("The length of Hello World! is %d\n", StrLen("Hello World!"));

	printf("-----------StrCpy-------------\n");
	StrCpy(szBuf, sz);
	printf("szBuf: %s\n", szBuf);
	printf("sz: %s\n", sz);

	printf("-----------StrCmp--------------\n");
	printf("The result of compare between szBuf and sz is %d\n", StrCmp(szBuf, sz));

	printf("-----------StrCat--------------\n");
	StrCat(szBuf, sz);
	printf("szBuf: %s\n", szBuf);
	printf("sz: %s\n", sz);

	printf("Hello World!\n");

	return 0;
}
