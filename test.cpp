// ThreadPool.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "threadpool.h"
#include <chrono>
#include <thread>
using namespace std;
/*
有些场景，是希望能够获取线程执行任务得返回值得
举例：
1 + 。。。 + 30000的和
thread1  1 + ... + 10000
thread2  10001 + ... + 20000
.....

main thread：给每一个线程分配计算的区间，并等待他们算完返回结果，合并最终的结果即可
*/
using uLong = unsigned long long;

class MyTask : public Task {
public:
	MyTask(int begin, int end)
		: begin_(begin)
		, end_(end)
	{}
	// 问题一：怎么设计run函数的返回值，可以表示任意的类型
	// Java Python   Object 是所有其它类类型的基类
	// C++17 Any类型
	Any run() { //线程代码    // run方法最终就在线程池分配的线程中去做执行了!
		std::cout << "tid:" << std::this_thread::get_id()<<"begin!"<<std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));
		uLong sum = 0;
		for (uLong i = begin_; i <= end_;i++ ) {
			sum += i;
		}
		std::cout << "tid:" << std::this_thread::get_id() << "end!" << std::endl;
		return sum;
	}
private:
	int begin_;
	int end_;
};

int main(){
	{
		ThreadPool pool;
		pool.setMode(PoolMode::MODE_CACHED);

		//开始启动线程池
		pool.start(2);

		Result res1 = pool.submitTask(std::make_shared<MyTask>(1, 100000000));
		Result res2 = pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
		pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
		pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
		pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
		uLong sum1 = res1.get().cast_<uLong>(); //get返回了一个Any类型，怎么转成具体的类型呢？
		cout << sum1 << endl;
	}// 这里Result对象也要析构!!! 在vs下，条件变量析构会释放相应资源的
	
	cout << "main over !" << endl;
	getchar();

#if 0
	{
		ThreadPool pool;
		//用户自己设置线程池的工作模式
		pool.setMode(PoolMode::MODE_CACHED);
		//开始启动线程池
		pool.start(4);


		// 问题：ThreadPool对象析构以后，怎么样把线程池相关的线程资源全部回收？

		// 如何设计这里的Result机制呢
		Result res1 = pool.submitTask(std::make_shared<MyTask>(1, 100000000));
		Result res2 = pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
		Result res3 = pool.submitTask(std::make_shared<MyTask>(200000001, 300000000));
		pool.submitTask(std::make_shared<MyTask>(200000001, 300000000));
		pool.submitTask(std::make_shared<MyTask>(200000001, 300000000));
		pool.submitTask(std::make_shared<MyTask>(200000001, 300000000));
		//随着task被执行完，task对象没了，依赖于task对象的Result对象也没了
		uLong sum1 = res1.get().cast_<uLong>(); //get返回了一个Any类型，怎么转成具体的类型呢？
		uLong sum2 = res2.get().cast_<uLong>();
		uLong sum3 = res3.get().cast_<uLong>();

		// Master - Slave线程模型
		// Master线程用来分解任务，然后给各个Slave线程分配任务
		// 等待各个Slave线程执行完任务，返回结果
		// Master线程合并各个任务结果，输出
		std::cout << (sum1 + sum2 + sum3) << std::endl;

		uLong sum = 0;
		for (uLong i = 1; i <= 300000000; i++) {
			sum += i;
		}
		cout << sum << endl;
	}
	
	//pool.submitTask(std::make_shared<MyTask>());
	//pool.submitTask(std::make_shared<MyTask>());
	//pool.submitTask(std::make_shared<MyTask>());
	//pool.submitTask(std::make_shared<MyTask>());
	//pool.submitTask(std::make_shared<MyTask>());
	//pool.submitTask(std::make_shared<MyTask>());
	//pool.submitTask(std::make_shared<MyTask>());
	//pool.submitTask(std::make_shared<MyTask>());
	//pool.submitTask(std::make_shared<MyTask>());


	getchar();
	//
	//std::this_thread::sleep_for(std::chrono::seconds(5));
#endif
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
