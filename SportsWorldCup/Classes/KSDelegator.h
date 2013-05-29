//
//  KSDelegator.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 1. 24..
//
//

#ifndef __SportsWorldCup__KSDelegator__
#define __SportsWorldCup__KSDelegator__

#include "cocos2d.h"
#include <map>
using namespace std;
USING_NS_CC;
template<typename selT>
class KSDelegator
{
	
public:
	struct DeleSel
	{
		CCObject* target;
		selT selector;
	};
	
	
public:
	void removeTarget(CCObject* _obj)
	{
		// DeleTable<int, DeleSel>
		for(auto iter = DeleSelTable.begin(); iter != DeleSelTable.end();)
		{
			if(iter->second.target == _obj)
			{
				DeleSelTable.erase(iter++);
			}
			else
				++iter;
		}
	}
	int addDeleSel(CCObject* target, selT selector)
	{
		key++;
		DeleSel temp;
		temp.target = target;
		temp.selector = selector;
		DeleSelTable[key] = temp;
		return key;
	}
	void removeDeleSel(int _key)
	{
		if(DeleSelTable.find(_key) != DeleSelTable.end())
			DeleSelTable.erase(_key);
	}
	DeleSel getDeleSel(int _key)
	{
		// find
		if(DeleSelTable.find(_key) != DeleSelTable.end())
		{
			return DeleSelTable[_key];
		}
		else
		{
			DeleSel _t;
			_t.target = 0;
			_t.selector = 0;
			return _t;
		}
		
	}
	
public:
	static KSDelegator* getInstance()
	{
		static KSDelegator* _ins = 0;
		if(_ins == 0)
			_ins = new KSDelegator();
		return _ins;
	}
	
private:
	KSDelegator()
	{
		key = 0;
	}
	virtual ~KSDelegator(){}
private:
	map<int, DeleSel> DeleSelTable;
	int key;
};

#endif /* defined(__SportsWorldCup__KSDelegator__) */
