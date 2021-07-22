# thread_pool
The dynamic thread pool, can adjust thread number according the tasks.
这是一个基于std c++17版本写的一个线程池，可以动态的根据任务的数量增加和减少线程的数量。当任务较少时，空闲线程会在等待一段时间后，如果仍然没有任务就会退出，直至当前可用线程的数量不大于最小数目，当任务较多时，会增加线程的数目，直至达到线程数量的最大值。


ThreadPool.h和Thread.cpp为线程池的类，pool_test.cpp为测试和示例文件，通过下面的命令可以编辑。

clang++ -o thread_pool ThreadPool.cpp main_test.cpp -std=c++17 -lpthread -O3
或
g++ -o thread_pool ThreadPool.cpp main_test.cpp -std=c++17 -lpthread -O3
