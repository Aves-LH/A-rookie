#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct telnum
{
   char name[7];
   char tel[13];
};

struct telnum num[100] = {{"", ""}};
int i;
int menu()
{
    int i=0;
    while(1)
    {
      printf("Please select a menu:\n");
      printf("       1: add a number.\n");
      printf("       2: show all the number.\n");
      printf("       3: select a number by name.\n");
      printf("       4: delete a number .\n");
      printf("       5: save to file .\n");
      printf("       6: load numbers from file .\n");
      printf("       0: exit .\n");
      printf("> ");
      scanf("%d",&i);
      if(i==0)
      {
         printf("Byebye.\n");
         exit(1);
      }
      if(i<1 || i>6)
      {
         printf("error.\n");
         continue;
      }
      else
      {
         return i;
      }
   }
}

void shownum(struct telnum t)
{
    printf("-----------------------------------\n");
    printf("Name:%s\n",t.name);
    printf("Tel:%s\n",t.tel);
}

struct telnum addnum()
{   
	struct telnum numtmp;
    char name[7],tel[13];
	printf("----------ADD-----------\n");
	printf("please input the name: ");
    scanf("%s",name);
	printf("please input the num: ");
	scanf("%s",tel);
    strcpy(numtmp.name,name);
	strcpy(numtmp.tel,tel);
	return(numtmp); 
}

void selectbyname()
{
    char name[7];
    int n=0,j;
    printf("----------SELECT----------\n");
	printf("please input the name : ");
    scanf("%s",name);
	for(j=0;j<i;j++)
	{
        if(strcmp(num[j].name,name)==0)
		{
			shownum(num[j]);
			n++;
		}
	}
	if(n==0)
	{
		printf("Don't find! Please check the list!");
	}
}

void delenum()
{
    char name[7];
    int j,n=0;
    printf("-----------DELETE-----------\n");
	printf("please input the name : ");
    scanf("%s",name);
	for(j=0;j<i;j++)
	{
        if(strcmp(num[j].name,name)==0)
		{
			n++;
			for(;j<i;j++)
	        {
		       num[j]=num[j+1];
	        }
	        i--;	
			break;
		}
	}
	if(n==0)
	{
		printf("Don't find! Please check the list!");
	}
}

void savetofile()
{
   int j,fd;
   char file[]="tel.txt";//此处修改为保存文件名
   extern int errno; 
   fd=open(file,O_RDWR | O_CREAT | O_TRUNC,0644);
   if(fd!=-1)
   {
      printf("opened file %s.\n",file);
   }
   else if(-1 == fd)
   {
      printf("cant't open file %s.\n",file);
      printf("errno：%d\n",errno);
      printf("ERR  ：%s\n",strerror(errno));
   }
   for(j=0;j<i;j++)
   {
      write(fd,num[j].name, 7);
      write(fd,num[j].tel, 13);
      //write(fd, "\n", 2);
   }
   printf("Save Successfully!\n");	
   close(fd);
}
void loadfromfile()
{
    int sfd,readnum,ofd;
	i=0;
    char name[7];
	char tel[13];
    char sfile[]="tel.txt";//此处修改为源文件名
    char ofile[]="tel1.txt";//此处修改为目的文件名
    extern int errno; 
    sfd=open(sfile,O_RDONLY);
    ofd=open(ofile,O_RDWR | O_CREAT ,0644);
    if(sfd!=-1)
    {
      printf("opened file %s.\n",sfile);
    }
    else
    {
      printf("cant't open file %s.\n",sfile);
      printf("errno：%d\n",errno);
      printf("ERR  ：%s\n",strerror(errno));
    }
    while((readnum=read(sfd,name,7))!=0&&readnum!=-1)
    {
       strcpy(num[i].name,name);
       write(ofd,num[i].name, strlen(num[i].name));
       read(sfd,tel,13);
       strcpy(num[i].tel,tel);
       write(ofd,num[i].tel, strlen(num[i].tel));
       i++;
   }
   close(sfd);
   close(ofd);
}


int main(int argc,char *argv[])
{ 
    int s,j, readnum;
   
    char name[7], tel[13];
    char file[]="/home/aves/Projects7672/LinuxScript/Embedded/chapter2/TEL/tel.txt";//此处修改为源文件名
    int fd = 0;
    fd = open(file, O_RDWR, 0600);
    
    if(fd!=-1)
    {
      printf("opened file %s.\n",file);
      while((readnum = read(fd, name, 7)) != 0 && readnum != -1)
      {
	    strcpy(num[i].name, name);
	    read(fd, tel, 13);
	    strcpy(num[i].tel, tel);
	    i++;
      }
    }
   /* else
    {
      printf("cant't open file %s.\n",file);
      printf("errno：%d\n",errno);
      printf("ERR  ：%s\n",strerror(errno));
    }*/

    while(1)
    {
    printf("\n- - - - Telphone Notebook. - - - \n");
	s = menu();
	switch(s)
	{
		case 1:
			{
				num[i]=addnum();
          			i++;	
			}
			break;
		case 2:
			{
				if(0 == strcmp(num[0].name, ""))
				{
					printf("Nothing! Please add information first!\n");
				}
				else
				{
					for(j=0;j<i;j++)
					{
				    	 shownum(num[j]);	
			 		}
				}
			}
			break;
		case 3:
			{
				selectbyname();
			}
			break;
		case 4:
			{
				delenum();	
			}
			break;
		case 5:
			{
				savetofile();
			}
			break;
		case 6:
			{
				loadfromfile();
			}
			break;
		default:
			break;
	}
   } 
}
