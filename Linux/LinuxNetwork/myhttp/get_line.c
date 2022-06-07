/*************************************************************************
	> File Name: get_line.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年07月03日 星期五 09时13分52秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int get_line(int cfd, char *buf, int size)
{
	int i = 0;
	char c = '\0';
	int n = 0;

	while((i < size - 1) && (c != '\n'))
	{
		n = recv(cfd, &c, 1, 0);
		if(n > 0)
		{
			if(c == '\r')
			{
				n = recv(cfd, &c, 1 , MSG_PEEK);
				if((n > 0) && (c == '\n'))
				{
					recv(cfd, &c, 1, 0);
				}
				else
				{
					c == '\n';
				}
			}
			buf[i] = c;
			i++;
		}
		else
		{
			c = '\n';
		}
	}
	buf[i] = '\0';

	if(-1 == n)
	{
		i = n;
	}

	return i;
}
