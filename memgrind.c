#include<stdio.h>
#include<stdlib.h>
#include "my_malloc.h"
#include<sys/time.h>

int main()
{
	struct timeval start, end;

	gettimeofday(&start, NULL);
	testA();
	//testB();
	//testC();
	//testD();
	gettimeofday(&end, NULL);

	printf("time: %lu seconds and %lu milliseconds\n", (end.tv_sec-start.tv_sec), ((end.tv_usec-start.tv_usec)));
}
