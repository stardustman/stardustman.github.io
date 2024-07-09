
# signal

## 什么是 signal

> Signals are various notifications sent to a process in order to notify it of various "important" events. 
> 信号是发送给进程的各种通知，以便通知它发生了各种“重要”的事件。

## 有哪些信号？默认怎么处理？

| Signal Name  | Default Action | Comment                                                       | POSIX |
| :----------- | :------------- | :------------------------------------------------------------ | :---- |
| 1 SIGHUP     | Terminate      | Hang up controlling terminal or process                       | Yes   |
| 2 SIGINT     | Terminate      | Interrupt from keyboard, Control-C                            | Yes   |
| 3 SIGQUIT    | Dump           | Quit from keyboard, Control-\                                 | Yes   |
| 4 SIGILL     | Dump           | Illegal instruction                                           | Yes   |
| 5 SIGTRAP    | Dump           | Breakpoint for debugging                                      | No    |
| 6 SIGABRT    | Dump           | Abnormal termination                                          | Yes   |
| 6 SIGIOT     | Dump           | Equivalent to SIGABRT                                         | No    |
| 7 SIGBUS     | Dump           | Bus error                                                     | No    |
| 8 SIGFPE     | Dump           | Floating-point exception                                      | Yes   |
| 9 `SIGKILL`  | Terminate      | Forced-process termination                                    | Yes   |
| 10 SIGUSR1   | Terminate      | Available to processes                                        | Yes   |
| 11 SIGSEGV   | Dump           | Invalid memory reference                                      | Yes   |
| 12 SIGUSR2   | Terminate      | Available to processes                                        | Yes   |
| 13 SIGPIPE   | Terminate      | Write to pipe with no readers                                 | Yes   |
| 14 SIGALRM   | Terminate      | Real-timer clock                                              | Yes   |
| 15 SIGTERM   | Terminate      | Process termination                                           | Yes   |
| 16 SIGSTKFLT | Terminate      | Coprocessor stack error                                       | No    |
| 17 SIGCHLD   | Ignore         | Child process stopped or terminated or got a signal if traced | Yes   |
| 18 SIGCONT   | Continue       | Resume execution, if stopped                                  | Yes   |
| 19 `SIGSTOP` | Stop           | Stop process execution, Ctrl-Z                                | Yes   |
| 20 SIGTSTP   | Stop           | Stop process issued from tty                                  | Yes   |
| 21 SIGTTIN   | Stop           | Background process requires input                             | Yes   |
| 22 SIGTTOU   | Stop           | Background process requires output                            | Yes   |
| 23 SIGURG    | Ignore         | Urgent condition on socket                                    | No    |
| 24 SIGXCPU   | Dump           | CPU time limit exceeded                                       | No    |
| 25 SIGXFSZ   | Dump           | File size limit exceeded                                      | No    |
| 26 SIGVTALRM | Terminate      | Virtual timer clock                                           | No    |
| 27 SIGPROF   | Terminate      | Profile timer clock                                           | No    |
| 28 SIGWINCH  | Ignore         | Window resizing                                               | No    |
| 29 SIGIO     | Terminate      | I/O now possible                                              | No    |
| 29 SIGPOLL   | Terminate      | Equivalent to SIGIO                                           | No    |
| 30 SIGPWR    | Terminate      | Power supply failure                                          | No    |
| 31 SIGSYS    | Dump           | Bad system call                                               | No    |
| 31SIGUNUSED  | Dump           | Equivalent to SIGSYS                                          | No    |

## 如何发送信号？

### 1. keyboard
1. SIGINT(Ctrl + C)
2. SIGTSTP(Ctrl + Z) -> resume by `fg`
3. SIGQUIT(Ctrl + \\)

### 2. command line
1.  kill -\<signal\> \<PID\>
2.  fg 

### 3. system call

