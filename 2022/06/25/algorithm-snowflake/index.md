
# 分布式 ID

snowflake 算法使用一个 64bits 的正整数作为 ID。64bits 正整数 layout 如下：

![snowflake-i64-layout](https://raw.githubusercontent.com/stardustman/pictures/main/img/snowflake.jpg)

```java
package stardustman.github.io;

public class SnowFlake {
    // 起始的时间戳
    private final static long START_STAMP = 1480166465631L;

    // sequence number
    private final static long SEQUENCE_BIT = 12;

    // machine id bits
    private final static long MACHINE_BIT = 5;

    // data-center id bits
    private final static long DATA_CENTER_BIT = 5;

    // 每一部分最大值
    private final static long MAX_SEQUENCE_NUM = ~(-1L << SEQUENCE_BIT);
    private final static long MAX_MACHINE_NUM = ~(-1L << MACHINE_BIT);
    private final static long MAX_DATA_CENTER_NUM = ~(-1L << DATA_CENTER_BIT);

    // 每一部分左移位数
    private final static long MACHINE_LEFT = SEQUENCE_BIT;
    private final static long DATA_CENTER_LEFT = SEQUENCE_BIT + MACHINE_BIT;
    private final static long TIMESTAMP_LEFT = DATA_CENTER_LEFT + DATA_CENTER_BIT;

    private final long dataCenterId;

    private final long machineId;

    private long sequence = 0L;

    private long lastStamp = -1L;

    public SnowFlake(long dataCenterId, long machineId){
        if (dataCenterId > MAX_DATA_CENTER_NUM || dataCenterId < 0){
            throw new IllegalArgumentException("dataCenterId can't be greater than MAX_DATA_CENTER_ID or less than 0" );
        }

        if (machineId > MAX_MACHINE_NUM || machineId < 0){
            throw new IllegalArgumentException("dataCenterId can't be greater than MAX_DATA_CENTER_ID or less than 0" );
        }

        this.dataCenterId = dataCenterId;
        this.machineId = machineId;
    }

    // generate next ID
    public synchronized long nextId(){
        long currStamp = getNewStamp();
        if (currStamp < lastStamp){
            throw new RuntimeException("clock moved backwards. Refusing to generate id");
        }

        if (currStamp == lastStamp) {
            // 同一个毫秒内,序号递增，sequence 最大是 4095
            sequence = (sequence + 1) & MAX_SEQUENCE_NUM;

            // 同一毫秒内，序列书已经到最大
            if (sequence == 0L){
                // 阻塞到下一个毫秒，获取新的时间戳
                currStamp = getNextMill();
            }
        } else {
            // 不同毫秒，序列号置为 0
            sequence = 0L;
        }
        lastStamp = currStamp;

        // assemble 64bits id
        return (currStamp - START_STAMP) << TIMESTAMP_LEFT
                | dataCenterId << DATA_CENTER_LEFT
                | machineId << MACHINE_LEFT
                | sequence;

    }

    private long getNextMill(){
        long mill = getNewStamp();
        while (mill <= lastStamp){
            mill = getNewStamp();
        }
        return mill;
    }

    private long getNewStamp(){
        return System.currentTimeMillis();
    }

    public static void main(String[] args) {
        SnowFlake snowFlake = new SnowFlake(11,11);

        long start = System.currentTimeMillis();

        for (int i = 0; i < 10 ; i++) {
            System.out.println(snowFlake.nextId());
        }
        System.out.println(System.currentTimeMillis() - start);
        System.out.println(MAX_SEQUENCE_NUM);
        System.out.println(MAX_MACHINE_NUM);
        System.out.println(MAX_DATA_CENTER_NUM);
    }

}

```

