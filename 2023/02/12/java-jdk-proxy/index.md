> 分析环境: jdk8

# dynamic proxy 是啥？

> A dynamic proxy class is a class that implements `a list of interfaces`<sub>**1**</sub> specified at `runtime `such that a method invocation through one of the interfaces on an instance of the class will be `encoded and dispatched` to `another` object through a `uniform interface`<sub>**2**</sub>. 

1. 动态代理类生成调用方法如下：
```java
Proxy.newProxyInstance(handler.getClass().getClassLoader(), new Class[]{Dynasty.class},handler);
```
  - `handler.getClass().getClassLoader()` 运行时动态生成的代理类 load 到 jvm 使用的 class loader。
  - `new Class[]{Dynasty.class}` 运行时动态生成的代理类实现的一系列接口。
  - `handler` Invocation Handler 通过 invoke() 来分发方法，包含被实际代理的对象实例。

# 保存运行时生成的动态代理类

## ProxyGenerator.java

```java
    /** debugging flag for saving generated class files */
    private final static boolean saveGeneratedFiles =
        java.security.AccessController.doPrivileged(
            new GetBooleanAction(
                "sun.misc.ProxyGenerator.saveGeneratedFiles")).booleanValue();
```
## 设置 `sun.misc.ProxyGenerator.saveGeneratedFiles`

```java
System.setProperty("sun.misc.ProxyGenerator.saveGeneratedFiles", "true");
```

# 运行时生成的动态代理类($Proxy0)分析

## 运行时生成的动态代理类实例化

![dynamic-proxy-instance](https://raw.githubusercontent.com/stardustman/pictures/main/img/dynamic-proxy-instance.jpg)

$Proxy0 -> Proxy(InvocationHandler h) 由此可见:
1. 动态代理类 `$Proxy0` 实例化时，调用 super class `Proxy` 的构造函数，需要的 InvocationHandler 是实现了该接口的 `DynastyDynamicProxy`
2. `DynastyDynamicProxy` 实例化时，构造函数需要的是 `TangDynasty` 这个实际被代理的类。

![dynamic-proxy-class-method-founder](https://raw.githubusercontent.com/stardustman/pictures/main/img/dynamic-proxy-class-method-founder.jpg)

## 运行时生成的动态代理类方法

### $Proxy0 的命名

```java
        // prefix for all proxy class names
        private static final String proxyClassNamePrefix = "$Proxy";

        // next number to use for generation of unique proxy class names
        private static final AtomicLong nextUniqueNumber = new AtomicLong();
```

###  $Proxy0#founder

![dynamic-proxy-stack-frame-1](https://raw.githubusercontent.com/stardustman/pictures/main/img/dynamic-proxy-stack-frame-1.jpg)

---

![](https://raw.githubusercontent.com/stardustman/pictures/main/img/dynamic-proxy-stack-frame-2.jpg)


```java
    public final String founder() throws  {
        try {
            return (String)super.h.invoke(this, m4, (Object[])null); // super.h.invoke 指的就是 DynastyDynamicProxy.invoke
        } catch (RuntimeException | Error var2) {
            throw var2;
        } catch (Throwable var3) {
            throw new UndeclaredThrowableException(var3);
        }
    }
```

### $Proxy0#reviver

```java
    public final String reviver() throws  {
        try {
            return (String)super.h.invoke(this, m5, (Object[])null); // super.h.invoke 指的就是 DynastyDynamicProxy.invoke
        } catch (RuntimeException | Error var2) {
            throw var2;
        } catch (Throwable var3) {
            throw new UndeclaredThrowableException(var3);
        }
    }
```

### $Proxy0#lastEmperor

```java
    public final String lastEmperor() throws  {
        try {
            return (String)super.h.invoke(this, m3, (Object[])null); // super.h.invoke 指的就是 DynastyDynamicProxy.invoke
        } catch (RuntimeException | Error var2) {
            throw var2;
        } catch (Throwable var3) {
            throw new UndeclaredThrowableException(var3);
        }
    }
```

> 由上可知，`$Proxy0` 实现的方法的统一调用入口(`uniform interface`) 就是 DynastyDynamicProxy#invoke，也就是 jdk 动态代理必须实现的 InvocationHandler#invoke 方法。

# `InvocationHandler` 接口实现类 `DynastyDynamicProxy` 分析

```java

public class DynastyDynamicProxy implements InvocationHandler {

    // 被代理的对象
    private Object delegate;
    public DynastyDynamicProxy(Object originalDynamic){
        this.delegate = originalDynamic;
    }
    /**
    @param proxy 当前动态代理类 $Proxy0 的实例
    @param method 当前动态代理类调用的方法
    @param args 当前动态代理类调用的方法的参数
    **/
    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        System.out.println("公元 618 年唐朝建立!");
        String res = (String) method.invoke(delegate,args);
        System.out.println(method.getName() + " of Tang Dynasty was " + res + "!");
        System.out.println("公元 907 年唐朝灭亡!\n");
        return res;
    }
}
```

![dynamic-proxy-method-call](https://raw.githubusercontent.com/stardustman/pictures/main/img/dynamic-proxy-stack-frame-3.jpg)

# 被代理的类 `TangDynasty` 分析

![](https://raw.githubusercontent.com/stardustman/pictures/main/img/dynamic-proxy-stack-frame-4.jpg)

```java
public class TangDynasty implements Dynasty{

    @Override
    public String founder() {
        return "李渊";
    }

    @Override
    public String reviver() {
        return "李隆基";
    }

    @Override
    public String lastEmperor() {
        return "李柷";
    }
}
```

> - 由`调用栈`可知，最终还是要调用`被代理的类`的方法。
> - 但是可以在执行真正的代理类方法之前，在 `InvocationHandler#invoke` 中添加实际需要的逻辑。这就是中间层，通过中间层进行逻辑的增强。
> - 这样看起来就好像被代理对象的逻辑进行了增强。

# References

1. [dynamic-proxy-class-method](https://github.com/JetBrains/jdk8u_jdk/blob/master/src/share/classes/sun/misc/ProxyGenerator.java#L441-L443)
2. [jdk-com-sun-proxy](https://www.baeldung.com/jdk-com-sun-proxy)
3. [ProxyGenerator#sun.misc.ProxyGenerator.saveGeneratedFiles](https://github.com/JetBrains/jdk8u_jdk/blob/master/src/share/classes/sun/misc/ProxyGenerator.java#L313-L317)
4. [jdk8-proxy](https://docs.oracle.com/javase/8/docs/technotes/guides/reflection/proxy.html)
5. [分析所用代码](https://github.com/stardustman/java-inspect)
6. [$Proxy0](https://github.com/JetBrains/jdk8u_jdk/blob/master/src/share/classes/java/lang/reflect/Proxy.java#L560-L564)
7. [jdk-porxy-and-cglib](https://www.springcloud.io/post/2023-01/jdk-porxy-and-cglib/#gsc.tab=0)

