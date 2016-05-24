/*
 * libev_sample.c
 *
 * Author by Canaan
 * website:www.github.com/ScarlettCanaan
 * 
 */
#include <stdio.h>
#include <time.h>
#include <ev.h>
#include <stdlib.h>
#include "queue.h"

// stop timer at counter == 43200
#define STOP 43200
#define TICKER 60

#ifndef FILEPATH
#define FILEPATH0 "./test1"
#define FILEPATH1 "./test2"
#define FILEPATH2 "./test3"
#define FILEPATH3 "./test4"
#define FILEPATH4 "./test5"
#endif

int ARRIVAL_RATE[5] = { 9, 1, 5, 1, 9 };
int SERVICE_RATE[5] = { 1, 9, 5, 1, 9 };

#ifndef MAXQUEUE
#define MAXQUEUE 10000
#endif

void showNowTime();
char* getNowTime();

int counter = 0;
char tbuffer[26];

Queue queue[5];

void timeout_cb(struct ev_loop *loop, struct ev_timer *w, int revents){
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
		ev_timer_stop(loop, w);
}

int main() {

 	srand(time(NULL));
 	int i;
 	for (i = 0; i < 5; ++i)
 	{
 		Initial(&queue[i], MAXQUEUE);
 	}

	// use the default event loop
	struct ev_loop *loop = ev_default_loop(0);

	ev_timer timeout_watcher;
	
	// initialize an timer watcher
	// the event will occur after 3 sec, and repeat every 1 sec.
	ev_timer_init(&timeout_watcher, timeout_cb, 3, 1);
	// insert watcher to event loop
	ev_timer_start(loop, &timeout_watcher);
	
	// start event loop
	printf("Start loop at ");
	showNowTime();
	ev_run(loop, 0);
	printf("End loop\n");
	return 0;
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

	
	char *tbuf = tbuffer;
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

void writetoFile(char* path, Queue queue) {
	FILE *pfile;
	if ( NULL != (pfile = fopen(path, "a")))
	{
		char str[50];
		char* streambuf = tbuffer;
		getNowTime();
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
		fwrite(szBuffer, strlen(szBuffer), 1, pfile);
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