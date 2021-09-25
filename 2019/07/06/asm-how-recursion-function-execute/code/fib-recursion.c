#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LENGTH 51
unsigned long fib(unsigned long n){
    if(n <= 2){
        return 1;
    }
   return fib(n-1) + fib(n-2);
}

int main(){
    unsigned long long result = 0;
    clock_t beginTime,endTime;
    beginTime = clock();
    result = fib(LENGTH - 1);
    endTime = clock();
    printf("Running Time %f Seconds\n",(double)(endTime - beginTime)/CLOCKS_PER_SEC);
    printf(" %d :: %lu \n", LENGTH - 1, result);
	return 0;
}