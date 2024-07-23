
# unix domain socket 是啥?

> A Unix domain socket (UDS) or IPC socket (inter-process communication) is a data communications endpoint for exchanging data between processes executing on the same host operating system. 
> 同一台机器两个不同的进程之间交换数据，优化过的 socket。

# 问题背景

> java 写的程序需要和 c 写的程序交换数据，且两个程序运行在同一台机器上。

# gradle 配置

```groovy
// https://mvnrepository.com/artifact/com.kohlschutter.junixsocket/junixsocket-native-common
implementation group: 'com.kohlschutter.junixsocket', name: 'junixsocket-native-common', version: '2.9.1'

// https://mvnrepository.com/artifact/com.kohlschutter.junixsocket/junixsocket-common
implementation group: 'com.kohlschutter.junixsocket', name: 'junixsocket-common', version: '2.9.1'
```

# 程序示例

```java
import org.newsclub.net.unix.AFUNIXServerSocket;
import org.newsclub.net.unix.AFUNIXSocket;
import org.newsclub.net.unix.AFUNIXSocketAddress;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;


/**
 * unix domain socket 进程通信
 */
@SuppressWarnings({"CatchAndPrintStackTrace" /* errorprone */, "PMD.CognitiveComplexity"})
public final class MsgServer implements Runnable {
    private static final String UDS = System.getProperty("user.home") + "ipc.sock";
    private static final Logger logger = LoggerFactory.getLogger(MsgServer.class);

    private static void startServer() throws IOException {
        Path path = Paths.get(UDS);
        logger.info("UDO  path: {}", UDS);
        if (Files.exists(path)) {
            Files.delete(path);
        }

        File socketFile = new File(UDS);
        try (AFUNIXServerSocket server = AFUNIXServerSocket.newInstance()) {
            server.setReuseAddress(false);
            server.bind(AFUNIXSocketAddress.of(socketFile));
            logger.info("MsgServer 监听 socket: {}", server);

            while (!Thread.interrupted() && !server.isClosed()) {
                logger.info("Waiting for connection...");
                try {
                    AFUNIXSocket sock = server.accept();
                    Thread thread = new Thread(new ClientHandler(sock), "msg-receiver");
                    thread.start();
                } catch (IOException e) {
                    logger.error("server.accept() 出错", e);
                }
            }
        } finally {
            logger.info("MsgServer terminated");
        }
    }

    @Override
    public void run() {
        try {
            startServer();
        } catch (IOException e) {
            logger.error("启动 MsgServer 失败", e);
        }
    }

    /**
     * 处理连接,以接收 utf-8 字符串为例
     */
    static class ClientHandler implements Runnable {
        AFUNIXSocket sock;

        public ClientHandler(AFUNIXSocket socket) {
            this.sock = socket;
        }

        @Override
        public void run() {
            try (InputStream is = sock.getInputStream(); OutputStream os = sock.getOutputStream(); PrintWriter writer = new PrintWriter(os, true); BufferedReader reader = new BufferedReader(new InputStreamReader(is, StandardCharsets.UTF_8))) {
                String line;
                while ((line = reader.readLine()) != null && !line.equalsIgnoreCase("bye")) {
                    logger.info("Received: " + line);
                    handleMsg(line);
                    writer.println("Received successfully");
                    writer.flush();
                }
            } catch (IOException e) {
                logger.error("客户端关闭了连接", e);
            } finally {
                try {
                    sock.close();
                } catch (IOException e) {
                    logger.error("关闭连接出错", e);
                }
            }

        }

        private void handleMsg(String msg) {
            // process receive string msg
        }

    }

}

```












# References

1. [junixsocket](https://github.com/kohlschutter/junixsocket)
2. [Unix_domain_socket](https://en.wikipedia.org/wiki/Unix_domain_socket)

