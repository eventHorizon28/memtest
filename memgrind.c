#include<stdio.h>
#include<stdlib.h>
#include "my_malloc.h"
#include<sys/time.h>

double testA(){
struct timeval start, end;
gettimeofday(&start, NULL);

int i=0;
for(;i < 150; i++){
	char * a = malloc(sizeof(char));
	free(a);
		}
gettimeofday(&end, NULL);
printf("time: %lu seconds and %lu milliseconds\n", (end.tv_sec-start.tv_sec), ((end.tv_usec-start.tv_usec)));
}
double testB(){
static char myblock[5000];


}

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
