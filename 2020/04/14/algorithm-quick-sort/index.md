
## quick sort

### C 代码

```c
#include <stdio.h>

int partition(int* array, int startIndex, int endIndex){
    int left = startIndex;
    int right = endIndex;
    int pivot = array[startIndex];
    int temp = 0;
    // 第一个 while 循环，其实就是从两个方向遍历一次数组，根据 pivot 进行分割。
    while (left != right)
    {
        // 第二个 while 循环， 找一个 array[right] <= pivot，也就是找一个小于等于 pivot 的元素
        while (left < right && array[right] > pivot)
        {
            right--;
        }

        // 第三个 while 循环， 找一个 array[left] > pivot，也就是找一个大于等于 pivot 的元素。
        while (left < right && array[left] <= pivot)
        {
            left++;
        }
        
        // 交换找到的两个元素
        if(left < right) {
            temp = array[left];
            array[left] = array[right];
            array[right] = temp;
        }
        
    }
   
   // pivot 归位, 左边都比 pivot 小， 右边都比 pivot 大
   temp = array[left];
   array[left] = pivot;
   array[startIndex] = temp;
   return left;
}

void quickSort(int* array, int startIndex, int endIndex){
    // 只有一个元素，有序。直接返回
    if(startIndex >= endIndex){
        return;
    }
    // 一次 partition 有序一个元素。被 pivot 分割的两部分继续 quick sort
    int pivotIndex = partition(array, startIndex, endIndex);
    quickSort(array, startIndex, pivotIndex - 1);
    quickSort(array, pivotIndex + 1, endIndex);
}

int main(){
    int array[8] = {4,7,6,5,3,2,8,1};
    quickSort(array, 0, 7);
    for (int i = 0; i < 8; i++)
    {
        printf("%d ",array[i]);
    }
    return 0;
}
```

### 汇编代码分析

