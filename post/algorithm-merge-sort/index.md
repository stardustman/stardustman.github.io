
## merge sort

### merge sort C code

```clang
#include <stdio.h>
void merge(int* array, int start, int mid, int end){
    // start = 0
    // mid = 3
    // end = 7
    int tempArrayLength = end - start + 1; // 8
    int tempArray[tempArrayLength]; // tempArray[8]
    int p1 = start; // 0
    int p2 = mid + 1; // 4
    int p = 0;
       // 比较两个小集合，依次放入大集合, 第一个 while 循环
        while (p1 <= mid && p2 <= end){ p1 <=3 && p2 <=7
            if(array[p1] <= array[p2]){ // 7 <= 5 XXX
                tempArray[p++] = array[p1++];
            } else {
                tempArray[p++] = array[p2++]; // tempArray[0] = 5
            }
        }

        // 左侧集合还有剩余，依次放入大集合， 第二个 while 循环
        while (p1 <= mid){ // 2 <= 2 
            tempArray[p++] = array[p1++]; tempArray[1] = 7
        }
        // 右侧集合还有剩余，依次放入大集合，第三个 while 循环
        while (p2 <= end){ // 4 <= 3 XXX
            tempArray[p++] = array[p2++]; // tempArray[1] = 9
        }

        // 大集合复制到原来的数组，for 循环
        for (int i = 0; i < tempArrayLength; i++) {
            array[i + start] = tempArray[i]; //  3,9,5,7
        }
}

void mergeSort(int* array, int start, int end){
    if(start < end){
        int mid = (start + end) / 2;
        mergeSort(array, start, mid); // mergeSort_L(array, start, mid);
        mergeSort(array, mid+1, end); // mergeSort_R(array, mid+1, end);
        merge(array, start, mid, end);
    }
}


int main(){
     int array[8] = {9, 3, 7, 5, 8, 6, 4, 10};
     mergeSort(array, 0 , 7);
     for (int i = 0; i < 8; i++)
     {
         printf("%d#",array[i]);
     }
     
}
```

### x64 汇编代码分析