[kill syscall](https://man7.org/linux/man-pages/man2/kill.2.html)


## 进程收到信号会怎么处理？

分为 `3` 种情况:

1. **`Ignore it`**. Many signals can be and are ignored, but not all. Hardware exceptions such as "divide by 0" (with integers) cannot be ignored successfully and some signals such as SIGKILL cannot be ignored at all.
2. **`Catch and handle the exception`**. The process has a function to be executed if and when the exception occurs. The function may terminate the program gracefully or it may handle it without terminating the program.
3. **`Let the default action apply`**. Every signal has a default action. The default may be:
   - ignore
   - terminate
   - terminate and dump core
   - stop or pause the program
   - resume a program paused earlier

## 如何自定义 signal handler?

> 注意：这里只有可以被 caught 的才可以自定义 handler。SIGKILL 和 SIGSTOP 这两个是不能自己定义 handler 的。

### java example

```java
import sun.misc.Signal;
import sun.misc.SignalHandler;

public class ExampleSignalHandler {
    public static void main(String... args) throws InterruptedException {
        final long start = System.nanoTime();
        Signal.handle(new Signal("INT"), new SignalHandler() {
            public void handle(Signal sig) {
                System.out.format("\nProgram execution took %f seconds\n", (System.nanoTime() - start) / 1e9f);
                System.out.println("cat the INT signal, but still run.....😄");
            }
        });
        int counter = 0;
        while(true) {
            System.out.println(counter++);
            Thread.sleep(500);
        }
    }
}
```

1. 可以将 “INT” 替换为 “STOP” 运行结果为:
   ```bash
   Exception in thread "main" java.lang.IllegalArgumentException: Signal already used by VM or OS: SIGSTOP
	at sun.misc.Signal.handle(Signal.java:166)
	at ExampleSignalHandler.main(ExampleSignalHandler.java:9)
   ```
2. 可以将 “INT” 替换为 “KILL” 运行结果为:
   ```bash
   Exception in thread "main" java.lang.IllegalArgumentException: Signal already used by VM or OS: SIGKILL
	at sun.misc.Signal.handle(Signal.java:166)
	at stardustman.github.io.signal.ExampleSignalHandler.main(ExampleSignalHandler.java:9)
   ```



# References


1. [Linux Signal Table](https://chromium.googlesource.com/chromiumos/docs/+/HEAD/constants/signals.md)
2. [Signal_(IPC)](https://en.wikipedia.org/wiki/Signal_(IPC))
3. [Introduction To Unix Signals Programming](https://www.cs.kent.edu/~ruttan/sysprog/lectures/signals.html)
4. [Standard-Signals](https://www.gnu.org/software/libc/manual/html_node/Standard-Signals.html)
5. [handle-a-signal](https://rosettacode.org/wiki/Handle_a_signal#Java)
6. [signal-block](https://man7.org/linux/man-pages/man2/sigprocmask.2.html)
7. [handling-unix-signals-in-python](https://stackabuse.com/handling-unix-signals-in-python/)
8. [signals](https://faculty.cs.niu.edu/~hutchins/csci480/signals.htm)
9. [signal-handling-in-linux](http://www.alexonlinux.com/signal-handling-in-linux)
10. [nohup](https://elixir.bootlin.com/busybox/latest/source/coreutils/nohup.c#L94)
11. [https://www.cs.princeton.edu/courses/archive/fall05/cos217/lectures/23signals.pdf](https://www.cs.princeton.edu/courses/archive/fall05/cos217/lectures/23signals.pdf)
12. [cs.kent.edu/signals.html](https://www.cs.kent.edu/~ruttan/sysprog/lectures/signals.html)
13. [csci480/signals.htm](https://faculty.cs.niu.edu/~hutchins/csci480/signals.htm)
14. [kill(2)](https://man7.org/linux/man-pages/man2/kill.2.html)
15. https://goodyduru.github.io/os/2023/10/05/ipc-unix-signals.html
16. [java Signal 处理实现](https://github.com/JetBrains/jdk8u_jdk/blob/94318f9185757cc33d2b8d527d36be26ac6b7582/src/share/classes/sun/misc/Signal.java#L30)
