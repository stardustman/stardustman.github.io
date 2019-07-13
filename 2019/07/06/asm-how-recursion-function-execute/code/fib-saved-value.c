#include <stdio.h>
#include <stdlib.h>
#include <time.h>
# define LENGTH 51
// 计算 fib(50)
unsigned long result[LENGTH];
unsigned long fib(long n){
    if(result[n] != 0){
        return result[n];
    } else{
       result[n] =  fib(n-1) + fib(n-2);
    } 
   return result[n];
}

int main(){
    clock_t beginTime,endTime;
    for(int i = 0; i < LENGTH ; i++){
        result[i] = 0;
    }

    result[1] = 1;
    result[2] = 1;
    beginTime = clock();
    fib(LENGTH - 1);
    endTime = clock();
	printf("Running Time %f Seconds\n",(double)(endTime - beginTime)/CLOCKS_PER_SEC);
	for(int i = 1; i < LENGTH; i++){
		printf(" %i :: %lu \n", i, result[i]);
	}
	
}
