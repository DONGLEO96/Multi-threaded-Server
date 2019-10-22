#pragma once
#include"FileUtil.h"
#include"Mutex.h"
#include<memory>
using namespace std;
class LogFile//��FileUtil��д�룬ÿд��̶�������ˢϴһ�λ�����
{
public:
	LogFile(const string& filename, int flushEveryN = 1024);
	~LogFile();
	void append(const char* logline, int len);
	void flush();
private:
	void append_unlocked(const char* logline, int len);
	string _filename;
	int _flushEveryN;
	int _count;
	Mutex _mutex;
	unique_ptr<FileUtil> _file;
};

