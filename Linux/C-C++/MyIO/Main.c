#include <stdio.h>
#include "Print.h"
#include "Scanf.h"

int main()
{
	char ch = '\0';
    int nNum = 0;
    char szBuf[255] = "";

    //Scanf("%c", &ch);
    //Print("char: %c\n", ch);

    //Scanf("%d", &nNum);
    //Print("d: %d\n", nNum);

    Scanf("%s", szBuf);
    Print("s: %s\n", szBuf);

	return 0;
}