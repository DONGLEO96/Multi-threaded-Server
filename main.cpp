#include"httpServer.h"
int main(int argc, char* argv[])
{
	httpServer test(3);
	test.setSourceFile("/home/ubuntu/myBlog/public");
	test.start();
}
//void echoFunc(TCPserver::ConnectionPtr connPtr, Buffer& buffer)//onMessage�ص�����
//{
//	connPtr->send(buffer.beginRead(), buffer.readableBytes());
//	buffer.retriveAll();
//}
//void echo10000Func(TCPserver::ConnectionPtr connPtr, Buffer& buffer)
//{
//	std::string a(buffer.beginRead(), buffer.readableBytes());
//	buffer.retriveAll();
//	std::string echoString;
//	echoString.append(60000	, '0');
//	echoString.append(a);
//	std::cout << echoString.size() << std::endl;
//	connPtr->send(const_cast<char*>(echoString.data()), echoString.size());
//}
//void TimerTest()//��ʱ�����Ժ���
//{
//	std::cout << "timer activity:" << TimeStamp::Now().toFormatteedString(true) << std::endl;
//}
//void ThreadTest()//�߳�����Ժ���
//{
//	std::cout << "Thread is Runing" << std::endl;
//	sleep(1);
//}
//
//static int a = 0;
//Mutex mutex;
//Condition cond(mutex);
//void ConditionTest1(int printNum)
//{
//	for (int i = 0; i < 5; i++)
//	{
//		MutexGuard mutexGuard(mutex);
//		while (a % 3 != printNum)
//			cond.wait();
//		std::cout << printNum << std::endl;
//		a += 1;
//		cond.notifyAll();
//	}
//}
//typedef std::function<void()> task;
//TaskQueue<task> taskQueue;
//void TaskQueueTest(task task)
//{
//	sleep(5);
//	taskQueue.push(task);
//	sleep(2);
//	taskQueue.push(task);
//	sleep(10);
//	taskQueue.push(task);
//}
//void ThreadpoolTest()
//{
//	std::cout << "Name:" << CurrentThread::name() << "  "
//		<< "tid:" << CurrentThread::tid() <<std::endl;
//}
//
//
////-----------------------------------�̳߳ز��Դ���---------------------------------------------
//int main()
//{
//	Threadpool threadpool;
//	threadpool.start(5);
//	for (int i = 0; i < 100; i++)
//	{
//		threadpool.addTask(ThreadpoolTest);
//	}
//	while (threadpool.getTaskNum() != 0)
//	{
//		std::cout << "Tsak have not finished" << std::endl;
//	}
//	threadpool.stop();
//	return 0;
//}
////-----------------------------------�̳߳ز��Դ���---------------------------------------------
////-----------------------------------������в��Դ���---------------------------------------------
////int main()
////{
////	
////	Thread taskThread(std::bind(TaskQueueTest, ThreadTest));
////	std::cout << TimeStamp::Now().toFormatteedString(false) << std::endl;
////	taskThread.start();
////	Thread thread0(taskQueue.take());
////	std::cout << TimeStamp::Now().toFormatteedString(false) << std::endl;
////	Thread thread1(taskQueue.take());
////	std::cout << TimeStamp::Now().toFormatteedString(false) << std::endl;
////	Thread thread2(taskQueue.take());
////	std::cout << TimeStamp::Now().toFormatteedString(false) << std::endl;
////	return 0;
////}
////-----------------------------------������в��Դ���---------------------------------------------
//
//
//
////-----------------------------------�߳�����Դ���---------------------------------------------
////int main()
////{
////	Thread thread(ThreadTest);
////	thread.start();
////	std::cout << thread.Getname() << ":" << thread.tid() << std::endl;
////	sleep(10);
////}
////-----------------------------------�߳�����Դ���---------------------------------------------
////-----------------------------------�������������������Դ���---------------------------------------------
////int main()
////{
////	Thread thread0(std::bind(ConditionTest1, 0));
////	Thread thread1(std::bind(ConditionTest1, 1)); 
////	Thread thread2(std::bind(ConditionTest1, 2)); 
////	thread1.start();
////	thread2.start();
////	sleep(1);
////	thread0.start();
////	thread0.join();
////	thread1.join();
////	thread2.join();
////	sleep(30);
////	return 0;
////}
////-----------------------------------�������������������Դ���---------------------------------------------
////-----------------------------------�������Դ���-------------------------------------------------------
//int main()
//{
//	Eventloop loop;
////	loop.runEvery(2, TimerTest);//timer���Դ���
//	TCPserver tcpserver(&loop);
//	tcpserver.setMessageCallback(echoFunc);
//	tcpserver.start();
//	loop.loop();
//	return 0;
//}
////-----------------------------------�������Դ���-------------------------------------------------------