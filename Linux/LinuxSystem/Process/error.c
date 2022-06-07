/*************************************************************************
	> File Name: error.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月15日 星期一 08时33分12秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
	char *p = "qweasd";

	p[2] = "q";


	return 0;
}
