#pragma once
#include"Channel.h"
#include<memory>
#include<netinet/in.h>
#include"Buffer.h"
#include"TCPserver.h"
#include<arpa/inet.h>
class Connection :public std::enable_shared_from_this<Connection>
{
	typedef std::function<void(int,Eventloop*)> CloseCallback;
public:
	Connection(int sockfd, Eventloop* loop, struct sockaddr_in cliaddr);
	~Connection();
	void SetHandleClose(CloseCallback cb);
	void SetMessageCallback(TCPserver::MessageCallback cb) { _messageCallback = cb; }
	void SetWriteCompleteCallback(TCPserver::WriteCompleteCallback cb) { _writecompleteCallback = cb; }
	int getSockfd() { return fd; }//û��send��������ʱ��sockfd�����������
	void send(char* data,size_t len);
	void handleClose(int fd);
	void send(std::string data);
	void activationChannel();
	inline sockaddr_in getAddr()
	{
		return _cliaddr;
	}
private:
	void handleRead(int fd);
	void handleWrite(int fd);
	void sendinLoop(char* data, size_t len);
	void defaultConnectionCallback();

	int fd;//��ʱ����Ҳû��
	Eventloop* _loop;
	std::shared_ptr<Channel> channel;//connection����channel����ָ�룬connection����ʱchannel�Զ�����
	Buffer inBuffer;//��socket������
	Buffer outBuffer;
	//Buffer outBuffer//��socketд���ݣ����������
	CloseCallback _closeCallback;
	TCPserver::MessageCallback _messageCallback;
	TCPserver::WriteCompleteCallback _writecompleteCallback;
	TCPserver::ConnectionCallback _connectionCallback;
	sockaddr_in _cliaddr;//��ͨ�ŶԶ�IP��˿�

};

