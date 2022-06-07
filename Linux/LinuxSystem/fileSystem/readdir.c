/*************************************************************************
	> File Name: readdir.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月12日 星期五 16时03分56秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char* argv[])
{
	DIR *pDir = opendir(".");
	struct dirent *pDirentry;
	int i = 0;

	if(NULL == pDir)
	{
		perror("open directory error!");
		exit(1);
	}

	while(NULL != (pDirentry = readdir(pDir)))
	{
		if(pDirentry->d_type != DT_DIR)
		{
			printf("%d file : %s\n", i, pDirentry->d_name);
		}
	}

	closedir(pDir);
	return 0;
}
