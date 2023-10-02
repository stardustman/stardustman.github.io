
C 语言里的概念在 X86-64 汇编层面的分析. 汇编风格使用 AT&T 风格. 编译器是 gcc-x86-64-9.1

# 指针

> A `pointer` is a programming language object that stores the memory address of another value located in computer memory.
> A pointer references a location in memory, and obtaining the value stored at that location is known as dereferencing the pointer.

## 变量地址

> The lea (load effective address) instruction is used to put a memory address into the destination. 
> leaq  -8(%rbp), %rdx
> -8(%rbp)等价于 mem[R[rbp] -8]. 将 rbp - 8 的值(这个值就是内存地址)复制到 rdx.

## 寄存器间接寻址

> movq    (%rax), %rdx
> 将 rax 的值(X)指向的内存地址处的值复制到 rdx. 此时 rax 的值(X)就是指针, 所谓指针在汇编层面就是将一个内存地址放到寄存器, 利用寄存器间接寻址去获取这个地址的数值. 这就是 C 语言里指针的本质. 

## movq operand combinations

![movq operand combinations](https://github.com/stardustman/pictures/raw/main/img/address-modes.png)

## 代码分析

### swap

```cpp
void swap(int *pa, int *pb){
    int temp = *pa; 
    *pa = *pb;
    *pb = temp;
}
```

```gas
swap(int*, int*):
        pushq   %rbp
        movq    %rsp, %rbp
        movq    %rdi, -24(%rbp) ; 将变量 a(10) 的地址(&a)复制到 rbp - 24 地址处, 参数 pa
        movq    %rsi, -32(%rbp) ; 将变量 b(100) 的地址(&b)复制到 rbp - 32 地址处, 参数 pb 
        movq    -24(%rbp), %rax ; 将 rbp - 24 指向的内存地址的数值(也就是变量 a 的地址)复制到 rax
        movl    (%rax), %eax    ; 将 rax 指向的内存地址的值 10(也就是变量 a) 复制到 eax, int a 4 Bytes 需要 32 bit 的 eax 即可.
        movl    %eax, -4(%rbp)  ; 将 eax 的值 10 (也就是变量 a) 复制到 rbp - 4 指向的内存地址处, 完成 temp = *pa;
        movq    -32(%rbp), %rax ; 将 rbp - 32 指向的内存地址的值(也就是变量 b 的地址) 的地址复制到 rax
        movl    (%rax), %edx    ; 将 rax 的值指向的内存地址出的值 100(也就是变量 b) 复制到 edx, int b 4 Bytes 需要 32 bit 的 eax 即可.
        movq    -24(%rbp), %rax ; 将 rbp - 24 指向的内存地址的值(也就是变量 a 的地址)复制到 rax
        movl    %edx, (%rax)    ; 将 edx 的值 100 (也就是变量 b)复制到 rax 指向的地址处(也就是变量 a 的地址), 完成 *pa = *pb
        movq    -32(%rbp), %rax ; 将 rbp - 32 指向的内存地址的值(也就是变量 b 的地址)复制到 rax
        movl    -4(%rbp), %edx  ; 将 rbp - 4 指向的内存地址的值 10 复制到 edx
        movl    %edx, (%rax)    ; 将 edx 的值 10 复制到 rax 指向的地址处(也就是变量 b 的地址), 完成 *pb = temp
        nop
        popq    %rbp
        ret
```

### main

```cpp
int main(){
    int a = 10;
    int b = 100;
    swap(&a,&b);
    return 0;
}
```

```gas
main:
        pushq   %rbp ; 保存调用者的 rbp
        movq    %rsp, %rbp ; 开辟 main 函数的栈帧
        subq    $16, %rsp  ; 分配 main 的栈空间 16 bytes
        movl    $10, -4(%rbp) ; 将变量 a 复制到 rbp - 4 指向的地址处.(int a, 4 bytes)
        movl    $100, -8(%rbp) ; 将变量 b 复制到 rbp - 8 指向的地址处.(int b, 4 bytes)
        leaq    -8(%rbp), %rdx ; 将 rbp - 8 的值复制到 rdx 寄存器(int b 变量的地址)
        leaq    -4(%rbp), %rax ; 将 rbp - 4 的值复制到 rax 寄存器(int a 变量的地址)
        movq    %rdx, %rsi ; 复制 rdx(第二个参数: 变量 b 的地址) 到 rsi 寄存器
        movq    %rax, %rdi ; 复制 rax(第一个参数: 变量 a 的地址) 到 rdi 寄存器
        call    swap(int*, int*)
        movl    $0, %eax
        leave   ; 等价于 1. mov %rbp, %rsp(回收分配的栈空间) 2.pop %rbp(恢复 main 的 rbp)
        ret
```

![swap-c-asm](https://github.com/stardustman/pictures/raw/main/img/swap-c-asm.png)

### 栈帧分析

![main-swap-call-stack](https://github.com/stardustman/pictures/raw/main/img/swap-pointer.png)

> 该图是 swap 执行完 int temp = *pa 后的状态.

### 函数返回值

> the return value for the function should be placed in rax if it is not already there. 
> 返回值放在 rax 寄存器. 这是 x86-64 的约定. 架构设计决定只有 rax 一个寄存器存放返回值, 这也是高级语言返回值只能是一个的原因. 
> C 语言里返回的结构体, 其实也就是结构体第一个字节的地址, 还是一个值.
> C 语言的函数返回值为 void, 在汇编层面也就是不用设置 rax.

### 结论

> 指针本质就是寄存器间接寻址. 函数传参其实传的都是数值而已, 本质上都是数值的副本. 

# 数据类型转化

## 整数类型

### x86-64 registers

![x86-64-registers](https://github.com/stardustman/pictures/raw/main/img/x86-64-registers.png)

### IA32 registers

![IA32-registers](https://github.com/stardustman/pictures/raw/main/img/IA32-registers.png)

### 代码分析

```cpp
void testTypeConvert(){
     unsigned long i64bit = 1844674407370955199;
     unsigned int i32bit = (unsigned int)i64bit;
     unsigned short i16bit = (unsigned int)i32bit;
     unsigned char i8bit = (unsigned char)i16bit;
     printf("%ld\n",i64bit);
     printf("%ld\n",i32bit);
     printf("%d\n",i16bit);
     printf("%d\n",i8bit);
}
```

```gas
testTypeConvert():
        pushq   %rbp
        movq    %rsp, %rbp
        movabsq $1844674407370955199, %rax ;‭00011001 10011001 10011001 10011001 10011001 10011001 10011001 10111111‬
        movq    %rax, -8(%rbp)
        movq    -8(%rbp), %rax
        movl    %eax, -12(%rbp) ; 10011001 10011001 10011001 10111111‬ (2576980415)
        movl    -12(%rbp), %eax
        movw    %ax, -14(%rbp) ; 10011001 10111111‬(39359)
        movzwl  -14(%rbp), %eax ; 00000000 00000000 10011001 10111111‬ (zero extend)
        movb    %al, -15(%rbp) ; 10111111‬(191)
        nop
        popq    %rbp
        ret
```

### 结论

int 类型的数据转化, 利用不同长度的寄存器. 比如 int 转 char 类型. 直接取 int 的最后 8 位. char 转 int ,对应的寄存器高位补上 3 个字节的 0.

# array

## C代码分析

### sumArray

```cpp
const int M = 2; 
const int N = 3; 
long sumArray(long a[], long length){
    int sum = 0;
    for(int i=0;i < length; i++){
        sum += a[i];
    }
    return sum;
}
```

### sumMatrix

```cpp
int sumMatrix(int a[M][N]){
     int sum = 0;
     int i = 0;
     int j = 0;
     for(;i < 2;i++){
         for(;j < 3;j++){
             sum += a[i][j];
         }
     }
     return sum;
}
```

### main

```cpp
int main(){
    long sum = 0;
    int sum1 = 0;
    long a[5] = {11,22,33,44,55};
    int matrix[2][3] = {{1,2,3},{4,5,6}};

    sum = sumArray(a, 5);
    sum1 = sumMatrix(matrix);
}
```

## Asm 代码分析

### sumArray

```gas
sumArray(long*, long):
        pushq   %rbp
        movq    %rsp, %rbp
        movq    %rdi, -24(%rbp) ; &a[0]
        movq    %rsi, -32(%rbp) ; length of a
        movl    $0, -4(%rbp) ; sum
        movl    $0, -8(%rbp) ; i
.L3:
        movl    -8(%rbp), %eax ; i
        cltq
        cmpq    %rax, -32(%rbp) ; cmpq i, length 其实也就是 i - length
        jle     .L2
        movl    -8(%rbp), %eax ; i
        cltq
        leaq    0(,%rax,8), %rdx ; rdx = 0 + 8 * i 
        movq    -24(%rbp), %rax  ; rax = &a[0]
        addq    %rdx, %rax       ; rax = &a[0] + (0 + 8 * i), 地址增加 8 * i
        movq    (%rax), %rax     ; rax = a[i]
        movl    %eax, %edx       ; edx = a[i]
        movl    -4(%rbp), %eax   ; eax = sum
        addl    %edx, %eax       ; eax = a[i] + sum
        movl    %eax, -4(%rbp)   
        addl    $1, -8(%rbp)    ; i = i + 1
        jmp     .L3
.L2:
        movl    -4(%rbp), %eax ; eax = sum
        cltq
        popq    %rbp
        ret
```

> 通过 a[i] 访问数组的方法, 与具体的数据类型有关  
> movl    -8(%rbp), %eax ; i  
> cltq    ; convert long(32 bit) to quad(64 bit)  
> leaq    0(,%rax,8), %rdx ; rdx = 0 + 8 * i , 比例寻址. 8 是比例因子,也就是数据类型的字节数,long 8 bytes.  
> movq    -24(%rbp), %rax  ; rax = &a[0]  
> addq    %rdx, %rax       ; rax = &a[0] + (0 + 8 * i), 地址增加 8 * i  
> movq    (%rax), %rax     ; rax = a[i]  

### Asm sumMatrix

```gas
sumMatrix(int (*) [3]):
        pushq   %rbp
        movq    %rsp, %rbp
        movq    %rdi, -24(%rbp) ; &matrix[0][0]
        movl    $0, -4(%rbp) ; sum = 0
        movl    $0, -8(%rbp) ;i = 0
        movl    $0, -12(%rbp);j = 0
; 循环体开始
.L9:
        cmpl    $1, -8(%rbp) ; i - 1
        jg      .L6 ; i - 1 > 0 ;也就是 i = 2 时跳到 .L6, 退出循环. 否则进入内部循环.
.L8:
        cmpl    $2, -12(%rbp) ; cmpl 2, j
        jg      .L7 ; j - 2 > 0 ;也就是 j = 3 时,内部循环结束, 跳到 .L7. 改变 i, 进入下一轮循环
        movl    -8(%rbp), %eax ; eax = i
        movslq  %eax, %rdx ; rdx = i
        movq    %rdx, %rax ; rax = i
        addq    %rax, %rax ; rax = i + i
        addq    %rdx, %rax ; rax = i + (i + i)
        salq    $2, %rax ; rax = 2 * 2 * 3 * i rax 算术左移 2 位
        movq    %rax, %rdx ; rdx = 12 * i
        movq    -24(%rbp), %rax ; rax = &matrix[0][0]
        addq    %rax, %rdx ; rdx = &matrix[0][0] + 12 * i
        movl    -12(%rbp), %eax ; eax = j
        cltq
        movl    (%rdx,%rax,4), %eax ; 4 * rax + rdx = &matrix[0][0] + 12 * i + 4 * j), 也就是 &matrix[i][j]. eax = matrix[i][j]
        addl    %eax, -4(%rbp) ; sum = sum + eax
        addl    $1, -12(%rbp)  ; j = j + 1
        jmp     .L8
.L7:
        addl    $1, -8(%rbp) ; i = i + 1
        jmp     .L9
.L6:
        movl    -4(%rbp), %eax ; eax = sum
        popq    %rbp
        ret
```

> int matrix[i][j]  
 movl    -8(%rbp), %eax ; eax = i  
 movslq  %eax, %rdx ; rdx = i  
 movq    %rdx, %rax ; rax = i  
 addq    %rax, %rax ; rax = i + i  
 addq    %rdx, %rax ; rax = i + (i + i)  
 salq    $2, %rax ; rax = 2 * 2 * 3 * i rax 算术左移 2 位  
 movq    %rax, %rdx ; rdx = 12 * i  
 movq    -24(%rbp), %rax ; rax = &matrix[0][0]  
 addq    %rax, %rdx ; rdx = &matrix[0][0] + 12 * i  
 movl    -12(%rbp), %eax ; eax = j  
 cltq  
 movl    (%rdx,%rax,4), %eax ; 4 * rax + rdx = 4 * j + &matrix[0][0] + 12 * i ), 也就是 &matrix[i][j]. eax = matrix[i][j]. sizeOf(int) = 4  
在 main 中可知 matrix[0][0],matrix[0][1] ... matrix[1][1],matrix[1][2]顺序排列  
当 i = 0, j = 0 时 4 * 0 + &matrix[0][0] + 12 * 0 = &matrix[0][0]  
当 i = 0, j = 1 时 4 * 1 + &matrix[0][0] + 12 * 0 = &matrix[0][1]  
当 i = 1, j = 0 时 4 * 0 + &matrix[0][0] + 12 * 1, 此时是 &matrix[0][0] + 12,   
二维数组每一行的字节总数是: 3(每一行 3 个 int) * 4(int 数据大小), 此时的地址是 &matrix[1][0].    
C 语言中二维数组作为函数参数传递时, 一定要给出列的数目, 不需要行数. 因为根据列数和数据类型的大小,就能算出下一行的第一个数据偏移起始位置. 偏移量 = 列数 * sizeOf(Data type)  

### Asm main

```gas
main:
        pushq   %rbp
        movq    %rsp, %rbp
        subq    $96, %rsp ; 分配栈空间
        movq    $0, -8(%rbp) ; sum
        movl    $0, -12(%rbp) ; sum1
        movq    $11, -64(%rbp) ; a[0]
        movq    $22, -56(%rbp) ; a[1]
        movq    $33, -48(%rbp) ; a[2]
        movq    $44, -40(%rbp) ; a[3]
        movq    $55, -32(%rbp) ; a[4]
        movl    $1, -96(%rbp)  ; matrix[0][0]
        movl    $2, -92(%rbp)  ; matrix[0][1]
        movl    $3, -88(%rbp)  ; matrix[0][2]
        movl    $4, -84(%rbp)  ; matrix[1][0]
        movl    $5, -80(%rbp)  ; matrix[1][1]
        movl    $6, -76(%rbp)  ; matrix[1][2]
        ; call sumArray
        leaq    -64(%rbp), %rax ; a[0] 的地址

        movl    $5, %esi ; length of a, 第二个参数
        movq    %rax, %rdi ; a[0] 的地址作为第一个参数
        call    sumArray(long*, long)
        movq    %rax, -8(%rbp) ; 返回值复制给 sum
        ; call sumMatrix
        leaq    -96(%rbp), %rax ; matrix[0][0] 的地址

        movq    %rax, %rdi ; matrix[0][0] 的地址作为第一个参数
        call    sumMatrix(int (*) [3])
        movl    %eax, -12(%rbp) ; 返回值复制给 sum1
        movl    $0, %eax
        leave   
        ret
```

> 由 main 可以看出, 编译器对待数组 a , a 就是数组第一个元素的地址.

## gdb 分析 array

### arrays.c

```c
int main(){
    int a[] = {1,2,3};
    int b[] = {7,8,9,10};
    return 0;
}
```

### gdb arrays

```bash
stardust@os:container$ gcc -g arrays.c -o arrays
stardust@os:container$ gdb arrays 

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from arrays...
(gdb) break main
Breakpoint 1 at 0x1149: file arrays.c, line 1.
(gdb) run
Starting program: /home/stardust/Desktop/k8s/container/arrays 

Breakpoint 1, main () at arrays.c:1
1       int main(){
(gdb) next
2           int a[] = {1,2,3};
(gdb) print a
$1 = {32767, 1431654832, 21845}
(gdb) next 
3           int b[] = {7,8,9,10};
(gdb) print b
$2 = {0, 0, 1431654496, 21845}
(gdb) next
4           return 0;
(gdb) print b
$3 = {7, 8, 9, 10}
(gdb) ptype b # 数组 b 的类型为 int [4]
type = int [4]
(gdb) ptype a # 数组 a 的类型为 int [3]
type = int [3]
(gdb) print a
$4 = {1, 2, 3}
(gdb) print b
$5 = {7, 8, 9, 10}
(gdb) print &a
$6 = (int (*)[3]) 0x7fffffffdb14
(gdb) print &b
$7 = (int (*)[4]) 0x7fffffffdb20
(gdb) print sizeof(a)
$8 = 12
(gdb) print sizeof(b)
$9 = 16
(gdb) x/12xb &a
0x7fffffffdb14: 0x01    0x00    0x00    0x00    0x02    0x00    0x00    0x00
0x7fffffffdb1c: 0x03    0x00    0x00    0x00
(gdb) print a + 1 # 指针运算
$10 = (int *) 0x7fffffffdb18
(gdb) print a + 2 # 指针运算
$11 = (int *) 0x7fffffffdb1c
(gdb) print x/4b a + 1
(gdb) x/4xb a + 1 # 查看 a[1]
0x7fffffffdb18: 0x02    0x00    0x00    0x00
(gdb) x/4xb a # 查看 a[0]
0x7fffffffdb14: 0x01    0x00    0x00    0x00
(gdb) print a[0]
$12 = 1
(gdb) print *(a + 1)
$13 = 2
(gdb) print a[1]
$14 = 2
(gdb) ptype &a # &a 的类型是指向【 3 个 int 元素的数组 】的指针
type = int (*)[3]
(gdb) ptype &b
type = int (*)[4] # &b 的类型是指向【 4 个 int 元素的数组 】的指针
(gdb) print a + 1
$15 = (int *) 0x7fffffffdb18
(gdb) print &a + 1 # 下一个【 指向 3 个 int 元素的数组的指针 】的地址
$16 = (int (*)[3]) 0x7fffffffdb20
(gdb) print &b
$18 = (int (*)[4]) 0x7fffffffdb20

```

# struct

> A struct in the C programming language (and many derivatives) is a composite data type (or record) declaration that defines a physically grouped list of variables to be placed under one name in a block of memory, allowing the different variables to be accessed via a single pointer, or the struct declared name which returns the same address.

## Code Analysis

### C

```cpp
typedef struct Student{
    long id;
    long score;
    long age;

} Student;
; 结构体指针作为参数
void testStruct(Student* s){
    s->id = 10;
    s->score = 100;
    s->age = 1000;
}

; 返回结构体
Student buildStruct(long id, long score, long age){
    Student s;
    s.id = id;
    s.score = score;
    s.age = age;
    return s;
}

int main(){
    Student s ;
    Student ss;
    s.id = 1;
    s.score = 10;
    s.age = 100;
    testStruct(&s);
    ss = buildStruct(11,22,33);
    return 0;
}
```

### Asm

```gas
testStruct(Student*):
        pushq   %rbp
        movq    %rsp, %rbp
        movq    %rdi, -8(%rbp) ; s 的地址
        movq    -8(%rbp), %rax
        movq    $10, (%rax) ; s->id = 10
        movq    -8(%rbp), %rax
        movq    $100, 8(%rax) ; s->score = 100
        movq    -8(%rbp), %rax
        movq    $1000, 16(%rax) ; s->age = 1000
        nop
        popq    %rbp
        ret
buildStruct(long, long, long):
        pushq   %rbp
        movq    %rsp, %rbp
        movq    %rdi, -8(%rbp) ; main 中隐性变量 ss 的地址(rbp(main) - 96)
        movq    %rsi, -16(%rbp) ; id
        movq    %rdx, -24(%rbp) ; score
        movq    %rcx, -32(%rbp) ; age
        movq    -8(%rbp), %rax
        movq    -16(%rbp), %rdx ; id
        movq    %rdx, (%rax)
        movq    -8(%rbp), %rax
        movq    -24(%rbp), %rdx  ; score
        movq    %rdx, 8(%rax)
        movq    -8(%rbp), %rax
        movq    -32(%rbp), %rdx ; age
        movq    %rdx, 16(%rax) 
        nop
        movq    -8(%rbp), %rax ; main 中隐性变量 ss 的地址
        popq    %rbp
        ret
main:
        pushq   %rbp
        movq    %rsp, %rbp
        subq    $96, %rsp ; 分配栈空间
        movq    $1, -32(%rbp) ; id
        movq    $10, -24(%rbp) ; score
        movq    $100, -16(%rbp) ; age
        leaq    -32(%rbp), %rax ; 结构体起始地址,也就是第一个字节的内存地址
        movq    %rax, %rdi ; 这个就类似 Java 中的 this 指针了
        call    testStruct(Student*)

        leaq    -96(%rbp), %rax ; 隐性的变量 ss 起始地址
        movl    $33, %ecx 
        movl    $22, %edx
        movl    $11, %esi
        movq    %rax, %rdi ; 隐形的参数, 像不像 Java 里的 this 啊. 编译器动的手脚.
        call    buildStruct(long, long, long)

        movq    -96(%rbp), %rax ; ss.id
        movq    -88(%rbp), %rdx ; ss.score
        movq    %rax, -64(%rbp) 
        movq    %rdx, -56(%rbp) 
        movq    -80(%rbp), %rax ; ss.age
        movq    %rax, -48(%rbp)
        movl    $0, %eax
        leave
        ret
```

> 结构体使用的是一块连续的内存地址. 结构体指针也就是结构体第一个字节的地址.

# Variable Shadowing

> 代码块、块级变量

## C 代码

```c
void test_variable_shadow(){
    int a = 10;
        // 代码块开始
        { 
            int a = 100;
            int b = a + 1000;
        }
        // 代码块结束
    int c = a + 10000;
}
```

## 汇编代码

```gas
test_variable_shadow():
        pushq   %rbp
        movq    %rsp, %rbp
        movl    $10, -4(%rbp)   ; int a = 10
        movl    $100, -8(%rbp)  ; int a = 100
        movl    -8(%rbp), %eax
        addl    $1000, %eax
        movl    %eax, -12(%rbp) ; int b = a + 1000 代码块里的 a
        movl    -4(%rbp), %eax
        addl    $10000, %eax    
        movl    %eax, -16(%rbp) ; int c = a + 10000
        nop
        popq    %rbp
        ret
```

![c-variable-shadow](https://github.com/stardustman/pictures/raw/main/img/c-variable-shadow.png)

## 汇编代码分析

![块级变量隐藏](https://github.com/stardustman/pictures/raw/main/img/variable_shadowing.png)

# funcation pointer

> A function pointer can be declared as: (return type of function) (*name of pointer) (type of function arguments)

## C 代码

```cpp
#include<stdio.h> 
int add (int a, int b)
{
    return a + b;
} 

int main(void)
{
    int(*fptr)(int,int); // Function pointer 
    fptr = add; // Assign address to function pointer 
    add(2,3);
    fptr(2,3); 
    return 0;
}
```

## 汇编分析

```gas
add(int, int):
        pushq   %rbp
        movq    %rsp, %rbp
        movl    %edi, -4(%rbp)
        movl    %esi, -8(%rbp)
        movl    -4(%rbp), %edx
        movl    -8(%rbp), %eax
        addl    %edx, %eax
        popq    %rbp
        ret
main:
        pushq   %rbp ; 保存调用者的 rbp
        movq    %rsp, %rbp ; 开辟 main 的栈帧
        subq    $16, %rsp  ; 分配栈帧空间
        movq    $add(int, int), -8(%rbp) ; 将 add(int, int) 第一条指令的地址保存在 rbp - 8 这个位置处
        movl    $3, %esi ; 从右往左保存第一个参数
        movl    $2, %edi ; 从右往左保存第二个参数
        call    add(int, int) ; 正常的 call 调用
        movq    -8(%rbp), %rax ; 将 add(int, int) 的地址复制到 rax 寄存器
        movl    $3, %esi ; 从右往左保存第一个参数
        movl    $2, %edi ; 从右往左保存第二个参数
        call    *%rax    ; 通过函数指针调用函数
        movl    $0, %eax
        leave
        ret
```

![函数指针](https://github.com/stardustman/pictures/raw/main/img/function-pointer.png)

# enum

> 编译器在背后搞鬼，自动为每一个 enum 常量进行编号。

## c 代码

```c
enum day{ monday, tuesday, wednesday,
thursday, friday, saturday, sunday};

enum color {
    RED=1, YELLOW, GREEN
};

int main(){
    enum day foo = friday;
    enum color color = GREEN;
}
```

## 汇编分析

```gas
main:
        pushq   %rbp
        movq    %rsp, %rbp
        movl    $4, -4(%rbp) ; friday 编号为 4，默认从 0 开始编号。
        movl    $3, -8(%rbp) ; GREEN 编号为 3
        movl    $0, %eax
        popq    %rbp
        ret
```

![ 枚举](https://github.com/stardustman/pictures/raw/main/img/c-enum.png)

# references

1. [pointer](https://en.wikipedia.org/wiki/Pointer_(computer_programming))
2. [lea instruction](https://stackoverflow.com/questions/29790175/assembly-x86-leave-instruction/29790275)
3. [x86 c call convention](https://aaronbloomfield.github.io/pdr/book/x86-64bit-ccc-chapter.pdf)
4. [opcode-movz](https://www.cs.auckland.ac.nz/references/macvax/op-codes/Instructions/movz.html)
5. [movsd](http://faydoc.tripod.com/cpu/movsd.htm)
6. [pass-2d-array-parameter-c](https://www.geeksforgeeks.org/pass-2d-array-parameter-c/)
7. [Variable Shadowing](https://en.wikipedia.org/wiki/Variable_shadowing)
8. [function pointer](https://www.thegeekstuff.com/2012/01/advanced-c-pointers)
9. [5-learning-c-with-gdb](https://www.recurse.com/blog/5-learning-c-with-gdb)
10. [用 gdb 学 C 语言](https://zhuanlan.zhihu.com/p/483372519)
