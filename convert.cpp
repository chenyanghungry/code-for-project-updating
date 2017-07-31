#include "convert.h"
#include<sstream>	

QString s2q(const string &s)
{
	return QString(QString::fromLocal8Bit(s.c_str()));
}
string q2s(const QString &s)
{
	return string((const char *)s.toLocal8Bit());
}

string d2s(double value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}