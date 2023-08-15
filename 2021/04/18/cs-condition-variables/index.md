
## condition variable 是啥？

condition variable 是啥？本质上就是一个`状态变量` +`队列`。现实世界中，想要进行下一步的行动，往往需要满足一定的条件(condition)。如十字路口的交通信号灯，信号灯的颜色可以视为状态变量，根据不同的状态，汽车做出不同的选择。一条马路，可以视为`队列`。红灯时, 汽车就不能通过，排队等候。绿灯时，汽车才可以通过。在计算机中，同样存在这样的问题，如父进程需要等待`子进程完成后(条件)`，才能继续运行。关键就是围绕`状态变量`来构建。

## Reference

1.[pthread_cond_wait](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html)
2.[pthread_cond_signal](https://man7.org/linux/man-pages/man3/pthread_cond_signal.3p.html)
