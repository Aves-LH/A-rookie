#include <stdio.h>
#include "cal.h"

int main()
{
	int a = 1;
	int b = 2;
	int c = 0;

	c = add(a, b);
	printf("a + b = %d\n", c);

	c = sub(a, b);
	printf("a - b = %d\n", c);

	return 0;
}
