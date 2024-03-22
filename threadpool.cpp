#include "threadpool.h"
#include <functional>
#include <thread>
#include <iostream>

const int TASK_MAX_THRESHHOLD = 1024;

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


	//�̵߳�ͨ��   �ȴ���������п���

	//����п��࣬������������������

	//��Ϊ�·�������������п϶������ˣ���notEmpty_�Ͻ���֪ͨ


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
	std::cout << "begin threadFunc tid:" << std::this_thread::get_id() << std::endl;
	std::cout << "end threadFunc tid:" << std::this_thread::get_id()<<std::endl;
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
