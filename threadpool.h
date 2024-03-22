#ifndef THREADPOOL_H 
#define THREADPOOL_H// THREADPOOL_H 
#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <unordered_map>
#include <thread>
#include <future>

//����������
class Task {
public:
	//�û������Զ��������������ͣ���Task�̳У���дrun������ʵ���Զ���������
	virtual void run() = 0;
};

//�̳߳�֧�ֵ�ģʽ
enum class PoolMode {
	MODE_FIXED,//�̶��������߳�
	MODE_CACHED,//�߳������ɶ�̬����
};

//�߳�����
class Thread {
public:
	//�̺߳�����������
	using ThreadFunc = std::function<void()>;

	//�̹߳���
	Thread(ThreadFunc func);
	//�߳�����
	~Thread();

	//�����߳�
	void start();
private:
	ThreadFunc func_;
};

//�̳߳�����
class ThreadPool {
public:
	//�̳߳ع���
	ThreadPool();

	//�̳߳�����
	~ThreadPool();

	// �����̳߳صĹ���ģʽ
	void setMode(PoolMode mode);

	//���ó�ʼ���߳�����
	void setInitThreadSize(int size);

	//����task�������������ֵ
	void setTaskQueMaxThreshHold(int threshhold);

	//���̳߳��ύ����
	void submitTask(std::shared_ptr<Task> sp);

	// �����̳߳�
	void start(int initThreadSize = 4);

	ThreadPool(const ThreadPool&) = delete; //��ֹ�û����̳߳ؽ��п��� �� ��ֵ ����
	ThreadPool& operator=(const ThreadPool&) = delete;
private:
	//�����̺߳���
	void threadFunc();
private:
	std::vector<std::unique_ptr<Thread>>threads_;//�߳��б�    unique_ptr �Զ���������
	int initThreadSize_;// ��ʼ�߳�����

	std::queue<std::shared_ptr<Task>> taskQue_; //�������
	std::atomic_int taskSize_;//���������
	int taskQueMaxThreshHold_;  // �����������������ֵ

	std::mutex taskQueMtx_; // ��֤������е��̰߳�ȫ
	std::condition_variable notFull_; // ��ʾ������в���
	std::condition_variable notEmpty_; // ��ʾ������в���

	PoolMode poolMode_; // ��ǰ�̳߳صĹ���ģʽ

};


#endif