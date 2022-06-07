/*************************************************************************
	> File Name: myhttpd.c
	> Author: amoscykl
	> Mail: amoscykl@163.com 
	> Created Time: 2020年06月30日 星期二 22时08分21秒
 ************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

int get_line(int cfd, char *buf, int size)
{
	int i = 0;
	char c = '\0';
	int n = 0;

	while((i < (size - 1)) && (c != '\n'))
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
					c = '\n';
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

const char* get_file_type(const char *file)
{
	char *dot = NULL;

	dot = strrchr(file, '.');
	printf("dot : %s\n", dot);
	if(NULL == dot)
	{
		return "text/plain;charset=utf-8";
	}
	else if(!strcmp(dot, ".html") || !strcmp(dot, ".htm"))
	{
		return "text/html;charset=utf-8";
	}
	else if(!strcmp(dot, ".jpg") || !strcmp(dot, ".jpeg"))
	{
		return "image/jpeg";
	}
	else if(!strcmp(dot, ".png"))
	{
		return "image/png";
	}
	else if(!strcmp(dot, ".gif"))
	{
		return "image/gif";
	}
	else if(!strcmp(dot, ".wav"))
	{
		return "audio/wav";
	}
	else if(!strcmp(dot, ".avi"))
	{
		return "video/x-msvideo";
	}
	else if(!strcmp(dot, ".mov"))
	{
		return "video/quicktime";
	}
	else if(!strcmp(dot, ".mp3"))
	{
		return "audio/mpeg";
	}

	return "text/plain;charset=utf-8";

}

int init_listen_socket(int port)
{
	int lfd;
	struct sockaddr_in serv_addr;

	lfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == lfd)
	{
		perror("socket error");
		exit(1);
	}

	int opt = 1;
	if(-1 == setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt)))
	{
		perror("setsockopt error");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	if(-1 == bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
	{
		perror("bind error");
		exit(1);
	}

	if(-1 == listen(lfd, 128))
	{
		perror("listen error");
		exit(1);
	}

	return lfd;
}

void do_accept(int lfd, int epfd)
{
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	struct epoll_event tep;
	int cfd;
	int ret = 0;

	clnt_addr_size = sizeof(clnt_addr);
	cfd = accept(lfd, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
	if(-1 == cfd)
	{
		perror("accept error");
		exit(1);
	}
	int flag = fcntl(cfd, F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(cfd, F_SETFL, flag);
	printf("%d client connected!\n", cfd);

	tep.events = EPOLLIN | EPOLLET;
	tep.data.fd = cfd;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &tep);
	if(-1 == ret)
	{
		perror("epoll_ctl error");
		exit(1);
	}

	return;
}

void disconnected(int cfd, int epfd)
{
	int ret = 0;

	ret = epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, NULL);
	if(-1 == ret)
	{
		perror("epoll delete error");
		exit(1);
	}
	close(cfd);

	return;
}

void send_response(int cfd, int signo, const char *disp, const char *type, int len)
{
	char response_buf[BUFSIZ] = "";

	sprintf(response_buf, "HTTP/1.1 %d %s\r\n", signo, disp);
	sprintf(response_buf + strlen(response_buf), "Content-Type: %s\r\n", type);
	sprintf(response_buf + strlen(response_buf), "Content-Length: %d\r\n", len);
	sprintf(response_buf + strlen(response_buf), "\r\n");

	send(cfd, response_buf, strlen(response_buf), 0);
	return;
}

void send_error(int cfd, int signo, const char *disp)
{
	char error_buf[BUFSIZ] = "";

	sprintf(error_buf, "HTTP/1.1 %d %s\r\n", signo, disp);
	sprintf(error_buf + strlen(error_buf), "Content-Type: text/html\r\n");
	sprintf(error_buf + strlen(error_buf), "Content-Length: -1\r\n");
	sprintf(error_buf + strlen(error_buf), "Connection: close\r\n");
	sprintf(error_buf + strlen(error_buf), "\r\n");
	send(cfd, error_buf, strlen(error_buf), 0);

	memset(error_buf, 0, sizeof(error_buf));
	sprintf(error_buf, "<html><head><title>%d %s</title></head>\n", signo, disp);
	sprintf(error_buf + strlen(error_buf), "<body><h4 align = \"center\">%d %s</h4></body>\n", signo, disp);
	sprintf(error_buf + strlen(error_buf), "<hr>\n<body>\n</html>\n");
	send(cfd, error_buf, strlen(error_buf), 0);

	return;
}

void send_file(int cfd, const char *file)
{
	int ret = 0;
	int n = 0;
	int sum = 0;
	int fd = open(file, O_RDONLY);
	if(-1 == fd)
	{
		perror("open error");
		exit(1);
	}

	char buf[4096] = "";
	while(1)
	{	
		n = read(fd, buf, sizeof(buf));
		if(-1 == n)
		{
			if((errno == EAGAIN) || (errno == EINTR))
			{
				continue;
			}
		}
		else if(0 == n)
		{
			break;
		}	
		else if(n > 0)
		{
			ret = send(cfd, buf,n , 0);
			sum += ret;
			if(-1 == ret)
			{
				if((errno == EAGAIN) || (errno == EINTR) || (errno == ECONNRESET))
				{
					continue;
				}
				else
				{
					exit(1);
				}
			}
		}
	}	
	printf("send file success! %d\n", sum);
	close(fd);

	return;
}

void request_handler(int cfd, const char *filename)
{
	struct stat stat_buf;
	int ret = 0; 
	ret = stat(filename, &stat_buf);
	if(-1 == ret)
	{
		send_error(cfd, 404, "Not Found");
	}

	if(S_ISREG(stat_buf.st_mode))
	{
		//send_response(cfd, 200, "Ok", "text/plain; charset-iso-8859-1", stat_buf.st_size);
		send_response(cfd, 200, "Ok", get_file_type(filename), stat_buf.st_size);
		send_file(cfd, filename);
	}

}

void do_read(int cfd, int epfd)
{
	char line[BUFSIZ] = "";
	
	int n = get_line(cfd, line, BUFSIZ);
	if(0 == n)
	{
		printf("%d client disconnected!\n", cfd);
		disconnected(cfd, epfd);
	}
	
	char method[256] = "";
	char file[256] = "";
	char protocol[256] = "";

	sscanf(line, "%[^ ] %[^ ] %[^ ]", method, file, protocol);
	printf("method : %s , filename : %s , protocol : %s\n", method, file, protocol);

	while(1)
	{
		char buf[1024];
		n = get_line(cfd, buf, sizeof(buf));
		if('\n' == buf[0] || -1 == n)
		{
			break;
		}
	}

	if(0 == strncasecmp(method, "GET", 3))
	{
		char *pfile = file+1;
		request_handler(cfd, pfile);
	}	
	return;
}

void epoll_run(int port)
{
	int epfd;
	int lfd;
	int i = 0;
	struct epoll_event tep, ep[1024];
	int ret = 0;

	lfd = init_listen_socket(port);

	epfd = epoll_create(1024);
	if(-1 == epfd)
	{
		perror("epoll_create error");
		exit(1);
	}
	
	int flag = fcntl(lfd, F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(lfd, F_SETFL, flag);

	tep.events = EPOLLIN;
	tep.data.fd = lfd;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &tep);
	if(-1 == ret)
	{
		perror("epoll_ctl error");
		exit(1);
	}
	while(1)
	{
		ret = epoll_wait(epfd, ep, 1024, -1);
		if(-1 == ret)
		{
			perror("epoll wait error");
			exit(1);
		}

		for(i = 0; i < ret; i++)
		{
			if(!(ep[i].events & EPOLLIN))
			{
				continue;
			}
			if(lfd == ep[i].data.fd)
			{
				do_accept(lfd, epfd);
			}
			else
			{
				do_read(ep[i].data.fd, epfd);
			}
		}
	}
	return;
}

int main(int argc, char* argv[])
{
	int ret = 0;

	if(3 != argc)
	{
		printf("Usage : %s <port> <path>\n", *argv);
		exit(1);
	}

	ret = chdir(argv[2]);
	if(-1 == ret)
	{
		perror("chdir error");
		exit(1);
	}
	
	epoll_run(atoi(argv[1]));

	return 0;
}
