#include <stdio.h>

long desc(long a)
{
    return a - 1;
}

int main()
{
    printf("%ld\n", desc(3));
    return 0;
}
