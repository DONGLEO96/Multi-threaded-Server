#include "EventLoopThread.h"
#include<assert.h>

EventLoopThread::EventLoopThread(const ThreadInitCallBack& cb, const std::string& name):_loop(NULL),_exiting(false),
							_thread(std::bind(&EventLoopThread::threadFunc, this), name),_mutex(),_cond(_mutex),_callback(cb)
{

}


EventLoopThread::~EventLoopThread()
{
	_exiting = true;
	if (_loop != NULL)
	{
		_loop->quit();
		_thread.join();//���ս�ʬ����
	}
}
Eventloop* EventLoopThread::startLoop()
{
	assert(!_thread.started());
	_thread.start();
	Eventloop* loop = NULL;//�����������threadfunc�����ڽ���ѭ����_loop����
	{
		MutexGuard lock(_mutex);
		while (_loop == NULL)
		{
			_cond.wait();
		}
		loop = _loop;
	}
	return loop;
}
void EventLoopThread::threadFunc()
{
	Eventloop loop;
	if (_callback)
	{
		_callback(&loop);
	}
	{
		MutexGuard lock(_mutex);
		_loop = &loop;
		_cond.notify();
	}
	loop.loop();
	MutexGuard lock(_mutex);
	_loop = NULL;//�����߳�loop�ᱻ���٣������ٳ������ָ��
}
