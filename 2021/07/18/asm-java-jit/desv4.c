#include <stdio.h>
#include <memory.h>
#include <sys/mman.h>

typedef int (*desc_func)(int a);
int main()
{
    char desc_code[] = {
        0x48, 0x8d, 0x47, 0xff, // lea -0x1(rdi), rax
        0xc3                    // ret
    };

    void *temp = mmap(NULL, sizeof(desc_code),
                      PROT_WRITE | PROT_EXEC,
                      MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    memcpy(temp, desc_code, sizeof(desc_code));
    desc_func p_desc = (desc_func)temp;
    printf("%d\n", p_desc(3));

    return 0;
}