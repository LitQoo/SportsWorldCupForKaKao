//
//  FBJniHelper.h
//  archervszombie
//
//  Created by LitQoo on 13. 1. 7..
//
//

#ifndef archervszombie_FBJniHelper_h
#define archervszombie_FBJniHelper_h
#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "FBDelegator.h"
#include "JNIKelper.h"
class FBJniHelper {
    
private:
	JniMethodInfo minfo;
	jobject jobj;
public:
    static FBJniHelper* getInstance()
	{
        
        
		static FBJniHelper* _ins = 0;
		if(_ins == 0)
			_ins = new FBJniHelper();
		return _ins;
	}
    void setJNI()
	{
        
        
        CCLog("set JNI");
		if(JniHelper::getStaticMethodInfo(minfo, "com.litqoo.lib.FBConnectorBase", "getActivity", "()Ljava/lang/Object;"))
		{
            
            CCLog("set JNI2");
			jobj =  minfo.env->NewGlobalRef(minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID));
            
            
            CCLog("set JNI3");
			minfo.env->DeleteLocalRef(minfo.classID);
		}
	}
    void callJava_fblogin(CCObject *target, FBSelType selector)
	{
        CCLog("calljava_Login1");
		JniMethodInfo __minfo;
		if(JniHelper::getMethodInfo(__minfo, "com.litqoo.lib.FBConnectorBase", "fb_login", "(I)V"))
		{
            
            CCLog("calljava_Login2");
			int getKey = FBDelegator::getInstance()->addDeleSel(target, selector);
			__minfo.env->CallVoidMethod(jobj, __minfo.methodID,getKey);
			__minfo.env->DeleteLocalRef(__minfo.classID);
            
            CCLog("calljava_Login");
		}
	}
    
    void callJava_fbscores(string appID,string userID,CCObject *target,FBSelType selector)
	{
        CCLog("calljava_fbsocres1");
		JniMethodInfo __minfo;
		if(JniHelper::getMethodInfo(__minfo, "com.litqoo.lib.FBConnectorBase", "fb_getscores", "(Ljava/lang/String;Ljava/lang/String;I)V"))
		{
            
            CCLog("calljava_fbsocres2");
			int getKey = FBDelegator::getInstance()->addDeleSel(target, selector);
			__minfo.env->CallVoidMethod(jobj,
                                        __minfo.methodID,
                                        __minfo.env->NewStringUTF(appID.c_str()),
                                        __minfo.env->NewStringUTF(userID.c_str()),
                                        getKey);
			__minfo.env->DeleteLocalRef(__minfo.classID);
            
            CCLog("calljava_fbsocres3");
		}
	}
    
    void callJava_fbgetreuqests(string uID,CCObject *target,FBSelType selector)
	{
        CCLog("callJava_fbgetreuqests");
		JniMethodInfo __minfo;
		if(JniHelper::getMethodInfo(__minfo, "com.litqoo.lib.FBConnectorBase", "fb_getrequest", "(Ljava/lang/String;I)V"))
		{
			int getKey = FBDelegator::getInstance()->addDeleSel(target, selector);
			__minfo.env->CallVoidMethod(jobj,
                                        __minfo.methodID,
                                        __minfo.env->NewStringUTF(uID.c_str())
                                        ,getKey);
			__minfo.env->DeleteLocalRef(__minfo.classID);
		}
	}
    
    void callJava_sendRequest(string pID, string pMsg, string pData,CCObject *target,FBSelType selector)
	{
		JniMethodInfo __minfo;
		if(JniHelper::getMethodInfo(__minfo,"com.litqoo.lib.FBConnectorBase", "fb_sendRequest", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V"))
		{
            int getKey = FBDelegator::getInstance()->addDeleSel(target, selector);
			__minfo.env->CallVoidMethod(jobj,
                                        __minfo.methodID,
                                        __minfo.env->NewStringUTF(pID.c_str()),
                                        __minfo.env->NewStringUTF(pMsg.c_str()),
                                        __minfo.env->NewStringUTF(pData.c_str()),
                                        getKey
                                        );
			__minfo.env->DeleteLocalRef(__minfo.classID);
		}
	}
    
    void callJava_removeRequest(string rID,CCObject *target,FBSelType selector)
	{
		JniMethodInfo __minfo;
		if(JniHelper::getMethodInfo(__minfo, "com.litqoo.lib.FBConnectorBase", "fb_removeRequest", "(Ljava/lang/String;I)V"))
		{
            int getKey = FBDelegator::getInstance()->addDeleSel(target, selector);
			__minfo.env->CallVoidMethod(jobj,
                                        __minfo.methodID,
                                        __minfo.env->NewStringUTF(rID.c_str()),
                                        getKey
                                        );
			__minfo.env->DeleteLocalRef(__minfo.classID);
		}
	}
    
    void callJava_sendInvite(string fID,string pAppName,string pInviteMsg,string pLink,string pIconUrl,CCObject *target,FBSelType selector){
        
        JniMethodInfo __minfo;
		if(JniHelper::getMethodInfo(__minfo, "com.litqoo.lib.FBConnectorBase", "fb_sendInvite", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V"))
		{
            int getKey = FBDelegator::getInstance()->addDeleSel(target, selector);
			__minfo.env->CallVoidMethod(jobj,
                                        __minfo.methodID,
                                        __minfo.env->NewStringUTF(fID.c_str()),
                                        __minfo.env->NewStringUTF(pAppName.c_str()),
                                        __minfo.env->NewStringUTF(pInviteMsg.c_str()),
                                        __minfo.env->NewStringUTF(pLink.c_str()),
                                        __minfo.env->NewStringUTF(pIconUrl.c_str()),
                                        getKey
                                        );
			__minfo.env->DeleteLocalRef(__minfo.classID);
		}
        
    }
    
    void callJava_fbsendscore(string uID,int pScore,CCObject *target,FBSelType selector){
		CCLog("lskadjflksajdf id is %s",uID.c_str());
        JniMethodInfo __minfo;
		if(JniHelper::getMethodInfo(__minfo, "com.litqoo.lib.FBConnectorBase", "fb_sendScore", "(Ljava/lang/String;II)V"))
		{
            int getKey = FBDelegator::getInstance()->addDeleSel(target, selector);
			__minfo.env->CallVoidMethod(jobj,
                                        __minfo.methodID,
                                        __minfo.env->NewStringUTF(uID.c_str()),
                                        pScore,
                                        getKey
                                        );
			__minfo.env->DeleteLocalRef(__minfo.classID);
		}
        
    }
    void callJava_simple(string functionName)
	{
        
        CCLog("call java_simple1");
		JniMethodInfo __minfo;
		if(JniHelper::getMethodInfo(__minfo, "com.litqoo.lib.FBConnectorBase", functionName.c_str(), "()V"))
		{
            CCLog("call java_simple2");
            CCLog("call %x,%x,%x",__minfo,jobj,__minfo.methodID);
			__minfo.env->CallVoidMethod(jobj, __minfo.methodID);
            CCLog("call java_simple3");
			__minfo.env->DeleteLocalRef(__minfo.classID);
            CCLog("call java_simple4");
		}
        
        CCLog("call java_simple5");
		
	}
    
private:
    FBJniHelper(){
        this->setJNI();
    }
    ~FBJniHelper(){
        
    }
};
#endif

#endif
