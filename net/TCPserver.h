#pragma once
#include<vector>
#include"Channel.h"
#include"Buffer.h"
#include<map>
#include<memory>
#include"Acceptor.h"
#include"EventLoopThreadpool.h"
class Connection;
class Eventloop;
class TCPserver
{
public:
	typedef std::shared_ptr<Connection> ConnectionPtr;
	typedef std::function<void(TCPserver::ConnectionPtr, Buffer&)> MessageCallback;
	typedef std::function<void(TCPserver::ConnectionPtr)> WriteCompleteCallback;
	typedef std::function<void(ConnectionPtr)> ConnectionCallback;
	TCPserver(Eventloop* loop,int threadNum);
	~TCPserver();
	//void removeConnection(int fd);
	void removeConnection(int fd,Eventloop* currLoop);
	void start();
	void setMessageCallback(MessageCallback cb) { _messageCallback = cb; }
	void setConnectionCompleteCallback(ConnectionCallback cb) { _connectionCompleteCallback = cb; }
private:
	void newConnnection(int fd, sockaddr_in cliaddr, Eventloop* currLoop);
	void newConnectioninLoop(int fd,sockaddr_in cliaddr);
	std::unique_ptr<Acceptor> _acceptor;
	//std::map<int, ConnectionPtr> connectionMap;//���map����ָ�����������Զ�����
	std::map<Eventloop*, std::map<int, ConnectionPtr>> connectionMap;
	std::map<int, Eventloop*> loopMap;//�������fd�����ڵ�loop,û����
	Eventloop* _loop;
	MessageCallback _messageCallback;
	WriteCompleteCallback _writecompleteCallback;
	ConnectionCallback _connectionCompleteCallback;
	EventLoopThreadpool _looppool;//version14 
};

