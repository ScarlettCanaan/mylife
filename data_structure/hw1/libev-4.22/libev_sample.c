/*
 * libev_sample.c
 * 
 * Copyright MWNL lab. 2016
 * Author by Canaan
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
#define FILEPATH "test"
#endif

#ifndef ARRIVAL_RATE
#define ARRIVAL_RATE 9
#endif

#ifndef SERVICE_RATE
#define SERVICE_RATE 1
#endif

#ifndef MAXQUEUE
#define MAXQUEUE 10000
#endif

void showNowTime();
char* getNowTime();

int counter = 0;

Queue* queue;

void timeout_cb(struct ev_loop *loop, struct ev_timer *w, int revents){
	printf("timeout_cb at ");
	showNowTime();

	if ((rand() % 10) < ARRIVAL_RATE)
	{
		Insert(queue, 1);
	}
	if ((rand() % 10) < SERVICE_RATE)
	{
		int a;
		Remove(queue, &a);
	}
	
	counter ++;

	if (0 == counter % 60)
	{
		FILE *pfile;
		if ( NULL != (pfile = fopen(FILEPATH, "a+")))
		 {
		 	//char streambuf[26] = getNowTime();

			fwrite(getNowTime(), 27, 1, pfile);
			fwrite(' ', 1, 1, pfile);
			fwrite(queue->cursize, 4, 1, pfile);
			fwrite(' ', 1, 1, pfile);
			if (isOverflow(queue))
			{
				fwrite('1', 1, 1, pfile);
			} else 
			{
				fwrite('0', 1, 1, pfile);
			}
			fwrite('\n', 1, 1, pfile);
			fflush(pfile);
			fclose(pfile);
		 }
	}

	if(counter == STOP) 
		ev_timer_stop(loop, w);
}

int main(){

 	srand(time(NULL));
	Initial(queue, MAXQUEUE);
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
	char tbuffer[26];
	struct tm* tm_info;
	
	// get total second from 1970/01/01
	time(&now);
	// convert time_t to struct tm
	tm_info = localtime(&now);
	// convert struct tm to string assigned
	strftime(tbuffer, sizeof(tbuffer), "%Y/%m/%d %H:%M:%S", tm_info);
	
	//printf("%s\n", tbuffer);
	return tbuffer;
}