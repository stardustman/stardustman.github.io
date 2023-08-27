
# jvm

> jvm 是一个栈式(stack-based)虚拟计算机。啥意思，就是大多数的 opcode 的操作数在 operand stack 上，执行的结果也放在 oprand stack 上。<br>
> 有的 opcode 的操作数在 local variable table，如 `iinc`。

# 分析的代码

```java
package stardustman.github.io;

public class BoxingUnboxingExample {
    public static void main(String[] args) {
        Integer sum = 0;
        for (int i = 1; i <= 1000000; i++) {
            sum += i; // Auto-boxing of primitive type int to Integer
        }
        System.out.println("The sum is: " + sum);
    }
}
```

# jclasslib 查看字节码

![BoxingUnboxingExample](https://raw.githubusercontent.com/stardustman/pictures/main/img/BoxingUnboxingExample.jpg)

# 字节码执行过程

![jvm-byte-code-execute](https://raw.githubusercontent.com/stardustman/pictures/main/img/jvm%20-byte-code-execute.svg)



# References

1. [postfix-expression](https://www.geeksforgeeks.org/evaluation-of-postfix-expression/)
2. [jclasslib](https://github.com/ingokegel/jclasslib/releases)
3. [jvms-6.5.iconst_i](https://docs.oracle.com/javase/specs/jvms/se19/html/jvms-6.html#jvms-6.5.iconst_i)
4. [jvms-6.5.invokestatic](https://docs.oracle.com/javase/specs/jvms/se19/html/jvms-6.html#jvms-6.5.invokestatic)
5. [jvms-6.5.getstatic](https://docs.oracle.com/javase/specs/jvms/se19/html/jvms-6.html#jvms-6.5.getstatic)
