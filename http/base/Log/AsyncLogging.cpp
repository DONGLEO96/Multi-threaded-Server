#include "AsyncLogging.h"
#include"TimeStamp.h"


AsyncLogging::AsyncLogging(string filename, int flushInteval)
	:_flushInterval(flushInteval),
	_running(false),
	_fileName(filename),
	_thread(std::bind(&AsyncLogging::threadFunc, this)),
	_mutex(),
	_cond(_mutex),
	_currBuffer(new Buffer),
	_nextBuffer(new Buffer),
	_buffers(),
	_latch(1)
{
	_currBuffer->bzero();
	_nextBuffer->bzero();
	_buffers.reserve(16);
}


AsyncLogging::~AsyncLogging()
{
	if (_running)
		stop();
}
void AsyncLogging::append(const char* logline, int len)//д����־����logging����ã���ǰ���߳�ʹ��
{
	MutexGuard lock(_mutex);
	if (_currBuffer->avail() > len)//���湻дֱ��д
		_currBuffer->append(logline, len);
	else
	{
		_buffers.push_back(_currBuffer);//��ǰ������д���˷����˵Ļ������б���
		_currBuffer.reset();
		if (_nextBuffer)//�������nextBuffer��������nextbuffer����д
			_currBuffer = std::move(_nextBuffer);
		else
			_currBuffer.reset(new Buffer);//û��nextbuffer�ˣ���һ���µ�buffer
		_currBuffer->append(logline, len);
		_cond.notify();//д����һ����������Ҫ���Ѻ���߳������ļ�д
	}
}

void AsyncLogging::start()
{
	_running = true;
	_thread.start();
	_latch.wait();//�ȵ���־�߳������������ٷ���
	//Ԥ��ǰ���߳� start(��־�߳�����,���ǻ�û�����е�func)---д��--->stop(��־�̸߳պ����е�func�����_running,�˳�����־��ʧ��)
}

void AsyncLogging::stop()
{
	_running = false;
	_cond.notify();
	_thread.join();
}
void AsyncLogging::threadFunc()//�����־�̣߳���ǰ��д�뻺��������־д�뵽�ļ���
{
	LogFile output(_fileName);
	BufferPtr newBuffer1(new Buffer);
	BufferPtr newBuffer2(new Buffer);
	newBuffer1->bzero();
	newBuffer2->bzero();
	BufferVector buffersTowrite;
	buffersTowrite.reserve(16);
	_latch.CountDown();
	while (_running)
	{
		MutexGuard lock(_mutex);
		if (_buffers.empty())
		{
			_cond.waitForSeconds(_flushInterval);
		}
		_buffers.push_back(_currBuffer);
		_currBuffer.reset();
		_currBuffer = std::move(newBuffer1);
		buffersTowrite.swap(_buffers);
		if (!_nextBuffer)//���nextBuffe�Ѿ���ʹ�ù���
		{
			_nextBuffer = std::move(newBuffer2);
		}
		// �����Ҫд���ļ���buffer�б���buffer�ĸ�������25����ô����������ɾ��  
		// ��Ϣ�ѻ�
		//ǰ��������ѭ����ƴ��������־��Ϣ��������˵Ĵ�������
		//���ǵ��͵������ٶȳ��������ٶȣ�������������ڴ��еĶѻ�
		//����ʱ������������(�����ڴ治��),
		//��������(�����ڴ�ʧ��)
		if (buffersTowrite.size() > 25)
		{
			char buf[256];
			snprintf(buf, sizeof(buf), "Dropped log messages at %s, %zd larger buffers\n"
				, TimeStamp::Now().toFormatteedString(0).data(), buffersTowrite.size() - 2);
			buffersTowrite.erase(buffersTowrite.begin() + 2, buffersTowrite.end());
		}
		for (int i = 0; i < buffersTowrite.size(); ++i)
		{
			output.append(buffersTowrite[i]->data(), buffersTowrite[i]->length());
		}
		if (buffersTowrite.size() > 2)
		{
			buffersTowrite.resize(2);
		}
		if (!newBuffer1)
		{
			newBuffer1 = buffersTowrite.back();
			buffersTowrite.pop_back();
			newBuffer1->reset();
		}
		if (!newBuffer2)
		{
			newBuffer2 = buffersTowrite.back();
			buffersTowrite.pop_back();
			newBuffer2->reset();
		}
		buffersTowrite.clear();
		output.flush();
	}
	output.flush();//ֹͣ��Ҳ��Ҫ�ѻ�������ϴһ�Σ�
}