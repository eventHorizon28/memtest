#include<stdio.h>
#include<stdlib.h>
#include "my_malloc.h"
#include<sys/time.h>

int main()
{
	struct timeval start, end;

	gettimeofday(&start, NULL);
	int* ptr = (int*)malloc(4996);
	char* ptr2 = (char*)malloc(20);

	free(ptr2);
	gettimeofday(&end, NULL);

	printf("time: %u seconds and %u milliseconds\n", (end.tv_sec-start.tv_sec), ((end.tv_usec-start.tv_usec)));
}
