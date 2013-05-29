#pragma once



#include "URLDelegator.h"
#include "InAppDelegator.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "jni.h"
#include "android/jni/JniHelper.h"


class JNIKelper
{
private:
	JniMethodInfo minfo;
	jobject jobj;
	
	string className;
public:
	
	void setJNI(string _className);
	void releaseJNI();
	void callJava_buyItem(string itemID, CCObject* target, RecvInAppSelType selector);
	void callJava_addWebView(string utf8url, int x, int y, int sx, int sy,
							 CCObject* target, RecvSelType selector);
	void callJava_simple(string functionName);
	string callJava_getPrice(string itemID);
	string callJava_getUDID();
	string callJava_getDeviceInfo();
	void callJava_otherApp(string packName);
	void callJava_moveWebView(int x, int y);
	void callJava_getDataFromURL(string utf8url, CCObject* target, RecvSelType selector);
	
	string callJava_getUTF8String(string _strkey)
	{
		JniMethodInfo __minfo;
		
		if(JniHelper::getMethodInfo(__minfo, className.c_str(), "getUTF8String", "(Ljava/lang/String;)Ljava/lang/String;"))
		{
			jstring returnedStr = (jstring)__minfo.env->CallObjectMethod(jobj, __minfo.methodID, __minfo.env->NewStringUTF(_strkey.c_str()));
			
			const char* ret = __minfo.env->GetStringUTFChars(returnedStr, JNI_FALSE);
			string realRet = ret;
			__minfo.env->ReleaseStringUTFChars(returnedStr, ret);
			__minfo.env->DeleteLocalRef(__minfo.classID);
			return realRet;
		}
		return _strkey;

		
	}
public:
	static JNIKelper* getInstance()
	{
		static JNIKelper* _ins = 0;
		if(_ins == 0)
			_ins = new JNIKelper();
		return _ins;
	}
private:
	JNIKelper()
	{
		setJNI("com.litqoo.lib.FBConnectorBase");
	}
	~JNIKelper()
	{
	}
};



#endif