```asm
mergesort:     file format elf64-x86-64 ; 64bits 汇编代码


Disassembly of section .init:

0000000000000548 <_init>:
 548:	48 83 ec 08          	sub    $0x8,%rsp
 54c:	48 8b 05 95 0a 20 00 	mov    0x200a95(%rip),%rax        # 200fe8 <__gmon_start__>
 553:	48 85 c0             	test   %rax,%rax
 556:	74 02                	je     55a <_init+0x12>
 558:	ff d0                	callq  *%rax
 55a:	48 83 c4 08          	add    $0x8,%rsp
 55e:	c3                   	retq   

Disassembly of section .plt:

0000000000000560 <.plt>:
 560:	ff 35 52 0a 20 00    	pushq  0x200a52(%rip)        # 200fb8 <_GLOBAL_OFFSET_TABLE_+0x8>
 566:	ff 25 54 0a 20 00    	jmpq   *0x200a54(%rip)        # 200fc0 <_GLOBAL_OFFSET_TABLE_+0x10>
 56c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000000570 <__stack_chk_fail@plt>:
 570:	ff 25 52 0a 20 00    	jmpq   *0x200a52(%rip)        # 200fc8 <__stack_chk_fail@GLIBC_2.4>
 576:	68 00 00 00 00       	pushq  $0x0
 57b:	e9 e0 ff ff ff       	jmpq   560 <.plt>

0000000000000580 <printf@plt>:
 580:	ff 25 4a 0a 20 00    	jmpq   *0x200a4a(%rip)        # 200fd0 <printf@GLIBC_2.2.5>
 586:	68 01 00 00 00       	pushq  $0x1
 58b:	e9 d0 ff ff ff       	jmpq   560 <.plt>

Disassembly of section .plt.got:

0000000000000590 <__cxa_finalize@plt>:
 590:	ff 25 62 0a 20 00    	jmpq   *0x200a62(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 596:	66 90                	xchg   %ax,%ax

Disassembly of section .text: ; 代码区

00000000000005a0 <_start>:
 5a0:	31 ed                	xor    %ebp,%ebp ; ebp = 0
 5a2:	49 89 d1             	mov    %rdx,%r9 
 5a5:	5e                   	pop    %rsi
 5a6:	48 89 e2             	mov    %rsp,%rdx
 5a9:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
 5ad:	50                   	push   %rax
 5ae:	54                   	push   %rsp
 5af:	4c 8d 05 aa 04 00 00 	lea    0x4aa(%rip),%r8        # a60 <__libc_csu_fini>
 5b6:	48 8d 0d 33 04 00 00 	lea    0x433(%rip),%rcx        # 9f0 <__libc_csu_init>
 5bd:	48 8d 3d 79 03 00 00 	lea    0x379(%rip),%rdi        # 93d <main>
 5c4:	ff 15 16 0a 20 00    	callq  *0x200a16(%rip)        # 200fe0 <__libc_start_main@GLIBC_2.2.5>
 5ca:	f4                   	hlt    
 5cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000005d0 <deregister_tm_clones>:
 5d0:	48 8d 3d 39 0a 20 00 	lea    0x200a39(%rip),%rdi        # 201010 <__TMC_END__>
 5d7:	55                   	push   %rbp
 5d8:	48 8d 05 31 0a 20 00 	lea    0x200a31(%rip),%rax        # 201010 <__TMC_END__>
 5df:	48 39 f8             	cmp    %rdi,%rax
 5e2:	48 89 e5             	mov    %rsp,%rbp
 5e5:	74 19                	je     600 <deregister_tm_clones+0x30>
 5e7:	48 8b 05 ea 09 20 00 	mov    0x2009ea(%rip),%rax        # 200fd8 <_ITM_deregisterTMCloneTable>
 5ee:	48 85 c0             	test   %rax,%rax
 5f1:	74 0d                	je     600 <deregister_tm_clones+0x30>
 5f3:	5d                   	pop    %rbp
 5f4:	ff e0                	jmpq   *%rax
 5f6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 5fd:	00 00 00 
 600:	5d                   	pop    %rbp
 601:	c3                   	retq   
 602:	0f 1f 40 00          	nopl   0x0(%rax)
 606:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 60d:	00 00 00 

0000000000000610 <register_tm_clones>:
 610:	48 8d 3d f9 09 20 00 	lea    0x2009f9(%rip),%rdi        # 201010 <__TMC_END__>
 617:	48 8d 35 f2 09 20 00 	lea    0x2009f2(%rip),%rsi        # 201010 <__TMC_END__>
 61e:	55                   	push   %rbp
 61f:	48 29 fe             	sub    %rdi,%rsi
 622:	48 89 e5             	mov    %rsp,%rbp
 625:	48 c1 fe 03          	sar    $0x3,%rsi
 629:	48 89 f0             	mov    %rsi,%rax
 62c:	48 c1 e8 3f          	shr    $0x3f,%rax
 630:	48 01 c6             	add    %rax,%rsi
 633:	48 d1 fe             	sar    %rsi
 636:	74 18                	je     650 <register_tm_clones+0x40>
 638:	48 8b 05 b1 09 20 00 	mov    0x2009b1(%rip),%rax        # 200ff0 <_ITM_registerTMCloneTable>
 63f:	48 85 c0             	test   %rax,%rax
 642:	74 0c                	je     650 <register_tm_clones+0x40>
 644:	5d                   	pop    %rbp
 645:	ff e0                	jmpq   *%rax
 647:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
 64e:	00 00 
 650:	5d                   	pop    %rbp
 651:	c3                   	retq   
 652:	0f 1f 40 00          	nopl   0x0(%rax)
 656:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 65d:	00 00 00 

0000000000000660 <__do_global_dtors_aux>:
 660:	80 3d a9 09 20 00 00 	cmpb   $0x0,0x2009a9(%rip)        # 201010 <__TMC_END__>
 667:	75 2f                	jne    698 <__do_global_dtors_aux+0x38>
 669:	48 83 3d 87 09 20 00 	cmpq   $0x0,0x200987(%rip)        # 200ff8 <__cxa_finalize@GLIBC_2.2.5>
 670:	00 
 671:	55                   	push   %rbp
 672:	48 89 e5             	mov    %rsp,%rbp
 675:	74 0c                	je     683 <__do_global_dtors_aux+0x23>
 677:	48 8b 3d 8a 09 20 00 	mov    0x20098a(%rip),%rdi        # 201008 <__dso_handle>
 67e:	e8 0d ff ff ff       	callq  590 <__cxa_finalize@plt>
 683:	e8 48 ff ff ff       	callq  5d0 <deregister_tm_clones>
 688:	c6 05 81 09 20 00 01 	movb   $0x1,0x200981(%rip)        # 201010 <__TMC_END__>
 68f:	5d                   	pop    %rbp
 690:	c3                   	retq   
 691:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 698:	f3 c3                	repz retq 
 69a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000000006a0 <frame_dummy>:
 6a0:	55                   	push   %rbp
 6a1:	48 89 e5             	mov    %rsp,%rbp
 6a4:	5d                   	pop    %rbp
 6a5:	e9 66 ff ff ff       	jmpq   610 <register_tm_clones>

00000000000006aa <merge>:
 6aa:	55                   	push   %rbp
 6ab:	48 89 e5             	mov    %rsp,%rbp ; rbp = rsp
 6ae:	48 83 ec 50          	sub    $0x50,%rsp ; rsp = rsp - 0x50 开辟栈帧
 6b2:	48 89 7d c8          	mov    %rdi,-0x38(%rbp) ; 第一个参数 &array[0]
 6b6:	89 75 c4             	mov    %esi,-0x3c(%rbp) ; 第二个参数 start
 6b9:	89 55 c0             	mov    %edx,-0x40(%rbp) ; 第三个参数 mid
 6bc:	89 4d bc             	mov    %ecx,-0x44(%rbp) ; 第四个参数 end
 6bf:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax ; 哨兵值判定栈帧是否出了问题。
 6c6:	00 00 
 6c8:	48 89 45 f8          	mov    %rax,-0x8(%rbp) ; 哨兵值
 6cc:	31 c0                	xor    %eax,%eax ; eax = 0
 6ce:	48 89 e0             	mov    %rsp,%rax ; rax = rsp
 6d1:	48 89 c6             	mov    %rax,%rsi ; rsi = rax = rsp 记录这个 rsp 的值
 6d4:	8b 45 bc             	mov    -0x44(%rbp),%eax ; eax = end
 6d7:	2b 45 c4             	sub    -0x3c(%rbp),%eax ; eax = end - start
 6da:	83 c0 01             	add    $0x1,%eax ; tempArrayLength = eax = (end - start) + 1
 6dd:	89 45 e4             	mov    %eax,-0x1c(%rbp) ; tempArrayLength = (end - start) + 1 复制到 rbp - 0x1c 内存处
 6e0:	8b 45 e4             	mov    -0x1c(%rbp),%eax ; eax = tempArrayLength
 6e3:	48 63 d0             	movslq %eax,%rdx ; rdx = tempArrayLength, cltq 32bit 扩展为 64bit
 6e6:	48 83 ea 01          	sub    $0x1,%rdx ; rdx = (tempArrayLength) - 1 = end - start
 6ea:	48 89 55 e8          	mov    %rdx,-0x18(%rbp) ; end - start 复制到 rbp - 0x18 内存地址处
 6ee:	48 63 d0             	movslq %eax,%rdx ; rdx = (end - start) + 1 ? tempArrayLength
 6f1:	49 89 d2             	mov    %rdx,%r10 ; r10 = (end - start) + 1
 6f4:	41 bb 00 00 00 00    	mov    $0x0,%r11d ; r11d = 0
 6fa:	48 63 d0             	movslq %eax,%rdx ; rdx = (end - start) + 1 ? tempArrayLength
 6fd:	49 89 d0             	mov    %rdx,%r8 ; r8 = (end - start) + 1 ? tempArrayLength
 700:	41 b9 00 00 00 00    	mov    $0x0,%r9d ; r9d = 0
 706:	48 98                	cltq   ; 相当于 movslq %eax,%rax.
 708:	48 c1 e0 02          	shl    $0x2,%rax ; rax = ((end - start) + 1) * 4
 70c:	48 8d 50 03          	lea    0x3(%rax),%rdx ; rdx = 4 * ((end - start) + 1) + 3 
 710:	b8 10 00 00 00       	mov    $0x10,%eax ; eax = 16
 715:	48 83 e8 01          	sub    $0x1,%rax ; rax = 16 - 1 = 15
 719:	48 01 d0             	add    %rdx,%rax ; rax = rax + rdx = 15 + 4 * ((end - start) + 1) + 3 = 4 * (end - start) + 22
 71c:	bf 10 00 00 00       	mov    $0x10,%edi ; edi = 16
 721:	ba 00 00 00 00       	mov    $0x0,%edx ; edx = 0
 726:	48 f7 f7             	div    %rdi ; (rdx(high-32bit):rax(low-32bit))/rdi , quotient(商) in %rax, remainder(余数) in %rdx
 729:	48 6b c0 10          	imul   $0x10,%rax,%rax ; rax = rax * 16
 72d:	48 29 c4             	sub    %rax,%rsp ; rsp = rsp - rax 分配空间
 730:	48 89 e0             	mov    %rsp,%rax ; rax = rsp
 733:	48 83 c0 03          	add    $0x3,%rax ; rax = rsp + 3
 737:	48 c1 e8 02          	shr    $0x2,%rax ; rax = (rsp + 3) >> 2
 73b:	48 c1 e0 02          	shl    $0x2,%rax ; rax = ((rsp + 3) >> 2) <<2
 73f:	48 89 45 f0          	mov    %rax,-0x10(%rbp) ;
 743:	8b 45 c4             	mov    -0x3c(%rbp),%eax ; eax = start
 746:	89 45 dc             	mov    %eax,-0x24(%rbp) ; start 复制到 rbp - 0x24(p1) 处
 749:	8b 45 c0             	mov    -0x40(%rbp),%eax ; eax = mid
 74c:	83 c0 01             	add    $0x1,%eax ; eax = mid + 1
 74f:	89 45 d8             	mov    %eax,-0x28(%rbp) ; mid + 1 复制到 rbp - 0x28(p2) 处
 752:	c7 45 d4 00 00 00 00 	movl   $0x0,-0x2c(%rbp) ; 0 复制到 rbp - 0x2c(p) 处
 759:	e9 90 00 00 00       	jmpq   7ee <merge+0x144> ; 无条件跳转, 0x144 表示要跳转出也就是 7ee 距离 merge(6aa) 之间的所有指令字节数是 0x144。。。。。。

 ; if(array[p1] <= array[p2]) 的判定
 75e:	8b 45 dc             	mov    -0x24(%rbp),%eax ; rax = p1
 761:	48 98                	cltq   
 763:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx ; rdx = 4 * p1 + 0
 76a:	00 
 76b:	48 8b 45 c8          	mov    -0x38(%rbp),%rax ; rax = &array[0]
 76f:	48 01 d0             	add    %rdx,%rax ; rax = &array[0] + 4 * p1 = &array[p1] 
 772:	8b 10                	mov    (%rax),%edx ; edx = array[p1]
 774:	8b 45 d8             	mov    -0x28(%rbp),%eax ; eax = p2
 777:	48 98                	cltq   
 779:	48 8d 0c 85 00 00 00 	lea    0x0(,%rax,4),%rcx ; rcx = 4 * rax = 4 * p2
 780:	00 
 781:	48 8b 45 c8          	mov    -0x38(%rbp),%rax ; rax = &array[0]
 785:	48 01 c8             	add    %rcx,%rax ; rax = &array[0] + 4 * p2 = &array[p2]
 788:	8b 00                	mov    (%rax),%eax ; eax = array[p2]
 78a:	39 c2                	cmp    %eax,%edx ; cmp array[p2],array[p1]
 78c:	7f 31                	jg     7bf <merge+0x115> ; array[p1] > array[p2] 跳到 else
 78e:	8b 45 dc             	mov    -0x24(%rbp),%eax ; eax = p1
 791:	8d 50 01             	lea    0x1(%rax),%edx ; edx = p1 + 1
 794:	89 55 dc             	mov    %edx,-0x24(%rbp) ; p1 = p1 + 1
 797:	48 98                	cltq   
 799:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx ; rdx = 4 * p1
 7a0:	00 
 7a1:	48 8b 45 c8          	mov    -0x38(%rbp),%rax ; rax = &array[0]
 7a5:	48 8d 0c 02          	lea    (%rdx,%rax,1),%rcx ; rcx = &array[0] + 4 * p1 = &array[p1]
 7a9:	8b 45 d4             	mov    -0x2c(%rbp),%eax ; eax = p
 7ac:	8d 50 01             	lea    0x1(%rax),%edx ; edx = p + 1
 7af:	89 55 d4             	mov    %edx,-0x2c(%rbp) ; p = p + 1
 7b2:	8b 09                	mov    (%rcx),%ecx ; ecx = array[p1]
 7b4:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx ;rdx = &tempArray[0]
 7b8:	48 98                	cltq   
 7ba:	89 0c 82             	mov    %ecx,(%rdx,%rax,4) ; tempArray[p] = array[p1]
 7bd:	eb 2f                	jmp    7ee <merge+0x144> ; 开启下一轮判定>>>>>>>

 7bf:	8b 45 d8             	mov    -0x28(%rbp),%eax ; eax = p2 $$$ begin else array[p1] > array[p2]
 7c2:	8d 50 01             	lea    0x1(%rax),%edx ; edx = p2 + 1
 7c5:	89 55 d8             	mov    %edx,-0x28(%rbp) ; p2 = p2 + 1
 7c8:	48 98                	cltq   
 7ca:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx ; rdx = 4 * p2
 7d1:	00 
 7d2:	48 8b 45 c8          	mov    -0x38(%rbp),%rax ; rax = &array[0]
 7d6:	48 8d 0c 02          	lea    (%rdx,%rax,1),%rcx ; rcx = rdx + 1 * rax = &array[0] + 4 * p2 = &array[p2] 
 7da:	8b 45 d4             	mov    -0x2c(%rbp),%eax ; eax = p
 7dd:	8d 50 01             	lea    0x1(%rax),%edx ; edx = p + 1
 7e0:	89 55 d4             	mov    %edx,-0x2c(%rbp) ; p = p + 1
 7e3:	8b 09                	mov    (%rcx),%ecx ; ecx = array[p2]
 7e5:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx ; rdx = &tempArray[0] , 临时数组地址
 7e9:	48 98                	cltq   
 7eb:	89 0c 82             	mov    %ecx,(%rdx,%rax,4) ; tempArray[p] = array[p2] $$$$$ end else
 7ee:	8b 45 dc             	mov    -0x24(%rbp),%eax ; eax = p1  下一轮判定>>>>>>>
 7f1:	3b 45 c0             	cmp    -0x40(%rbp),%eax ; cmp mid,p1
 7f4:	7f 3d                	jg     833 <merge+0x189> ;   p1 > mid 跳出去 
 7f6:	8b 45 d8             	mov    -0x28(%rbp),%eax ; eax = p2 此时 p1 <= mid, 判定 p2 <= end 吗？
 7f9:	3b 45 bc             	cmp    -0x44(%rbp),%eax ; cmp end,p2
 7fc:	0f 8e 5c ff ff ff    	jle    75e <merge+0xb4> ; 此时 p1 <= mid,p2 <= end，进入 if。 
 802:	eb 2f                	jmp    833 <merge+0x189>; 不满足第一个 while 循环，进入第二个 while 循环

; p1 <= mid 循环体开始
 804:	8b 45 dc             	mov    -0x24(%rbp),%eax ; eax = p1 
 807:	8d 50 01             	lea    0x1(%rax),%edx ; edx = p1 + 1
 80a:	89 55 dc             	mov    %edx,-0x24(%rbp) ; p1 = p1 + 1 ####
 80d:	48 98                	cltq   ; movslq %eax,%rax
 80f:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx ; rdx = 4 * p1
 816:	00 
 817:	48 8b 45 c8          	mov    -0x38(%rbp),%rax ; rax = &array[0]
 81b:	48 8d 0c 02          	lea    (%rdx,%rax,1),%rcx ; rcx = &array[p1]
 81f:	8b 45 d4             	mov    -0x2c(%rbp),%eax ; eax = p
 822:	8d 50 01             	lea    0x1(%rax),%edx ; edx = p + 1
 825:	89 55 d4             	mov    %edx,-0x2c(%rbp) ; p = p + 1 
 828:	8b 09                	mov    (%rcx),%ecx ; ecx = array[p1]
 82a:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx ; rdx = &tempArray[0] 
 82e:	48 98                	cltq   
 830:	89 0c 82             	mov    %ecx,(%rdx,%rax,4) ; tempArray[p] = array[p1]

; 第二个 while 循环 也就是 while(p1 <= mid)
 833:	8b 45 dc             	mov    -0x24(%rbp),%eax ; eax = p1  
 836:	3b 45 c0             	cmp    -0x40(%rbp),%eax ; cmp mid, p1 判定 p1 <= mid 吗？
 839:	7e c9                	jle    804 <merge+0x15a> ;  p1 <= mid
; p1 <= mid 循环体结束

 83b:	eb 2f                	jmp    86c <merge+0x1c2> ; 第二个 while 循环的 p1 <= mid不满足， 短路。直接进入第三个while(p2 <= end) 

; p2 <= end 的循环体开始
 83d:	8b 45 d8             	mov    -0x28(%rbp),%eax ; eax = p2
 840:	8d 50 01             	lea    0x1(%rax),%edx ; edx = p2 + 1
 843:	89 55 d8             	mov    %edx,-0x28(%rbp) ; p = p2 + 1
 846:	48 98                	cltq   
 848:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx ; rdx = 4 * p2
 84f:	00 
 850:	48 8b 45 c8          	mov    -0x38(%rbp),%rax ; rax = &array[0]
 854:	48 8d 0c 02          	lea    (%rdx,%rax,1),%rcx ; rcx = &array[0] + 4 * p2 = &array[p2]
 858:	8b 45 d4             	mov    -0x2c(%rbp),%eax ; eax = p
 85b:	8d 50 01             	lea    0x1(%rax),%edx ; eax = p + 1
 85e:	89 55 d4             	mov    %edx,-0x2c(%rbp) ; p = p + 1
 861:	8b 09                	mov    (%rcx),%ecx ; ecx = array[p2]
 863:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx ;rdx = &tempArray[0]
 867:	48 98                	cltq   
 869:	89 0c 82             	mov    %ecx,(%rdx,%rax,4) ; tempArray[p] = array[p2]

 ; 第三个 while 循环
 86c:	8b 45 d8             	mov    -0x28(%rbp),%eax ; eax = p2 开始判定 p2 <= end 吗？
 86f:	3b 45 bc             	cmp    -0x44(%rbp),%eax ; cmp end, p2
 872:	7e c9                	jle    83d <merge+0x193> ; 满足 p2 <= end，第三个 while 循环

 ; p2 > end
 874:	c7 45 e0 00 00 00 00 	movl   $0x0,-0x20(%rbp) ; 此时 p2 > end 了。i = 0
 87b:	eb 2d                	jmp    8aa <merge+0x200>  ; 直接跳到 for 循环

; for(int i = 0; i < tempArrayLength; i++) 开始
 87d:	8b 55 e0             	mov    -0x20(%rbp),%edx ; edx = i
 880:	8b 45 c4             	mov    -0x3c(%rbp),%eax ; eax = start
 883:	01 d0                	add    %edx,%eax ; eax = start + i
 885:	48 98                	cltq   
 887:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx ; rdx = 4 * (start + i)
 88e:	00 
 88f:	48 8b 45 c8          	mov    -0x38(%rbp),%rax ; rax = &array[0]
 893:	48 8d 0c 02          	lea    (%rdx,%rax,1),%rcx ; rcx = array[start + i] 
 897:	48 8b 45 f0          	mov    -0x10(%rbp),%rax ; rax = &tempArray[0]
 89b:	8b 55 e0             	mov    -0x20(%rbp),%edx ; edx = i
 89e:	48 63 d2             	movslq %edx,%rdx
 8a1:	8b 04 90             	mov    (%rax,%rdx,4),%eax ; eax = tempArray[i]
 8a4:	89 01                	mov    %eax,(%rcx) ; array[start + i] = tempArray[i];
 8a6:	83 45 e0 01          	addl   $0x1,-0x20(%rbp) ; i = i + 1
 ; 判定 i < tempArrayLength 吗？
 8aa:	8b 45 e0             	mov    -0x20(%rbp),%eax ; eax = i 
 8ad:	3b 45 e4             	cmp    -0x1c(%rbp),%eax ; cmp tempArrayLength, i
 8b0:	7c cb                	jl     87d <merge+0x1d3> ; i < tempArrayLength
; for(int i = 0; i < tempArrayLength; i++) 结束

; 此时 i >= tempArrayLength 了。
 8b2:	48 89 f4             	mov    %rsi,%rsp ; rsp = rsi(记录的 rsp 的值) i >= tempArrayLength
 8b5:	90                   	nop
 8b6:	48 8b 45 f8          	mov    -0x8(%rbp),%rax ; rax = %fs:0x28
 8ba:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax ;The OF and CF flags are cleared; the SF, ZF, and PF flags are set according to the result. The state of the AF flag is undefined.
 8c1:	00 00 
 8c3:	74 05                	je     8ca <merge+0x220> ; ZF = 0 说明栈帧没出问题，退出函数。
 8c5:	e8 a6 fc ff ff       	callq  570 <__stack_chk_fail@plt> ; 能执行到这说明栈帧出问题了。。。

 8ca:	c9                   	leaveq 
 8cb:	c3                   	retq   

00000000000008cc <mergeSort>:
 8cc:	55                   	push   %rbp
 8cd:	48 89 e5             	mov    %rsp,%rbp ; rbp = rsp
 8d0:	48 83 ec 20          	sub    $0x20,%rsp ; rsp = rsp - 0x20
 8d4:	48 89 7d e8          	mov    %rdi,-0x18(%rbp) ;&array[0] 复制到 rbp - 0x18 地址处
 8d8:	89 75 e4             	mov    %esi,-0x1c(%rbp) ;第二个参数 (start) 复制到 rbp -0x1c 地址处
 8db:	89 55 e0             	mov    %edx,-0x20(%rbp) ;第三个参数 (end) 复制到 rbp -0x20 地址处
 8de:	8b 45 e4             	mov    -0x1c(%rbp),%eax ;eax = start
 8e1:	3b 45 e0             	cmp    -0x20(%rbp),%eax ;cmp end,start 
 8e4:	7d 54                	jge    93a <mergeSort+0x6e> ; start >= end，直接退出函数。
 8e6:	8b 55 e4             	mov    -0x1c(%rbp),%edx ; edx = start 此时 start < end 
 8e9:	8b 45 e0             	mov    -0x20(%rbp),%eax ; eax = end
 8ec:	01 d0                	add    %edx,%eax ; eax = start + end
 8ee:	89 c2                	mov    %eax,%edx ; edx = start + end
 8f0:	c1 ea 1f             	shr    $0x1f,%edx ; edx = edx >> 31 (start + end)/pow(2,31) 相当于是 0 了😂
 8f3:	01 d0                	add    %edx,%eax ; eax = (start + end) + 0
 8f5:	d1 f8                	sar    %eax ; eax = (start + end) / 2 算术右移一位，最高位补上 eax 的符号位
 8f7:	89 45 fc             	mov    %eax,-0x4(%rbp) ; mid = (start + end) / 2 复制到 rbp - 0x4 内存地址处
 8fa:	8b 55 fc             	mov    -0x4(%rbp),%edx ; edx = mid(start + end) / 2 也就是准备第三个参数 mid
 8fd:	8b 4d e4             	mov    -0x1c(%rbp),%ecx ; ecx = start
 900:	48 8b 45 e8          	mov    -0x18(%rbp),%rax ; rax = &array[0]
 904:	89 ce                	mov    %ecx,%esi ; esi = start ; 第二个参数
 906:	48 89 c7             	mov    %rax,%rdi ; rdi = &array[0] ; 第一个参数
 909:	e8 be ff ff ff       	callq  8cc <mergeSort> ; 调用 mergeSort 左侧 mergeSort

 90e:	8b 45 fc             	mov    -0x4(%rbp),%eax ; eax = mid = (start + end) / 2
 911:	8d 48 01             	lea    0x1(%rax),%ecx  ; mid = mid + 1 
 914:	8b 55 e0             	mov    -0x20(%rbp),%edx ; edx = end 第三个参数
 917:	48 8b 45 e8          	mov    -0x18(%rbp),%rax ; rax = &array[0]
 91b:	89 ce                	mov    %ecx,%esi ; esi = mid 第二个参数
 91d:	48 89 c7             	mov    %rax,%rdi ; rdi = &array[0] 第一个参数
 920:	e8 a7 ff ff ff       	callq  8cc <mergeSort> ; 调用 mergeSort 右侧 mergeSort

 925:	8b 4d e0             	mov    -0x20(%rbp),%ecx ; ecx = end ; 第四个参数
 928:	8b 55 fc             	mov    -0x4(%rbp),%edx ; edx = mid = (start + end) / 2 第三个参数
 92b:	8b 75 e4             	mov    -0x1c(%rbp),%esi ; esi = start ; 第二个参数
 92e:	48 8b 45 e8          	mov    -0x18(%rbp),%rax ; rax = &array[0] 
 932:	48 89 c7             	mov    %rax,%rdi ; rdi = rax  ; 第一个参数
 935:	e8 70 fd ff ff       	callq  6aa <merge> ; 调用 merge 开始 merge

 93a:	90                   	nop ; 从此处销毁栈帧
 93b:	c9                   	leaveq // 销毁栈帧，相当于 rsp = rbp，pop rbp
 93c:	c3                   	retq   

000000000000093d <main>:
 93d:	55                   	push   %rbp
 93e:	48 89 e5             	mov    %rsp,%rbp
 941:	48 83 ec 40          	sub    $0x40,%rsp
 945:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
 94c:	00 00 
 94e:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
 952:	31 c0                	xor    %eax,%eax
 954:	c7 45 d0 09 00 00 00 	movl   $0x9,-0x30(%rbp) ; 0x9 复制到 rbp - 0x30 地址处。array[0] = 0x9
 95b:	c7 45 d4 03 00 00 00 	movl   $0x3,-0x2c(%rbp) ; 0x3 复制到 rbp - 0x2c 地址处。array[1] = 0x3
 962:	c7 45 d8 07 00 00 00 	movl   $0x7,-0x28(%rbp) ; 0x7 复制到 rbp - 0x28 地址处。array[2] = 0x7
 969:	c7 45 dc 05 00 00 00 	movl   $0x5,-0x24(%rbp) ; 0x5 复制到 rbp - 0x24 地址处。array[3] = 0x5
 970:	c7 45 e0 08 00 00 00 	movl   $0x8,-0x20(%rbp) ; 0x8 复制到 rbp - 0x20 地址处。array[4] = 0x8
 977:	c7 45 e4 06 00 00 00 	movl   $0x6,-0x1c(%rbp) ; 0x6 复制到 rbp - 0x1c 地址处。array[5] = 0x6
 97e:	c7 45 e8 04 00 00 00 	movl   $0x4,-0x18(%rbp) ; 0x4 复制到 rbp - 0x28 地址处。array[6] = 0x4
 985:	c7 45 ec 0a 00 00 00 	movl   $0xa,-0x14(%rbp) ; 0xa 复制到 rbp - 0x14 地址处。array[7] = 0xa
 98c:	48 8d 45 d0          	lea    -0x30(%rbp),%rax ; rax = &array[0]
 990:	ba 07 00 00 00       	mov    $0x7,%edx ; 第三个参数 7
 995:	be 00 00 00 00       	mov    $0x0,%esi ; 第二个参数 0
 99a:	48 89 c7             	mov    %rax,%rdi ; 第一个参数 &array[0]
 99d:	e8 2a ff ff ff       	callq  8cc <mergeSort> ; 调用 mergeSort，地址偏移是 0x8cc
 9a2:	c7 45 cc 00 00 00 00 	movl   $0x0,-0x34(%rbp)
 9a9:	eb 20                	jmp    9cb <main+0x8e>
 9ab:	8b 45 cc             	mov    -0x34(%rbp),%eax
 9ae:	48 98                	cltq   
 9b0:	8b 44 85 d0          	mov    -0x30(%rbp,%rax,4),%eax
 9b4:	89 c6                	mov    %eax,%esi
 9b6:	48 8d 3d b7 00 00 00 	lea    0xb7(%rip),%rdi        # a74 <_IO_stdin_used+0x4>
 9bd:	b8 00 00 00 00       	mov    $0x0,%eax
 9c2:	e8 b9 fb ff ff       	callq  580 <printf@plt>
 9c7:	83 45 cc 01          	addl   $0x1,-0x34(%rbp)
 9cb:	83 7d cc 07          	cmpl   $0x7,-0x34(%rbp)
 9cf:	7e da                	jle    9ab <main+0x6e>
 9d1:	b8 00 00 00 00       	mov    $0x0,%eax
 9d6:	48 8b 4d f8          	mov    -0x8(%rbp),%rcx
 9da:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
 9e1:	00 00 
 9e3:	74 05                	je     9ea <main+0xad>
 9e5:	e8 86 fb ff ff       	callq  570 <__stack_chk_fail@plt>
 9ea:	c9                   	leaveq 
 9eb:	c3                   	retq   
 9ec:	0f 1f 40 00          	nopl   0x0(%rax)

00000000000009f0 <__libc_csu_init>:
 9f0:	41 57                	push   %r15
 9f2:	41 56                	push   %r14
 9f4:	49 89 d7             	mov    %rdx,%r15
 9f7:	41 55                	push   %r13
 9f9:	41 54                	push   %r12
 9fb:	4c 8d 25 ae 03 20 00 	lea    0x2003ae(%rip),%r12        # 200db0 <__frame_dummy_init_array_entry>
 a02:	55                   	push   %rbp
 a03:	48 8d 2d ae 03 20 00 	lea    0x2003ae(%rip),%rbp        # 200db8 <__init_array_end>
 a0a:	53                   	push   %rbx
 a0b:	41 89 fd             	mov    %edi,%r13d
 a0e:	49 89 f6             	mov    %rsi,%r14
 a11:	4c 29 e5             	sub    %r12,%rbp
 a14:	48 83 ec 08          	sub    $0x8,%rsp
 a18:	48 c1 fd 03          	sar    $0x3,%rbp
 a1c:	e8 27 fb ff ff       	callq  548 <_init>
 a21:	48 85 ed             	test   %rbp,%rbp
 a24:	74 20                	je     a46 <__libc_csu_init+0x56>
 a26:	31 db                	xor    %ebx,%ebx
 a28:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
 a2f:	00 
 a30:	4c 89 fa             	mov    %r15,%rdx
 a33:	4c 89 f6             	mov    %r14,%rsi
 a36:	44 89 ef             	mov    %r13d,%edi
 a39:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
 a3d:	48 83 c3 01          	add    $0x1,%rbx
 a41:	48 39 dd             	cmp    %rbx,%rbp
 a44:	75 ea                	jne    a30 <__libc_csu_init+0x40>
 a46:	48 83 c4 08          	add    $0x8,%rsp
 a4a:	5b                   	pop    %rbx
 a4b:	5d                   	pop    %rbp
 a4c:	41 5c                	pop    %r12
 a4e:	41 5d                	pop    %r13
 a50:	41 5e                	pop    %r14
 a52:	41 5f                	pop    %r15
 a54:	c3                   	retq   
 a55:	90                   	nop
 a56:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 a5d:	00 00 00 

0000000000000a60 <__libc_csu_fini>:
 a60:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000000a64 <_fini>:
 a64:	48 83 ec 08          	sub    $0x8,%rsp
 a68:	48 83 c4 08          	add    $0x8,%rsp
 a6c:	c3                   	retq   

```

