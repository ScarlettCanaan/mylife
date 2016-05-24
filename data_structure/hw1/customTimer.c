#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include "queue.h"

#ifndef TIME_DELAY 
#define TIME_DELAY 1
#endif

#ifndef TIME_DELAY_UNSEC
#define TIME_DELAY_UNSEC 0
#endif

#define STOP 43200
#define TICKER 60

#ifndef FILEPATH
#define FILEPATH0 "./testc1"
#define FILEPATH1 "./testc2"
#define FILEPATH2 "./testc3"
#define FILEPATH3 "./testc4"
#define FILEPATH4 "./testc5"
#endif

int ARRIVAL_RATE[5] = { 9, 1, 5, 1, 9 };
int SERVICE_RATE[5] = { 1, 9, 5, 1, 9 };

#ifndef MAXQUEUE
#define MAXQUEUE 10000
#endif

Queue queue[5];

int counter = 0;
char tbuffer[26];

// custom timer
void timer_run(int fd)
{
	fd_set rd;
	struct timeval tv;
	tv.tv_sec = TIME_DELAY;
	tv.tv_usec = TIME_DELAY_UNSEC;
	FD_ZERO(&rd);
	FD_SET(fd, &rd);
	fd_set tmp;
	while(FD_ISSET(fd, &rd))
	{
		tmp = rd;
		tv.tv_sec = TIME_DELAY;
		tv.tv_usec = TIME_DELAY_UNSEC;
		int record = select(fd+1, &tmp, NULL, NULL, &tv); 
		if (record == -1)
		{
			perror("select error\n");
		} else

		if (record == 0)
		{
			if (timeout_cb(0))
			return;
		}
	}
	return;
}

void showNowTime() {
	time_t now;
	char tbuffer[26];
	struct tm* tm_info;
	
	// get total second from 1970/01/01
	time(&now);
	// convert time_t to struct tm
	tm_info = localtime(&now);
	// convert struct tm to string assigned
	strftime(tbuffer, sizeof(tbuffer), "%Y/%m/%d %H:%M:%S", tm_info);
	
	printf("%s\n", tbuffer);
}

char* getNowTime() {
	time_t now;
	char *tbuf;
	
	tbuf = tbuffer;
	struct tm* tm_info;
	
	// get total second from 1970/01/01
	time(&now);
	// convert time_t to struct tm
	tm_info = localtime(&now);
	// convert struct tm to string assigned
	strftime(tbuffer, sizeof(tbuffer), "%Y/%m/%d %H:%M:%S", tm_info);
	
	//printf("%s\n", tbuffer);
	return tbuf;
}


int timeout_cb(int revents) {
	int i;
	for (i = 0; i < 5; ++i)
	{
		if ((rand() % 10) < ARRIVAL_RATE[i]) 
		{
			Insert(&queue[i], 1);
		}
		if ((rand() % 10) < SERVICE_RATE[i]) 
		{
			int a;
			Remove(&queue[i], &a);
		}

	}
	
	counter++;

	if (0 == counter % TICKER)
	{
		printf("write log data at ");
		showNowTime();
		writetoFile(FILEPATH0, queue[0]);
		writetoFile(FILEPATH1, queue[1]);
		writetoFile(FILEPATH2, queue[2]);
		writetoFile(FILEPATH3, queue[3]);
		writetoFile(FILEPATH4, queue[4]);
	}

	if(counter == STOP) 
		return 1;
	return 0;
}

void writetoFile(char* path, Queue queue) {
	FILE *pfile;
	if ( NULL != (pfile = fopen(path, "a")))
	{
		//char str[50];
		//char* streambuf = getNowTime();
		getNowTime();
		char* streambuf = tbuffer;
		//strcpy(str, streambuf);
		char split = ' ';
		char true = '1';
		char false = '0';
		char lf = '\n';
		char* szBuffer = (char *)malloc(sizeof(int) + 1);
		memset(szBuffer, 0, sizeof(int) + 1);
		sprintf(szBuffer, "%d", queue.cursize);
		//printf("%s\n", streambuf);

		fputs(streambuf,pfile);
		fputc(split,pfile); //
		

		fwrite(szBuffer, strlen(szBuffer), 1, pfile);//block
		free(szBuffer);
		fputc(split,pfile);
		if (isOverflow(&queue))
		{
			fputc(true, pfile);
		} else 
		{
			fputc(false, pfile);
		}
		fputc(lf,pfile);
		fclose(pfile);
	}	
}

int main(int argc, char const *argv[])
{
 	srand(time(NULL));
 	int i;
 	for (i = 0; i < 5; ++i)
 	{
 		Initial(&queue[i], MAXQUEUE);
 	}
 	timer_run(0);
	return 0;
}
