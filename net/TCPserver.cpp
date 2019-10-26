#include "TCPserver.h"
#include<iostream>
#include<functional>
#include"Connection.h"
#include"Logging.h"
#include"util.h"
#include<signal.h>

void handleSIGPIPE()
{
	struct sigaction sa;
	memset(&sa, '\0', sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	if (sigaction(SIGPIPE, &sa, NULL))
		return;
}

TCPserver::TCPserver(Eventloop* loop,int threadNum) :_loop(loop), _looppool(loop)
{
	if (threadNum > 0)
	{
		_looppool.setThreadNum(threadNum);
	}
}


TCPserver::~TCPserver()
{
}
//void TCPserver::removeConnection(int fd)
//{
//	Eventloop* currloop = (loopMap.find(fd))->second;
//	currloop->assertInLoopThread();
//	currloop->removeChannel(fd);
//	//_loop->assertInLoopThread();
//	//_loop->removeChannel(fd);//��ô�ö�Ӧ��loopȥɾ��channel
//	connectionMap.erase(fd);//���½����ӽ�����IO�̣߳����map�ͱ�����ٽ�ֵ��������ôд
//	//std::cout << "connection nums:" << connectionMap.size() << std::endl;
//	//�������reactor�̳߳أ��Ƴ���Ҫ�޸ģ�connection���Լ����̵߳���removeconnection��
//	//Ӧ�ý�removechannel����connection�Լ����߳��д���TCPserver�߳��н���ɾ��map�е�ָ��
//	//���ٳ������connection��ͨ����shared_from_this��һ��ɾ���ص�����������connection�����߳���ִ�С�
//	//���ڽ�������
//}
void TCPserver::removeConnection(int fd,Eventloop* currloop)
{
	currloop->assertInLoopThread();
	currloop->removeChannel(fd);
	LOG << "Disconncet with addr:" << inet_ntoa(connectionMap[currloop].find(fd)->second->getAddr().sin_addr) << "\n";
	connectionMap[currloop].erase(fd);
	//_loop->assertInLoopThread();
	//_loop->removeChannel(fd);//��ô�ö�Ӧ��loopȥɾ��channel
	//connectionMap.erase(fd);//���½����ӽ�����IO�̣߳����map�ͱ�����ٽ�ֵ��������ôд
	//std::cout << "connection nums:" << connectionMap.size() << std::endl;
	//�������reactor�̳߳أ��Ƴ���Ҫ�޸ģ�connection���Լ����̵߳���removeconnection��
	//Ӧ�ý�removechannel����connection�Լ����߳��д���TCPserver�߳��н���ɾ��map�е�ָ��
	//���ٳ������connection��ͨ����shared_from_this��һ��ɾ���ص�����������connection�����߳���ִ�С�
	//���ڽ�������
}

void TCPserver::newConnnection(int fd, sockaddr_in cliaddr, Eventloop* currLoop)//���½�connection�����񶪸�IO�߳��Լ�ȥִ��
{
	ConnectionPtr _conn(new Connection(fd, currLoop, cliaddr));
	_conn->SetHandleClose(std::bind(&TCPserver::removeConnection, this, std::placeholders::_1, std::placeholders::_2));
	_conn->SetMessageCallback(_messageCallback);
	if (_writecompleteCallback)
		_conn->SetWriteCompleteCallback(_writecompleteCallback);
	_conn->activationChannel();
	connectionMap[currLoop].insert(std::make_pair(fd, _conn));
	//std::cout << "connection nums:" << connectionMap.size() << std::endl;s
}
void TCPserver::newConnectioninLoop(int fd, sockaddr_in cliaddr)
{
	Eventloop* currLoop = _looppool.getNextLoop();
	if (!currLoop->isQueueInLoop())
	{
		//���Ǹ�fd����һ����������æ
		close(fd);
		std::cout << "add func is error" << std::endl;
	}
	currLoop->queueInLoop(std::bind(&TCPserver::newConnnection, this, fd, cliaddr, currLoop));
}
//void TCPserver::newConnnection1(int fd, sockaddr_in cliaddr, Eventloop* currLoop, ConnectionPtr _conn)//���½�connection�����񶪸�IO�߳��Լ�ȥִ��
//{
//	_conn->activationChannel();
//	connectionMap[currLoop].insert(std::make_pair(fd, _conn));
//}
//void TCPserver::newConnectioninLoop1(int fd, sockaddr_in cliaddr)
//{
//	Eventloop* currLoop = _looppool.getNextLoop();
//	ConnectionPtr _conn(new Connection(fd, currLoop, cliaddr));
//	_conn->SetHandleClose(std::bind(&TCPserver::removeConnection, this, std::placeholders::_1, std::placeholders::_2));
//	_conn->SetMessageCallback(_messageCallback);
//	if (_writecompleteCallback)
//		_conn->SetWriteCompleteCallback(_writecompleteCallback);
//	//connectionMap[currLoop].insert(std::make_pair(fd, _conn));
//	currLoop->queueInLoop(std::bind(&TCPserver::newConnnection1, this, fd, cliaddr, currLoop,_conn->shared_from_this()));
//}
void TCPserver::start()
{
	handleSIGPIPE();
	_acceptor.reset(new Acceptor(_loop));
	
	_acceptor->setnewConnCallback(std::bind(&TCPserver::newConnectioninLoop, this, std::placeholders::_1, std::placeholders::_2));
	//_acceptor->setnewConnCallback(std::bind(&TCPserver::newConnnection1, this, std::placeholders::_1, std::placeholders::_2));
	
	_looppool.start();
	//��ʼ��connectionMap
	std::vector<Eventloop*> loops = _looppool.getAllLoops();
	if (!loops.empty())//IO��
	{
		for (Eventloop* l : loops)
		{
			std::map<int, ConnectionPtr> p;
			connectionMap.insert(std::make_pair(l, p));
		}
	}
	else//��ѭ��
	{
		std::map<int, ConnectionPtr> p;
		connectionMap.insert(std::make_pair(_loop, p));
	}
	_acceptor->start();
}

