#pragma once
#include"TCPserver.h"
#include"Connection.h"
#include"Eventloop.h"
#include<memory>
#include<map>
//�����ӻ����Ǻܶ����Ȳ�д
enum CHECK_STATE { CHECK_STATE_REQUESTLINE = 0, CHECK_STATE_HEADER ,CHECK_STATE_CONTENT};//��״̬����״̬�ֱ��ǽ��������кͽ���ͷ�ļ�
enum LINE_STATUS { LINE_OK = 0, LINE_BAD, LINE_CONTINUE };//��״̬������ȥ�����Ƿ�õ�һ������
enum HTTP_CODE { NO_REQUEST, GET_REQUEST, BAD_REQUEST,INTERNAL_ERROR,NO_SOURCE };//���������е�״̬
enum METHOD { GET = 0, HEAD, UNREALIZED };
enum VERSION { HTTP_11 = 0, HTTP_10 };
enum FILETYPE {HTML=0,JPG,JS,CSS};
static std::map<std::string, std::string> contenttypeMap;
struct requestHeadData//ֻ�����⼸���ֶ�
{
	VERSION version;
	std::string url;
	METHOD method=UNREALIZED;
	bool keep_alive = false;
	int contentLength=0;
	std::string type;
};
class httpServer
{
public:
	httpServer(int threadNum);
	~httpServer();
	void start();
	void setSourceFile(std::string path);
private:
	//std::unique_ptr<TCPserver> tcpserver;
	Eventloop loop;
	TCPserver tcpserver;
	LINE_STATUS parse_line(Buffer& inBuffer, int& index);
	HTTP_CODE parse_requestline(Buffer& inbuffer, int index, requestHeadData& data, CHECK_STATE& checkstate);
	HTTP_CODE parse_header(Buffer& inbuffer, int index, requestHeadData& data, CHECK_STATE& checkstate);
	HTTP_CODE parse_content(Buffer & inBuffer, int& content_length, int lineSize);
	HTTP_CODE parse_http(Buffer& inBuffer, requestHeadData& data);
	HTTP_CODE get_file(std::string& url, struct stat& file_stat, int& fd);
	void parse_contentType(requestHeadData& data);
	//HTTP_CODE parse_url(string& url);//��ʱ��д
	void handleRead(TCPserver::ConnectionPtr connPtr, Buffer& inBuffer);
	void addStatusLine(int status, const char* title,  char* data);
	void addHeader(bool keep_alive,int content_len,  char* data,int dataSize,const                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                char* content_type);
	void addHeader_ico(char * data, int dataSize);
	std::string sourceFilePath;
	int sourceDirfd;





	//char* url;
	//char* version;
	//bool keep_alive;//��ʱ����
	//int content_length;
	//char* host;

};

