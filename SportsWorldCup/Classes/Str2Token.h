#pragma once

// hsnks100@gmail.com LitQoo.한경수
#include <string>
using namespace std;
class Str2Token
{
private:
	int index;
	int pos;
	string delim;
	const string& targetText;
public:
	Str2Token(const string& target, const string& d) : targetText(target), delim(d)
	{
		pos = index = 0;
	}
	
	bool hasToken()
	{
		return pos != string::npos && pos != targetText.size()-1;
	}
	string nextToken()
	{
		pos = targetText.find(delim, index);
		string ret = targetText.substr(index, pos - index);
		index = pos + 1;
		return ret;
	}
};