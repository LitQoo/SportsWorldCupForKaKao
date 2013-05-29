#ifndef ____KSBlock__
#define ____KSBlock__
#include "cocos2d.h"
#include <functional>


USING_NS_CC;
template < typename T >
class KSBlock : public CCObject
{
private:
	KSBlock(){}
public:
	static KSBlock* create(std::function<T> tFunc)
	{
		KSBlock* pRet = new KSBlock();
		pRet->_block = tFunc;
		return (KSBlock*)pRet->autorelease();
	}
	void execute(CCObject* o)
	{
		_block(o);
	}
	void execute(CCNode* o)
	{
		_block(o);
	}
	void execute()
	{
		_block();
	}
	void execute(CCNode* o, void* d)
	{
		_block(o, d);
	}
	virtual ~KSBlock(){}
private:
	std::function<T> _block;
};
class KSFuncHelper
{
private:
	
public:
	static CCCallFuncN* create(std::function<void(CCNode*)> lamb)
	{
		return CCCallFuncN::actionWithTarget(
											 KSBlock<void(CCNode*)>::create(lamb),
											 callfuncN_selector(KSBlock<void(CCNode*)>::execute)
											 );
	}
	static CCCallFuncO* create(std::function<void(CCObject*)> lamb, CCObject* obj)
	{
		return CCCallFuncO::actionWithTarget(
											 KSBlock<void(CCObject*)>::create(lamb),
											 callfuncO_selector(KSBlock<void(CCObject*)>::execute),
											 obj
											 );
		
		
	}
	static CCCallFuncND* create(std::function<void(CCNode*, void*)> lamb, void* d)
	{
		return CCCallFuncND::actionWithTarget(KSBlock<void(CCNode*, void*)>::create(lamb),
									   callfuncND_selector(KSBlock<void(CCNode*, void*)>::execute),
											  d);
	}
	static CCCallFunc* create(std::function<void()> lamb)
	{
//		return 0;
		return CCCallFunc::actionWithTarget(KSBlock<void(void)>::create(lamb),
											  callfunc_selector(KSBlock<void(void)>::execute));
	}
private:
	KSFuncHelper(){}
};

#define KSFunc(...) KSFuncHelper::create(std::function<void(void)>( [=](void){ do{__VA_ARGS__}while(0);}) )
#define KSFuncN(VARNAME, ...) KSFuncHelper::create(std::function<void(CCNode*)>([=](CCNode* VARNAME){ do{__VA_ARGS__}while(0);}))
#define KSFuncO(OBJ, VARNAME, ...) KSFuncHelper::create(std::function<void(CCObject*)>([=](CCObject* VARNAME){ do{__VA_ARGS__}while(0); }), OBJ)
#define KSFuncND(OBJ, NODENAME, VOIDNAME, ...) KSFuncHelper::create(std::function<void(CCNode*, void*)>([=](CCNode* NODENAME, void* VOIDNAME){ do{__VA_ARGS__}while(0);}), OBJ)

#define KSFuncRef(...) KSFuncHelper::create(std::function<void(void)>( [&](void){ do{__VA_ARGS__}while(0);}) )
#define KSFuncNRef(VARNAME, ...) KSFuncHelper::create(std::function<void(CCNode*)>([&](CCNode* VARNAME){ do{__VA_ARGS__}while(0);}))
#define KSFuncORef(OBJ, VARNAME, ...) KSFuncHelper::create(std::function<void(CCObject*)>([&](CCObject* VARNAME){ do{__VA_ARGS__}while(0); }), OBJ)
#define KSFuncNDRef(OBJ, NODENAME, VOIDNAME, ...) KSFuncHelper::create(std::function<void(CCNode*, void*)>([&](CCNode* NODENAME, void* VOIDNAME){ do{__VA_ARGS__}while(0);}), OBJ)

#endif