#pragma once

#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "android/jni/JniHelper.h"
#include "jni.h"
#endif

#include "JNIKelper.h"

#include "JsonBox.h"
//#include "FBConnector.h"
using namespace cocos2d;
using namespace std;
typedef void (CCObject::*FBSelType)(JsonBox::Value);
#define fb_selector(_SELECTOR) (FBSelType)(&_SELECTOR)
class FBDelegator
{
    
public:
	
	struct DeleSel
	{
		CCObject* target;
		FBSelType selector;
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
	int addDeleSel(CCObject* target, FBSelType selector)
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
		if(DeleSelTable.find(_key) != DeleSelTable.end())
		{
			
			return DeleSelTable[_key];
		}
		else
		{
			//CCLog("not found!!!");
			DeleSel _t;
			_t.target = 0;
			_t.selector = 0;
			return _t;
		}
	}
    
public:
	static FBDelegator* getInstance()
	{
		static FBDelegator* _ins = 0;
		if(_ins == 0)
			_ins = new FBDelegator();
		return _ins;
	}
    
private:
	FBDelegator()
	{
		key = 0;
//        buff = new CCDictionary;
        buff=JsonBox::Object();
	}
	~FBDelegator(){}
private:
	map<int, DeleSel> DeleSelTable;
	int key;
    
public:
    JsonBox::Object buff;
};

