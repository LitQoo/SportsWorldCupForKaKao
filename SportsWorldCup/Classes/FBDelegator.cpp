#include "FBDelegator.h"
#include "FBConnector.h"

#include "cocos2d.h"
#include <sstream>
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#ifdef __cplusplus
extern "C" {
#endif
        
    void Java_com_litqoo_lib_FBConnectorBase_sendFBLoginInfo(JNIEnv *env, jobject thiz, jstring datas, int delekey)
	{
        
        CCLog("getfunc sendFBLoginInfo %d",delekey);
        
		jboolean isCopy = JNI_FALSE;
		const char* revStr = env->GetStringUTFChars(datas, &isCopy); // UTF-8 πÆ¿⁄∏¶ æÚæÓø».
		string throwData = revStr;
        FBDelegator::DeleSel delesel = FBDelegator::getInstance()->getDeleSel(delekey);
		if(delesel.target != 0 && delesel.selector != 0)
        {
            CCLog("getfunc sendFBLoginInfo %s -",throwData.c_str());
            
            JsonBox::Value getdata;
            getdata.loadFromString(throwData);
            JsonBox::Object user = getdata.getObject();
            JsonBox::Object userinfo = user["user"].getObject();
            FBConnector::get()->setUser(userinfo["id"].getString());
            FBConnector::get()->setUserName(userinfo["name"].getString());
            ((delesel.target)->*(delesel.selector))(getdata);
            CCLog("getfunc sendFBLoginInfo %s -",throwData.c_str());
        }
        
        FBDelegator::getInstance()->removeDeleSel(delekey);
        
        env->ReleaseStringUTFChars(datas, revStr);
        
        return;
        
        
	}
    
    void Java_com_litqoo_lib_FBConnectorBase_sendFBSingleData(JNIEnv *env, jobject thiz, jstring datas, int delekey)
	{
		CCLog("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		jboolean isCopy = JNI_FALSE;
		const char* revStr = env->GetStringUTFChars(datas, &isCopy); // UTF-8 πÆ¿⁄∏¶ æÚæÓø».
		string throwData = revStr;
        FBDelegator::DeleSel delesel = FBDelegator::getInstance()->getDeleSel(delekey);
		if(delesel.target != 0 && delesel.selector != 0)
        {
            CCLog("sendFBSingleData call c++ func %s -",throwData.c_str());
            JsonBox::Value getdata;
            getdata.loadFromString(throwData);
            ((delesel.target)->*(delesel.selector))(getdata);
            CCLog("sendFBSingleData called c++ func %s ",throwData.c_str());
        }
        
        FBDelegator::getInstance()->removeDeleSel(delekey);
        
        env->ReleaseStringUTFChars(datas, revStr);
        CCLog("??????????????????????????????????????????????????");
        return;
        
        
	}
    
    void Java_com_litqoo_lib_FBConnectorBase_sendFBArrayData(JNIEnv *env, jobject thiz, jstring datas, int delekey)
	{
        
        
		jboolean isCopy = JNI_FALSE;
		const char* revStr = env->GetStringUTFChars(datas, &isCopy);
		string throwData = revStr;
        JsonBox::Object _buff = FBDelegator::getInstance()->buff;
        ostringstream oss;
        oss << delekey;
        string _key = oss.str();
        
        
        CCLog("sendFBArrayData start %d, %s",delekey,throwData.c_str());
        
        if(throwData=="start"){
            CCLog("START sendFBArrayData");
            JsonBox::Array _new = JsonBox::Array();
            _buff[_key.c_str()]=JsonBox::Array();
            
            
            //            CCArray *_new = new CCArray;
            //            _new->init();
            //            _buff->setObject(_new,_key.c_str());
            //            _new->release();
            env->ReleaseStringUTFChars(datas, revStr);
            return;
        }
        
        if(throwData=="end"){
            CCLog("END sendFBArrayData");
            
            FBDelegator::DeleSel delesel = FBDelegator::getInstance()->getDeleSel(delekey);
            if(delesel.target != 0 && delesel.selector != 0)
            {
                
                JsonBox::Array _buffArray =  _buff[_key.c_str()].getArray();
                JsonBox::Value _v(_buffArray);
                ((delesel.target)->*(delesel.selector))(_v);
                
                FBDelegator::getInstance()->buff[_key.c_str()].setNull();
            }
            FBDelegator::getInstance()->removeDeleSel(delekey);
            env->ReleaseStringUTFChars(datas, revStr);
            
            return;
        }
        
        JsonBox::Value _value;
        _value.loadFromString(throwData);
        JsonBox::Object _new = _value.getObject();
        JsonBox::Array _buffArray =  (FBDelegator::getInstance()->buff[_key.c_str()].getArray());
        _buffArray.push_back(JsonBox::Value(_new));
        FBDelegator::getInstance()->buff[_key.c_str()].setArray(_buffArray);
        

        
        env->ReleaseStringUTFChars(datas, revStr);
        
	}
#ifdef __cplusplus
}
#endif

#endif