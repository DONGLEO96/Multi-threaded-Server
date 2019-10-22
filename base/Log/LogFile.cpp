#include "LogFile.h"



LogFile::LogFile(const string& filename, int flushEveryN):_filename(filename),_flushEveryN(flushEveryN),_count(0),_mutex()
{
	_file.reset(new FileUtil(_filename));
}


LogFile::~LogFile()
{
}

void LogFile::append(const char* logline, int len)
{
	MutexGuard lock(_mutex);
	append_unlocked(logline, len);
}
void LogFile::flush()
{
	MutexGuard lock(_mutex);
	_file->flush();
}
void LogFile::append_unlocked(const char* logline, int len)
{
	_file->append(logline, len);//���׼IOд��д�������˾�ˢϴһ��
	++_count;
	if (_count >= _flushEveryN)
	{
		_count = 0;
		_file->flush();
	}
}