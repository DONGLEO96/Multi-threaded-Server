#pragma once
#include<string>
using namespace std;
class FileUtil//���ļ���׼IO���з�װ
{
public:
	FileUtil(string& filename);
	~FileUtil();
	void append(const char* ligline, size_t len);
	void flush();
private:
	size_t write(const char* logline, size_t len);
	FILE* _file;
	char buffer[64 * 1024];
};

