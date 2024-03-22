// ThreadPool.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "threadpool.h"
#include <chrono>
#include <thread>
/*
有些场景，是希望能够获取线程执行任务得返回值得
举例：
1 + 。。。 + 30000的和
thread1  1 + ... + 10000
thread2  10001 + ... + 20000
.....

main thread：给每一个线程分配计算的区间，并等待他们算完返回结果，合并最终的结果即可
*/

class MyTask : public Task {
public:
	MyTask(int begin, int end)
		: begin_(begin)
		, end_(end)
	{}
	// 问题一：怎么设计run函数的返回值，可以表示任意的类型
	// Java Python   Object 是所有其它类类型的基类
	// C++17 Any类型
	Any run() { //线程代码
		std::cout << "tid:" << std::this_thread::get_id()<<"begin!"<<std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		int sum = 0;
		for (int i = begin_; i <= end_;i++ ) {
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
	ThreadPool pool;
	pool.start(4);


	// 问题：ThreadPool对象析构以后，怎么样把线程池相关的线程资源全部回收？
	
	// 如何设计这里的Result机制呢
	Result res = pool.submitTask(std::make_shared<MyTask>());
	int sum = res.get().cast<int>();


	pool.submitTask(std::make_shared<MyTask>());
	pool.submitTask(std::make_shared<MyTask>());
	pool.submitTask(std::make_shared<MyTask>());
	pool.submitTask(std::make_shared<MyTask>());
	pool.submitTask(std::make_shared<MyTask>());
	pool.submitTask(std::make_shared<MyTask>());
	pool.submitTask(std::make_shared<MyTask>());
	pool.submitTask(std::make_shared<MyTask>());
	pool.submitTask(std::make_shared<MyTask>());


	getchar();
	//
	//std::this_thread::sleep_for(std::chrono::seconds(5));

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