```asm
quickSort:     file format elf64-x86-64


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

Disassembly of section .text:

00000000000005a0 <_start>:
 5a0:	31 ed                	xor    %ebp,%ebp ; ebp = 0
 5a2:	49 89 d1             	mov    %rdx,%r9  ; r9 = rdx
 5a5:	5e                   	pop    %rsi
 5a6:	48 89 e2             	mov    %rsp,%rdx ; rdx = rsp
 5a9:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
 5ad:	50                   	push   %rax
 5ae:	54                   	push   %rsp
 5af:	4c 8d 05 ca 03 00 00 	lea    0x3ca(%rip),%r8        # 980 <__libc_csu_fini>
 5b6:	48 8d 0d 53 03 00 00 	lea    0x353(%rip),%rcx        # 910 <__libc_csu_init>
 5bd:	48 8d 3d 9c 02 00 00 	lea    0x29c(%rip),%rdi        # 860 <main>
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

00000000000006aa <partition>:
 6aa:	55                   	push   %rbp
 6ab:	48 89 e5             	mov    %rsp,%rbp
 6ae:	48 89 7d e8          	mov    %rdi,-0x18(%rbp) ; &array[0]
 6b2:	89 75 e4             	mov    %esi,-0x1c(%rbp) ; startIndex
 6b5:	89 55 e0             	mov    %edx,-0x20(%rbp) ; endIndex
 6b8:	8b 45 e4             	mov    -0x1c(%rbp),%eax ; eax = startIndex
 6bb:	89 45 f0             	mov    %eax,-0x10(%rbp) ; left = startIndex
 6be:	8b 45 e0             	mov    -0x20(%rbp),%eax ; eax = endIndex
 6c1:	89 45 f4             	mov    %eax,-0xc(%rbp)  ; right = endIndex
 6c4:	8b 45 e4             	mov    -0x1c(%rbp),%eax ; eax = startIndex
 6c7:	48 98                	cltq                    
 6c9:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx ; rdx = 4 * startIndex
 6d0:	00 
 6d1:	48 8b 45 e8          	mov    -0x18(%rbp),%rax ; rax = &array[0]
 6d5:	48 01 d0             	add    %rdx,%rax        ; rax = &array[0] + 4 * startIndex
 6d8:	8b 00                	mov    (%rax),%eax      ; eax = array[startIndex]
 6da:	89 45 f8             	mov    %eax,-0x8(%rbp)  ; pivot = array[startIndex]
 6dd:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)  ; temp = 0;
 6e4:	e9 b7 00 00 00       	jmpq   7a0 <partition+0xf6> ; 跳转到 while(left != right) 判断 left != right

; 第二个 while 循环的循环体
 6e9:	83 6d f4 01          	subl   $0x1,-0xc(%rbp)  ; right = right - 1

; while(left != right) 的循环体
 6ed:	8b 45 f0             	mov    -0x10(%rbp),%eax ; eax = left
 6f0:	3b 45 f4             	cmp    -0xc(%rbp),%eax  ; cmp right,left
 6f3:	7d 21                	jge    716 <partition+0x6c> ; left >= right，第二个 while 循环不满足，进入判断第三个 while 循环
 6f5:	8b 45 f4             	mov    -0xc(%rbp),%eax  ; eax = right 能执行到这，说明 left < right
 6f8:	48 98                	cltq                    ; convert eax to rax
 6fa:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx; rdx = 4 * right
 701:	00 
 702:	48 8b 45 e8          	mov    -0x18(%rbp),%rax ; rax = &array[0]
 706:	48 01 d0             	add    %rdx,%rax        ; rax = &array[0] + 4 * right
 709:	8b 00                	mov    (%rax),%eax      ; eax = array[right]
 70b:	39 45 f8             	cmp    %eax,-0x8(%rbp)  ; cmp array[right],pivot
 70e:	7c d9                	jl     6e9 <partition+0x3f> ; pivot < array[right] 执行跳转，执行循环体。
 710:	eb 04                	jmp    716 <partition+0x6c> ; 

; 第三个 while 循环的循环体
 712:	83 45 f0 01          	addl   $0x1,-0x10(%rbp) ; left = left + 1
 
 ; 第三个 while 循环
 716:	8b 45 f0             	mov    -0x10(%rbp),%eax ; eax = left
 719:	3b 45 f4             	cmp    -0xc(%rbp),%eax  ; cmp,right,left
 71c:	7d 1b                	jge    739 <partition+0x8f> ; left >= right，第三个 while 循环 条件 1 不满足，短路。进入 if 语句。
 71e:	8b 45 f0             	mov    -0x10(%rbp),%eax ; eax = left
 721:	48 98                	cltq   
 723:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx ; rdx = 4 * left
 72a:	00 
 72b:	48 8b 45 e8          	mov    -0x18(%rbp),%rax  ; rax = &array[0]
 72f:	48 01 d0             	add    %rdx,%rax         ; rax = &array[0] + 4 * left
 732:	8b 00                	mov    (%rax),%eax       ; eax = array[left]
 734:	39 45 f8             	cmp    %eax,-0x8(%rbp)   ; cmp,array[left],pivot
 737:	7d d9                	jge    712 <partition+0x68> ; pivot >= array[left]，第三个 while 循环 条件 2 满足。执行循环体。

 ; if 代码块开始
 739:	8b 45 f0             	mov    -0x10(%rbp),%eax  ; eax = left
 73c:	3b 45 f4             	cmp    -0xc(%rbp),%eax   ; cmp right,left
 73f:	7d 5f                	jge    7a0 <partition+0xf6> ; left >= right 进入下一轮循环
 741:	8b 45 f0             	mov    -0x10(%rbp),%eax  ; eax = left
 744:	48 98                	cltq   
 746:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx ; rdx = 4 * left
 74d:	00 
 74e:	48 8b 45 e8          	mov    -0x18(%rbp),%rax  ; rax = &array[0]
 752:	48 01 d0             	add    %rdx,%rax         ; rax = &array[0] + 4 * left
 755:	8b 00                	mov    (%rax),%eax       ; eax = array[left]
 757:	89 45 fc             	mov    %eax,-0x4(%rbp)   ; temp = array[left]
 75a:	8b 45 f4             	mov    -0xc(%rbp),%eax   ; eax = right
 75d:	48 98                	cltq   
 75f:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx ; rdx = 4 * right
 766:	00 
 767:	48 8b 45 e8          	mov    -0x18(%rbp),%rax  ; rax = &array[0]
 76b:	48 01 d0             	add    %rdx,%rax         ; rax = &array[0] + 4 * right
 76e:	8b 55 f0             	mov    -0x10(%rbp),%edx  ; edx = left
 771:	48 63 d2             	movslq %edx,%rdx         ; 
 774:	48 8d 0c 95 00 00 00 	lea    0x0(,%rdx,4),%rcx ; rcx = 4 * left
 77b:	00 
 77c:	48 8b 55 e8          	mov    -0x18(%rbp),%rdx  ; rdx = &array[0]
 780:	48 01 ca             	add    %rcx,%rdx         ; rdx = &array[0] + 4 * left
 783:	8b 00                	mov    (%rax),%eax       ; eax = array[right]
 785:	89 02                	mov    %eax,(%rdx)       ; array[left] = array[right]
 787:	8b 45 f4             	mov    -0xc(%rbp),%eax   ; eax = right
 78a:	48 98                	cltq   
 78c:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx ; rdx = 4 * right
 793:	00 
 794:	48 8b 45 e8          	mov    -0x18(%rbp),%rax  ; rax = &array[0]
 798:	48 01 c2             	add    %rax,%rdx         ; rdx = &array[0] + 4 * right
 79b:	8b 45 fc             	mov    -0x4(%rbp),%eax   ; eax = temp;
 79e:	89 02                	mov    %eax,(%rdx)       ; array[right] = temp
 ; if 代码块结束

; while(left != right) 进入循环的条件判断
 7a0:	8b 45 f0             	mov    -0x10(%rbp),%eax ; eax = left
 7a3:	3b 45 f4             	cmp    -0xc(%rbp),%eax  ; cpm right, left
 7a6:	0f 85 41 ff ff ff    	jne    6ed <partition+0x43> ; left != right 满足循环条件进入循环体

 7ac:	8b 45 f0             	mov    -0x10(%rbp),%eax ; eax = left  不满足循环条件 left != right，此时循环结束。
 
 ; pivot 归位
 7af:	48 98                	cltq   
 7b1:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx ; rdx = 4 * left
 7b8:	00 
 7b9:	48 8b 45 e8          	mov    -0x18(%rbp),%rax  ; rax = &array[0]
 7bd:	48 01 d0             	add    %rdx,%rax         ; rax = &array[0] + 4 * left
 7c0:	8b 00                	mov    (%rax),%eax       ; eax = array[left]
 7c2:	89 45 fc             	mov    %eax,-0x4(%rbp)   ; temp = array[left]
 7c5:	8b 45 f0             	mov    -0x10(%rbp),%eax  ; eax = left
 7c8:	48 98                	cltq   
 7ca:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx ; rdx = 4 * left
 7d1:	00 
 7d2:	48 8b 45 e8          	mov    -0x18(%rbp),%rax  ; rax = &array[0]
 7d6:	48 01 c2             	add    %rax,%rdx         ; rdx = &array[0] + 4 * left
 7d9:	8b 45 f8             	mov    -0x8(%rbp),%eax   ; eax = pivot
 7dc:	89 02                	mov    %eax,(%rdx)       ; array[left] = pivot
 7de:	8b 45 e4             	mov    -0x1c(%rbp),%eax  ; eax = startIndex
 7e1:	48 98                	cltq   
 7e3:	48 8d 14 85 00 00 00 	lea    0x0(,%rax,4),%rdx ; rdx = 4 * startIndex
 7ea:	00 
 7eb:	48 8b 45 e8          	mov    -0x18(%rbp),%rax  ; rax = &array[0]
 7ef:	48 01 c2             	add    %rax,%rdx         ; rdx = &array[0] + 4 * startIndex
 7f2:	8b 45 fc             	mov    -0x4(%rbp),%eax   ; eax = temp
 7f5:	89 02                	mov    %eax,(%rdx)       ; array[startIndex] = temp
 7f7:	8b 45 f0             	mov    -0x10(%rbp),%eax  ; eax = left 返回值
 7fa:	5d                   	pop    %rbp              ; pop 调用者的 rbp
 7fb:	c3                   	retq                     ; 1. pop %rip 2.jmp %rip

00000000000007fc <quickSort>:
 7fc:	55                   	push   %rbp
 7fd:	48 89 e5             	mov    %rsp,%rbp
 800:	48 83 ec 20          	sub    $0x20,%rsp ;分配 32Bytes 空间
 804:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)  ; &array[0]
 808:	89 75 e4             	mov    %esi,-0x1c(%rbp)  ; startIndex
 80b:	89 55 e0             	mov    %edx,-0x20(%rbp)  ; endIndex
 80e:	8b 45 e4             	mov    -0x1c(%rbp),%eax  ; eax = startIndex
 811:	3b 45 e0             	cmp    -0x20(%rbp),%eax  ; cmp endIndex,startIndex
 814:	7d 47                	jge    85d <quickSort+0x61> ; startIndex >= endIndex 不满足条件，销毁栈帧，结束调用。
 816:	8b 55 e0             	mov    -0x20(%rbp),%edx  ; edx = endIndex   第三个参数 能执行到这，说明 startIndex < endIndex
 819:	8b 4d e4             	mov    -0x1c(%rbp),%ecx  ; ecx = startIndex
 81c:	48 8b 45 e8          	mov    -0x18(%rbp),%rax  ; rax = &array[0]
 820:	89 ce                	mov    %ecx,%esi         ; esi = startIndex 第二个参数
 822:	48 89 c7             	mov    %rax,%rdi         ; rdi = &array[0]  第一个参数
 825:	e8 80 fe ff ff       	callq  6aa <partition>

 82a:	89 45 fc             	mov    %eax,-0x4(%rbp)   ; pivotIndex = eax partition 的返回值在 eax 里
 82d:	8b 45 fc             	mov    -0x4(%rbp),%eax   ; eax = pivotIndex
 830:	8d 50 ff             	lea    -0x1(%rax),%edx   ; edx = pivotIndex - 1 第三个参数
 833:	8b 4d e4             	mov    -0x1c(%rbp),%ecx  ; ecx = startIndex
 836:	48 8b 45 e8          	mov    -0x18(%rbp),%rax  ; rax = &array[0]
 83a:	89 ce                	mov    %ecx,%esi         ; esi = startIndex     第二个参数
 83c:	48 89 c7             	mov    %rax,%rdi         ; rdi = &array[0]      第一个参数
 83f:	e8 b8 ff ff ff       	callq  7fc <quickSort>   ; quickSort(array, startIndex, pivotIndex - 1)

 844:	8b 45 fc             	mov    -0x4(%rbp),%eax   ; eax = pivotIndex
 847:	8d 48 01             	lea    0x1(%rax),%ecx    ; ecx = pivotIndex + 1
 84a:	8b 55 e0             	mov    -0x20(%rbp),%edx  ; edx = endIndex       第三个参数
 84d:	48 8b 45 e8          	mov    -0x18(%rbp),%rax  ; rax = &array[0]
 851:	89 ce                	mov    %ecx,%esi         ; esi = pivotIndex + 1 第二个参数
 853:	48 89 c7             	mov    %rax,%rdi         ; rdi = &array[0]      第一个参数
 856:	e8 a1 ff ff ff       	callq  7fc <quickSort>   ; quickSort(array, pivotIndex + 1, endIndex)

 85b:	eb 01                	jmp    85e <quickSort+0x62>

 85d:	90                   	nop
 85e:	c9                   	leaveq 
 85f:	c3                   	retq   

0000000000000860 <main>:
 860:	55                   	push   %rbp
 861:	48 89 e5             	mov    %rsp,%rbp
 864:	48 83 ec 40          	sub    $0x40,%rsp ; 分配 64Bytes 空间。 
 868:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
 86f:	00 00 
 871:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
 875:	31 c0                	xor    %eax,%eax
 877:	c7 45 d0 04 00 00 00 	movl   $0x4,-0x30(%rbp) ; array[0] = 4
 87e:	c7 45 d4 07 00 00 00 	movl   $0x7,-0x2c(%rbp) ; array[1] = 7
 885:	c7 45 d8 06 00 00 00 	movl   $0x6,-0x28(%rbp) ; array[2] = 6
 88c:	c7 45 dc 05 00 00 00 	movl   $0x5,-0x24(%rbp) ; array[3] = 5
 893:	c7 45 e0 03 00 00 00 	movl   $0x3,-0x20(%rbp) ; array[4] = 3
 89a:	c7 45 e4 02 00 00 00 	movl   $0x2,-0x1c(%rbp) ; array[5] = 2
 8a1:	c7 45 e8 08 00 00 00 	movl   $0x8,-0x18(%rbp) ; array[6] = 8
 8a8:	c7 45 ec 01 00 00 00 	movl   $0x1,-0x14(%rbp) ; array[7] = 1
 8af:	48 8d 45 d0          	lea    -0x30(%rbp),%rax ; rax = &array[0]
 8b3:	ba 07 00 00 00       	mov    $0x7,%edx ; edx = endIndex = 7
 8b8:	be 00 00 00 00       	mov    $0x0,%esi ; esi = startIndex = 0
 8bd:	48 89 c7             	mov    %rax,%rdi ; rdi = rax = &array[0]
 8c0:	e8 37 ff ff ff       	callq  7fc <quickSort>
 8c5:	c7 45 cc 00 00 00 00 	movl   $0x0,-0x34(%rbp) ; i = 0
 8cc:	eb 20                	jmp    8ee <main+0x8e>
 8ce:	8b 45 cc             	mov    -0x34(%rbp),%eax
 8d1:	48 98                	cltq   
 8d3:	8b 44 85 d0          	mov    -0x30(%rbp,%rax,4),%eax
 8d7:	89 c6                	mov    %eax,%esi
 8d9:	48 8d 3d b4 00 00 00 	lea    0xb4(%rip),%rdi        # 994 <_IO_stdin_used+0x4>
 8e0:	b8 00 00 00 00       	mov    $0x0,%eax
 8e5:	e8 96 fc ff ff       	callq  580 <printf@plt>
 8ea:	83 45 cc 01          	addl   $0x1,-0x34(%rbp)
 8ee:	83 7d cc 07          	cmpl   $0x7,-0x34(%rbp)
 8f2:	7e da                	jle    8ce <main+0x6e>
 8f4:	b8 00 00 00 00       	mov    $0x0,%eax
 8f9:	48 8b 4d f8          	mov    -0x8(%rbp),%rcx
 8fd:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
 904:	00 00 
 906:	74 05                	je     90d <main+0xad>
 908:	e8 63 fc ff ff       	callq  570 <__stack_chk_fail@plt>
 90d:	c9                   	leaveq 
 90e:	c3                   	retq   
 90f:	90                   	nop

0000000000000910 <__libc_csu_init>:
 910:	41 57                	push   %r15
 912:	41 56                	push   %r14
 914:	49 89 d7             	mov    %rdx,%r15
 917:	41 55                	push   %r13
 919:	41 54                	push   %r12
 91b:	4c 8d 25 8e 04 20 00 	lea    0x20048e(%rip),%r12        # 200db0 <__frame_dummy_init_array_entry>
 922:	55                   	push   %rbp
 923:	48 8d 2d 8e 04 20 00 	lea    0x20048e(%rip),%rbp        # 200db8 <__init_array_end>
 92a:	53                   	push   %rbx
 92b:	41 89 fd             	mov    %edi,%r13d
 92e:	49 89 f6             	mov    %rsi,%r14
 931:	4c 29 e5             	sub    %r12,%rbp
 934:	48 83 ec 08          	sub    $0x8,%rsp
 938:	48 c1 fd 03          	sar    $0x3,%rbp
 93c:	e8 07 fc ff ff       	callq  548 <_init>
 941:	48 85 ed             	test   %rbp,%rbp
 944:	74 20                	je     966 <__libc_csu_init+0x56>
 946:	31 db                	xor    %ebx,%ebx
 948:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
 94f:	00 
 950:	4c 89 fa             	mov    %r15,%rdx
 953:	4c 89 f6             	mov    %r14,%rsi
 956:	44 89 ef             	mov    %r13d,%edi
 959:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
 95d:	48 83 c3 01          	add    $0x1,%rbx
 961:	48 39 dd             	cmp    %rbx,%rbp
 964:	75 ea                	jne    950 <__libc_csu_init+0x40>
 966:	48 83 c4 08          	add    $0x8,%rsp
 96a:	5b                   	pop    %rbx
 96b:	5d                   	pop    %rbp
 96c:	41 5c                	pop    %r12
 96e:	41 5d                	pop    %r13
 970:	41 5e                	pop    %r14
 972:	41 5f                	pop    %r15
 974:	c3                   	retq   
 975:	90                   	nop
 976:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 97d:	00 00 00 

0000000000000980 <__libc_csu_fini>:
 980:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000000984 <_fini>:
 984:	48 83 ec 08          	sub    $0x8,%rsp
 988:	48 83 c4 08          	add    $0x8,%rsp
 98c:	c3                   	retq   

```