### 栈帧调用图解分析

![merge-sort-stack-frame-analysi](https://gitee.com/stardustman/pictrues/raw/master/img/merge-sort-stack-frame-analysis.svg)

> 栈帧图分析
> mergeSort_L 对应的是左半侧的 merge。mid 为红色
> mergeSort_R 对应的是右半侧的 merge。mid 为蓝色
> merge 对应的是最下层的浅绿色，绿色，深绿色


### array 归并顺序图解分析

![merge-sort-analysis](https://gitee.com/stardustman/pictrues/raw/master/img/merge-sort-analysis.svg)

> 可以看到对于 array 来说，确实是一半一半 `merge` 的, 这就是分治。

### strace 分析

```
root@aliyun:~# strace ./mergesort 
execve("./mergesort", ["./mergesort"], 0x7fff5d5bf0c0 /* 27 vars */) = 0
brk(NULL)                               = 0x5583b9756000
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=39490, ...}) = 0
mmap(NULL, 39490, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f4ed0097000
close(3)                                = 0
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\260\34\2\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0755, st_size=2030544, ...}) = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f4ed0095000
mmap(NULL, 4131552, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f4ecfa89000
mprotect(0x7f4ecfc70000, 2097152, PROT_NONE) = 0
mmap(0x7f4ecfe70000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1e7000) = 0x7f4ecfe70000
mmap(0x7f4ecfe76000, 15072, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f4ecfe76000
close(3)                                = 0
arch_prctl(ARCH_SET_FS, 0x7f4ed00964c0) = 0
mprotect(0x7f4ecfe70000, 16384, PROT_READ) = 0
mprotect(0x5583b9675000, 4096, PROT_READ) = 0
mprotect(0x7f4ed00a1000, 4096, PROT_READ) = 0
munmap(0x7f4ed0097000, 39490)           = 0
fstat(1, {st_mode=S_IFCHR|0600, st_rdev=makedev(136, 0), ...}) = 0
brk(NULL)                               = 0x5583b9756000
brk(0x5583b9777000)                     = 0x5583b9777000
write(1, "3 4 5 6 7 8 9 10 ", 173 4 5 6 7 8 9 10 )       = 17
exit_group(0)                           = ?
+++ exited with 0 +++

```







## reference
1. [why-does-this-memory-address-fs0x28-fs0x28-have-a-random-value](https://stackoverflow.com/questions/10325713/why-does-this-memory-address-fs0x28-fs0x28-have-a-random-value)
2. [stack-smashing](https://stackoverflow.com/questions/1345670/stack-smashing-detected)
3. [jge](https://www.aldeid.com/wiki/X86-assembly/Instructions/jge)
4. [cmp](https://www.aldeid.com/wiki/X86-assembly/Instructions/cmp)
5. [assembly-guide](https://www.cs.yale.edu/flint/cs421/papers/x86-asm/asm.html)
6. [Shift Arithmetic Right](http://www.c-jump.com/CIS77/ASM/Flags/F77_0160_sar_instruction.htm)
7. [x86-asm-leave](https://stackoverflow.com/questions/29790175/assembly-x86-leave-instruction/29790275)
8. [x86-instructions-guide](https://web.stanford.edu/class/archive/cs/cs107/cs107.1166/guide_x86-64.html)
9. [Notes on x86-64 programming](https://www.lri.fr/~filliatr/ens/compil/x86-64.pdf)
10. [x86-multiply](https://www.felixcloutier.com/x86/imul)
11. [x64_cheatsheet](https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf)
12. [CS107 Spring 2019](https://web.stanford.edu/class/archive/cs/cs107/cs107.1196/lectures/)
13. [CS107, Lecture 12 Assembly: Arithmetic, Logic and Condition Codes](https://web.stanford.edu/class/archive/cs/cs107/cs107.1196/lectures/12/Lecture12.pdf)
14. [x86-flags](https://riptutorial.com/x86/example/6976/flags-register)
15. [xor](https://c9x.me/x86/html/file_module_x86_id_330.html)
16. [x64-into](https://cs.nyu.edu/courses/fall11/CSCI-GA.2130-001/x64-intro.pdf)
17. [归并排序](https://mp.weixin.qq.com/s/885uGVhlffWAxjgIEW-TiA)
