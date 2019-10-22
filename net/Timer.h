#pragma once
#include"TimeStamp.h"
#include<functional>
#include<queue>
#include<memory>
class Timer
{

public:
	typedef std::function<void()> DeleteCallback;
	Timer(TimeStamp timeout,DeleteCallback cb);
	Timer(TimeStamp timeout, int second, DeleteCallback cb);
	~Timer();
	void SetDelete() { _deleted = true; }
	bool isValid();
	void update(TimeStamp timeout);//������ʱû��
	bool isDeleted() { return _deleted; }//������ʱû��
	bool isRepeated() { return _repeat; }//���ڶ�ʱ��ʹ��
	int getRepeatSecond() { return _repeatSecond; }//���ڶ�ʱ��ʹ��
	DeleteCallback getDeleteCallback() { return _deleteCallback; }//���ڶ�ʱ��ʹ��
	TimeStamp getExpiredTime() { return _expiredTime; }
	TimeStamp getNextExpiredTime() { return TimeStamp::addTime(TimeStamp::Now(), _repeatSecond); }//���ڶ�ʱʹ��
private:
	bool _deleted;
	bool _repeat;
	TimeStamp _expiredTime;
	DeleteCallback _deleteCallback;//����������ִ�лص���ȫ��
	int _repeatSecond;
};

struct TimerCmp
{
	bool operator()(std::shared_ptr<Timer>& a, std::shared_ptr<Timer>& b) const
	{
		return a->getExpiredTime() > b->getExpiredTime();
	}
};

class TimerQueue
{
public:
	TimerQueue() {};
	~TimerQueue() {};
	typedef std::shared_ptr<Timer> TimerPtr;
	void addTimer(TimeStamp timeout,Timer::DeleteCallback cb);
	void addTimer(TimeStamp timeout, int second, Timer::DeleteCallback cb);
	void handleExpriedEvent();
	TimeStamp GetLatestTimestamp() { return _timerQueue.top()->getExpiredTime(); }
	bool isEmpty() { return _timerQueue.empty(); }
private:
	std::priority_queue < TimerPtr, std::vector<TimerPtr>, TimerCmp> _timerQueue;
};
//Next Version:ȱ��ɾ����ʱ������
//��ʱ�����Ȳ�����ֻ�ܵ����뼶
//����ʹ��timerfd�ع�����������ʱ����˵�ɡ�����