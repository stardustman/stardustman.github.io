#include <stdio.h>
#include <stdlib.h>
#include <time.h>
unsigned long fib(unsigned long n, unsigned long prev, unsigned long sum ){
    if(n <= 3){
        return sum;
    }
    return fib(n-1, sum , prev + sum);
}

unsigned long   fibonacc(unsigned long n){
    if( n <= 2){
        return 1;
    } 
    return fib(n, 1, 2);
}
int main(){
    unsigned long result = 0;
    clock_t beginTime,endTime;
    beginTime = clock();
    result = fibonacc(50);
    endTime = clock();
    printf("Running Time %f Seconds\n",(double)(endTime - beginTime)/CLOCKS_PER_SEC);
    printf("fib(50) is %lu", result);
    return 0;
}