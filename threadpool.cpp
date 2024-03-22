#include "threadpool.h"
#include <functional>
#include <thread>
#include <iostream>

const int TASK_MAX_THRESHHOLD = 4;

//�̳߳ع���
ThreadPool::ThreadPool()
	:initThreadSize_(4)
	, taskSize_(0)
	, taskQueMaxThreshHold_(TASK_MAX_THRESHHOLD)
	, poolMode_(PoolMode::MODE_FIXED)
{
}

//�̳߳�����
ThreadPool::~ThreadPool() {
}

// �����̳߳صĹ���ģʽ
void ThreadPool::setMode(PoolMode mode) {
	poolMode_ = mode;
}

//���ó�ʼ���߳�����
void ThreadPool::setInitThreadSize(int size) {
}

//����task�������������ֵ
void ThreadPool::setTaskQueMaxThreshHold(int threshhold) {
	taskQueMaxThreshHold_ = threshhold;
}

//���̳߳��ύ����     �û����øýӿڣ��������������������
void ThreadPool::submitTask(std::shared_ptr<Task> sp) {
	//��ȡ��
	std::unique_lock<std::mutex> lock(taskQueMtx_);

	//�̵߳�ͨ��   �ȴ���������п���  wait    wait_for������һ��ʱ��     wait_until ���˸��ȴ�ʱ��
	//�û��ύ�����������������1s,�����ж��ύ����ʧ�ܣ�����
	if (!notFull_.wait_for(lock, std::chrono::seconds(1),
		[&]()->bool {return taskQue_.size() < taskQueMaxThreshHold_;	})) {
		//��ʾnotFull_ �ȴ�1s,������Ȼû������
		std::cerr << "task queue is full, submit task fail." << std::endl;
		return;
	}
	
	//����п��࣬������������������
	taskQue_.emplace(sp);
	taskSize_++;

	//��Ϊ�·�������������п϶������ˣ���notEmpty_�Ͻ���֪ͨ���Ͽ�����߳�ִ������
	notEmpty_.notify_all();

}

// �����̳߳�
void ThreadPool::start(int initThreadSize) {
	//��¼��ʼ�̸߳���
	initThreadSize_ = initThreadSize;

	//�����̶߳���
	for (int i = 0; i < initThreadSize_; i++) {
		// ����thread�̶߳����ʱ�򣬰��̺߳�������thread�̶߳���
		auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this)); // make_unique���ٿռ�
		threads_.emplace_back(std::move(ptr)); //unique_ptr��ֹ������ֵ
	}

	//���������߳�
	for (int i = 0; i < initThreadSize_; i++) {
		threads_[i]->start(); //��Ҫȥִ��һ���̺߳���
	}

}

// �����̺߳���   �̳߳ص������̴߳��������������������
void ThreadPool::threadFunc() {
	/*std::cout << "begin threadFunc tid:" << std::this_thread::get_id() << std::endl;
	std::cout << "end threadFunc tid:" << std::this_thread::get_id()<<std::endl;*/

	for (;;) {
		std::shared_ptr<Task> task; {
			//��ȡ��
			std::unique_lock<std::mutex> lock(taskQueMtx_);

			std::cout << "tid:" << std::this_thread::get_id() << "���Ի�ȡ����..." << std::endl;


			//�ȴ�notEmpty_����
			notEmpty_.wait(lock, [&]()->bool {return taskQue_.size() > 0; });

			std::cout << "tid:" << std::this_thread::get_id() << "��ȡ����ɹ�..." << std::endl;

			//�������������ȡ��һ������
			task = taskQue_.front();
			taskQue_.pop();
			taskSize_--;

			//�����Ȼ��ʣ�����񣬼���֪ͨ�������߳�ִ������
			if (taskQue_.size() > 0) {
				notEmpty_.notify_all();
			}

			//ȡ��һ�����񣬽���֪ͨ��֪ͨ���Լ����ύ��������
			notFull_.notify_all();
		}//�ͷ���
		//��ǰ�̸߳���ִ���������
		if (task!=nullptr) {
			task->run();
		}

	}

}


//////////////////////////////////////////////�̷߳���ʵ��

//�̹߳���
Thread::Thread(ThreadFunc func) : func_(func) {
	
}

//�߳�����
Thread::~Thread() {

}

//�����߳�
void Thread::start(){
	//����һ���߳���ִ��һ���̺߳���
	std::thread t(func_); // C++11 ��˵ �̶߳���t ���̺߳���func_
	t.detach();  //���÷����߳�  ���̶߳������������������ �̺߳�����Ҫ����ʹ��  pthread_detach   pthread_t���÷����߳�
}