### partition 分析

```
Before:: [4, 7, 6, 5, 3, 2, 8, 1]
第 1 次 partition [3, 1, 2, 4, 5, 6, 8, 7]  returned index is: 3
第 2 次 partition [2, 1, 3, 4, 5, 6, 8, 7]  returned index is: 2
第 3 次 partition [1, 2, 3, 4, 5, 6, 8, 7]  returned index is: 1
第 4 次 partition [1, 2, 3, 4, 5, 6, 8, 7]  returned index is: 4
第 5 次 partition [1, 2, 3, 4, 5, 6, 8, 7]  returned index is: 5
第 6 次 partition [1, 2, 3, 4, 5, 6, 7, 8]  returned index is: 7
 After:: [1, 2, 3, 4, 5, 6, 7, 8]
```

#### partition 逻辑图解

![quick-sort-partition-flow-chart ](https://github.com/stardustman/pictures/raw/main/img/quick-sort-partition-flow-chart.svg)


### 系统调用 strace 分析

```
root@aliyun:~# strace -Ti ./quickSort 
[00007f2ee3f68e37] execve("./quickSort", ["./quickSort"], 0x7ffffe4dc528 /* 27 vars */) = 0 <0.000122>
[00007f0045366ec9] brk(NULL)            = 0x55e15d32b000 <0.000006>
[00007f004535a7de] access("/etc/ld.so.nohwcap", F_OK) = -1 ENOENT (No such file or directory) <0.000010>
[00007f0045367e27] access("/etc/ld.so.preload", R_OK) = -1 ENOENT (No such file or directory) <0.000007>
[00007f0045367cdd] openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3 <0.000008>
[00007f0045367c43] fstat(3, {st_mode=S_IFREG|0644, st_size=39490, ...}) = 0 <0.000006>
[00007f0045367f43] mmap(NULL, 39490, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f0045568000 <0.000009>
[00007f0045367ed7] close(3)             = 0 <0.000005>
[00007f0045363139] access("/etc/ld.so.nohwcap", F_OK) = -1 ENOENT (No such file or directory) <0.000008>
[00007f0045367cdd] openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3 <0.000012>
[00007f0045367da4] read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\260\34\2\0\0\0\0\0"..., 832) = 832 <0.000007>
[00007f0045367c43] fstat(3, {st_mode=S_IFREG|0755, st_size=2030544, ...}) = 0 <0.000006>
[00007f0045367f43] mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f0045566000 <0.000007>
[00007f0045367f43] mmap(NULL, 4131552, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f0044f5a000 <0.000008>
[00007f0045367ff7] mprotect(0x7f0045141000, 2097152, PROT_NONE) = 0 <0.000012>
[00007f0045367f43] mmap(0x7f0045341000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1e7000) = 0x7f0045341000 <0.000011>
[00007f0045367f43] mmap(0x7f0045347000, 15072, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f0045347000 <0.000008>
[00007f0045367ed7] close(3)             = 0 <0.000007>
[00007f004534c024] arch_prctl(ARCH_SET_FS, 0x7f00455674c0) = 0 <0.000006>
[00007f0045367ff7] mprotect(0x7f0045341000, 16384, PROT_READ) = 0 <0.000009>
[00007f0045367ff7] mprotect(0x55e15d271000, 4096, PROT_READ) = 0 <0.000008>
[00007f0045367ff7] mprotect(0x7f0045572000, 4096, PROT_READ) = 0 <0.000011>
[00007f0045367fd7] munmap(0x7f0045568000, 39490) = 0 <0.000015>
[00007f00450697c3] fstat(1, {st_mode=S_IFCHR|0600, st_rdev=makedev(136, 0), ...}) = 0 <0.000006>
[00007f00450704b9] brk(NULL)            = 0x55e15d32b000 <0.000006>
[00007f00450704b9] brk(0x55e15d34c000)  = 0x55e15d34c000 <0.000007>
[00007f004506a154] write(1, "1 2 3 4 5 6 7 8 ", 161 2 3 4 5 6 7 8 ) = 16 <0.000024>
[00007f004503ee06] exit_group(0)        = ?
[????????????????] +++ exited with 0 +++
```

### reference
1. [system-call-execve](http://man7.org/linux/man-pages/man2/execve.2.html)
2. [system-call-brk](http://man7.org/linux/man-pages/man2/brk.2.html)
3. [system-call-access](http://man7.org/linux/man-pages/man2/access.2.html)