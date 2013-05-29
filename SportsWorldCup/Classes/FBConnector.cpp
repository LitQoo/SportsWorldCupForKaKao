//
//  FBConnector.m
//  archervszombie
//
//  Created by LitQoo on 12. 12. 17..
//
//

#include "FBConnector.h"
#include "KS_Util.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "AppController.h"
#endif


class CCImage2 : public CCImage
{
public:
	
	bool initWithImageData(void * pData,
									int nDataLen,
						   EImageFormat eFmt = CCImage::kFmtUnKnown/* = eSrcFmtPng*/,
									int nWidth = 0/* = 0*/,
									int nHeight = 0 /* = 0*/,
									int nBitsPerComponent = 8/* = 8*/)
	{
		bool bRet = false;
		do
		{
			CC_BREAK_IF(! pData || nDataLen <= 0);
			
			if (kFmtPng == eFmt)
			{
				bRet = _initWithPngData(pData, nDataLen);
				break;
			}
			else if (kFmtJpg == eFmt)
			{
				bRet = _initWithJpgData(pData, nDataLen);
				break;
			}
			else if (kFmtRawData == eFmt)
			{
				bRet = _initWithRawData(pData, nDataLen, nWidth, nHeight, nBitsPerComponent);
				break;
			}else
			{
				// if it is a png file buffer.
				if (nDataLen > 8)
				{
					unsigned char* pHead = (unsigned char*)pData;
					if (   pHead[0] == 0x89
						&& pHead[1] == 0x50
						&& pHead[2] == 0x4E
						&& pHead[3] == 0x47
						&& pHead[4] == 0x0D
						&& pHead[5] == 0x0A
						&& pHead[6] == 0x1A
						&& pHead[7] == 0x0A)
					{
						bRet = _initWithPngData(pData, nDataLen);
						break;
					}
				}				
				// if it is a jpeg file buffer.
				if (nDataLen > 2)
				{
					unsigned char* pHead = (unsigned char*)pData;
					if (   pHead[0] == 0xff
						&& pHead[1] == 0xd8)
					{
						bRet = _initWithJpgData(pData, nDataLen);
						break;
					}
				}
			}
		} while (0);
		return bRet;
	}
	

};

void FBConnector::init(){
    downloadIndex=0;
    imagesDown=false;
	sprites = CCArray::array();
	sprites->retain(); // 전역이라서
    sprites->init();
    userID=CCUserDefault::sharedUserDefault()->getStringForKey("FBuserID");
    if(userID==""){
        sendPermission=false;
    }else{
        sendPermission=CCUserDefault::sharedUserDefault()->getBoolForKey("FBsendPermission");
    }
	CCScheduler::sharedScheduler()->scheduleSelector(schedule_selector(FBConnector::imageDown), this, 0,false);
}

void FBConnector::setSendPermission(bool state){
    sendPermission=state;
    CCUserDefault::sharedUserDefault()->setBoolForKey("FBsendPermission", state);
    CCUserDefault::sharedUserDefault()->flush();
}
bool FBConnector::isUsed(){
    if(userID=="")return false;
    return true;
}

bool FBConnector::isLogin(){
    if(userName=="")return false;
    return true;
}

void FBConnector::setUser(string fID){
    CCLog("setuser : %",fID.c_str());
	userID=fID;
    CCUserDefault::sharedUserDefault()->setStringForKey("FBuserID", fID.c_str());
    CCUserDefault::sharedUserDefault()->flush();
}

void FBConnector::setUserName(string fName){
    CCLog("setuser : %",fName.c_str());
    userName=fName;
}

void FBConnector::setup(string pAppID){
    appID=pAppID;
}

void FBConnector::CreateNewSession()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    FBSession* session = [[FBSession alloc] init];
    [FBSession setActiveSession: session];
#endif
}

void FBConnector::login(CCObject *target,FBSelType selector){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NSArray *permissions = [[NSArray alloc] initWithObjects:
                            @"user_birthday",
                            nil];
	
	int delekey=0;
    delekey=FBDelegator::getInstance()->addDeleSel(target,selector);

    
    // Attempt to open the session. If the session is not open, show the user the Facebook login UX
    [FBSession openActiveSessionWithReadPermissions:permissions allowLoginUI:true completionHandler:^(FBSession *session,
                                                                                                      FBSessionState status,
                                                                                                      NSError *error)
     {
    
         // Did something go wrong during login? I.e. did the user cancel?
         if (status == FBSessionStateClosedLoginFailed || status == FBSessionStateCreatedOpening) {
             
             // If so, just send them round the loop again
             [[FBSession activeSession] closeAndClearTokenInformation];
             [FBSession setActiveSession:nil];
             CreateNewSession();
			 
			 NSLog(@"login fail %@,%@",session,error);
			 
			 if(status != FBSessionStateCreatedOpening){
				 JsonBox::Value result;
				 result.loadFromString("{\"error\":1}");
				 FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
				 FBDelegator::getInstance()->removeDeleSel(delekey);
				 if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
			 }
         }
         else
         {
             AppController *appDelegate = (AppController *)[[UIApplication sharedApplication]delegate];
             
             // Required to initialise the old SDK FB object here so we can play with Dialogs
             appDelegate.facebook = [[Facebook alloc] initWithAppId:[FBSession activeSession].appID andDelegate:nil];
             
             // Initialise the old SDK with our new credentials
             appDelegate.facebook.accessToken = [FBSession activeSession].accessToken;
             appDelegate.facebook.expirationDate = [FBSession activeSession].expirationDate;
             if(userName=="")this->customize(delekey);
         }
         
     }];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    FBJniHelper::getInstance()->callJava_fblogin(target,selector);
#endif
}




void FBConnector::logout(){
	CCLog("logout");
    this->setUser("");
    this->userName="";
    this->setSendPermission(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication]delegate];
    
    [appDelegate.facebook logout:nil];
    [[FBSession activeSession] closeAndClearTokenInformation];
    [FBSession setActiveSession:nil];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    FBJniHelper::getInstance()->callJava_simple("fb_logout");
#endif
}


void FBConnector::getScores(CCObject *target,FBSelType selector)
{
	int delekey = FBDelegator::getInstance()->addDeleSel(target, selector);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCLog("start load scores");
    [FBRequestConnection startWithGraphPath:[NSString stringWithFormat:@"%s/scores?fields=score,user", appID.c_str()] parameters:nil HTTPMethod:@"GET" completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {

        if (result && !error)
        {
            JsonBox::Array *scoreArray = new JsonBox::Array;
            
            //CCArray *scoreArray = new CCArray;
            //scoreArray->init();
            for (NSDictionary *dict in [result objectForKey:@"data"])
            {
                JsonBox::Object dic;
                JsonBox::Object user;
                dic["score"]=JsonBox::Value([[dict objectForKey:@"score"] intValue]);
                user["id"]=JsonBox::Value([[[dict objectForKey:@"user"] objectForKey:@"id"] cStringUsingEncoding:NSUTF8StringEncoding]);
                user["name"]=JsonBox::Value([[[dict objectForKey:@"user"] objectForKey:@"name"] cStringUsingEncoding:NSUTF8StringEncoding]);
                
                dic["user"]=JsonBox::Value(user);
                
                scoreArray->push_back(JsonBox::Value(dic));
                //CCDictionary *dic = new CCDictionary;
     
//                CCInteger *score = CCInteger::create([[dict objectForKey:@"score"] intValue]);
//                CCString *id = CCString::create([[[dict objectForKey:@"user"] objectForKey:@"id"] cStringUsingEncoding:NSUTF8StringEncoding]);
//                CCString *name = CCString::create([[[dict objectForKey:@"user"] objectForKey:@"name"] cStringUsingEncoding:NSUTF8StringEncoding]);
//                CCString *imgurl = CCString::createWithFormat("graph.facebook.com/%s/picture?width=64&height=64",id->getCString());
//     
//                dic->setObject(score, "score");
//                dic->setObject(id, "id");
//                dic->setObject(name, "name");
//                dic->setObject(imgurl, "imgurl");
    
//                scoreArray->addObject(dic);
//                dic->release();
            }
     
            //(target->*selector)(scoreArray);
            //scoreArray->release();
            //get friends list and push to resultarray without overlap friends?field=name,id
     
     NSString *query =
     @"SELECT uid, name FROM user WHERE uid IN "
     @"(SELECT uid2 FROM friend WHERE uid1 = me()) ORDER BY rand() limit 30";
     // Set up the query parameter
     NSDictionary *queryParam =
     [NSDictionary dictionaryWithObjectsAndKeys:query, @"q", nil];
     
     //CCString* query = CCString::createWithFormat("SELECT uid, name FROM user WHERE uid IN (SELECT uid2 FROM friend WHERE uid1 = %s) ORDER BY rand() limit 10",userID.c_str());
     [FBRequestConnection startWithGraphPath:@"/fql"
                                  parameters:queryParam
                                  HTTPMethod:@"GET" completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
									  
									  //NSLog(@"scores log %@,%@",result,error); //##
                if (result && !error)
                {
                    //CCArray *resultArray = new CCArray;
                    //resultArray->init();
                        for (NSDictionary *dict in [result objectForKey:@"data"])
                        {
                            
                            string _id = [[NSString stringWithFormat:@"%llu",[[dict objectForKey:@"uid"] longLongValue]] cStringUsingEncoding:NSUTF8StringEncoding];
                            string _name = [[dict objectForKey:@"name"] cStringUsingEncoding:NSUTF8StringEncoding];
                            
                            bool isfind=false;
                            
                            for(int i=0;i<scoreArray->size();i++){
                                JsonBox::Value _value = JsonBox::Value((scoreArray->at(i)));
                                JsonBox::Object sdic = _value.getObject();
                                JsonBox::Object suser = sdic["user"].getObject();
                                string sid = suser["id"].getString();
                                if(sid==_id){
                                    isfind=true;
                                    break;
                                }
                            }
             
                            if(isfind==false){
                                JsonBox::Object _dic;
                                JsonBox::Object _user;
                                _user["id"]=JsonBox::Value(_id);
                                _user["name"]=JsonBox::Value(_name);
                                _dic["user"]=JsonBox::Value(_user);
                                _dic["score"]=JsonBox::Value(-1);
                                
                                scoreArray->push_back(JsonBox::Value(_dic));
//                                CCDictionary *dic = new CCDictionary;
//             
//                                dic->setObject(_id, "id");
//                                dic->setObject(_name, "name");
//                                dic->setObject(imgurl, "imgurl");
//                                dic->setObject(CCInteger::create(-1), "score");
//                                scoreArray->addObject(dic);
//                                dic->release(); 

                            }
                        }
      
                    CCLog("load scores1");
					
					FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
					FBDelegator::getInstance()->removeDeleSel(delekey);
					if(delesel.target!=0 && delesel.selector!=0)
						((delesel.target)->*(delesel.selector))(JsonBox::Value(*scoreArray));
					
                }else{
                    CCLog("load scores2");
					FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
					FBDelegator::getInstance()->removeDeleSel(delekey);
					if(delesel.target!=0 && delesel.selector!=0)
						((delesel.target)->*(delesel.selector))(JsonBox::Value(*scoreArray));
					
                }
                delete scoreArray;
                }];
            
            //delete scoreArray;
            //friends end
     
     
     
        }else{
            JsonBox::Value result;
			result.loadFromString("{\"error\":5}");
			FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
			FBDelegator::getInstance()->removeDeleSel(delekey);
			if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
			
        }
        
    }];
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    CCLog("calljava fbscores 1");
    FBJniHelper::getInstance()->callJava_fbscores(appID,userID,target,selector);
    CCLog("calljava fbscores 2");
#endif
}



void FBConnector::getRequests(CCObject *target,FBSelType selector)
{
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	int delekey = FBDelegator::getInstance()->addDeleSel(target, selector);
    [FBRequestConnection startWithGraphPath:[NSString stringWithFormat:@"%s/apprequests",userID.c_str()] parameters:nil HTTPMethod:@"GET" completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
     
     if (result && !error)
     {
         JsonBox::Array resultArray;
//         CCArray *resultArray = new CCArray;
//         resultArray->init();
         for (NSDictionary *dict in [result objectForKey:@"data"])
         {  
             JsonBox::Object dic;
             JsonBox::Object from;
             from["id"]=JsonBox::Value([[[dict objectForKey:@"from"] objectForKey:@"id"] cStringUsingEncoding:NSUTF8StringEncoding]);
             from["name"]=JsonBox::Value([[[dict objectForKey:@"from"] objectForKey:@"name"] cStringUsingEncoding:NSUTF8StringEncoding]);
             dic["from"]=JsonBox::Value(from);
             dic["id"]=JsonBox::Value([[dict objectForKey:@"id"] cStringUsingEncoding:NSUTF8StringEncoding]);
             dic["message"]=JsonBox::Value([[dict objectForKey:@"message"] cStringUsingEncoding:NSUTF8StringEncoding]);
             dic["created_time"]=JsonBox::Value([[dict objectForKey:@"created_time"] cStringUsingEncoding:NSUTF8StringEncoding]);
             dic["data"]=JsonBox::Value([[dict objectForKey:@"data"] cStringUsingEncoding:NSUTF8StringEncoding]);
////             CCDictionary *dic = new CCDictionary;
//             
//             CCString *rid = CCString::create([[dict objectForKey:@"id"] cStringUsingEncoding:NSUTF8StringEncoding]);
//             CCString *id = CCString::create([[[dict objectForKey:@"from"] objectForKey:@"id"] cStringUsingEncoding:NSUTF8StringEncoding]);
//             CCString *name = CCString::create([[[dict objectForKey:@"from"] objectForKey:@"name"] cStringUsingEncoding:NSUTF8StringEncoding]);
//             CCString *message = CCString::create([[dict objectForKey:@"message"] cStringUsingEncoding:NSUTF8StringEncoding]);
//             CCString *imgurl = CCString::createWithFormat("graph.facebook.com/%s/picture?width=64&height=64",id->getCString());
//     
//            CCString *time = CCString::create([[dict objectForKey:@"created_time"] cStringUsingEncoding:NSUTF8StringEncoding]);
//            CCString *data = CCString::create([[dict objectForKey:@"data"] cStringUsingEncoding:NSUTF8StringEncoding]);
//             dic->setObject(rid, "rid");
//             dic->setObject(id, "id");
//             dic->setObject(name, "name");
//             dic->setObject(imgurl, "imgurl");
//             dic->setObject(time, "time");
//     dic->setObject(message, "message");
//     dic->setObject(data, "data");
//     
//             //resultArray->addObject(dic);
//             dic->release();
             resultArray.push_back(dic);
         }
         
		 
		 
		 FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
		 FBDelegator::getInstance()->removeDeleSel(delekey);
		 if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(resultArray);
		 
         //resultArray->release();
     }else{
         if(target==NULL)return;
         JsonBox::Value result;
		 result.loadFromString("{\"error\":1}");
		 FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
		 FBDelegator::getInstance()->removeDeleSel(delekey);
		 if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
     }
     
     }];
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    FBJniHelper::getInstance()->callJava_fbgetreuqests(userID.c_str(),target,selector);
#endif
}




void FBConnector::sendScore(int nScore,CCObject *target,FBSelType selector)
{
 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // Make sure we have write permissions
    //RequestWritePermissions();
	int delekey = FBDelegator::getInstance()->addDeleSel(target, selector);
    
    if(sendPermission==false){
		NSArray *permissions = [[NSArray alloc] initWithObjects:@"publish_actions", nil];
		
		[[FBSession activeSession] reauthorizeWithPublishPermissions:permissions defaultAudience:FBSessionDefaultAudienceFriends
												   completionHandler:^(FBSession *session, NSError* error)
		{
			 
			 if(!error){
				 NSMutableDictionary* params =   [NSMutableDictionary dictionaryWithObjectsAndKeys:
												  [NSString stringWithFormat:@"%d", nScore], @"score",
												  nil];
				 
				 setSendPermission(true);
				 // Get the score, and only send the updated score if it's highter
				 [FBRequestConnection startWithGraphPath:[NSString stringWithFormat:@"%s/scores", userID.c_str()] parameters:params HTTPMethod:@"GET" completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
					 if (result && !error) {
						 
						 int nCurrentScore = 0;
						 if([result objectForKey:@"data"]!=NULL && [[result objectForKey:@"data"] count]>0){
							 nCurrentScore= [[[[result objectForKey:@"data"] objectAtIndex:0] objectForKey:@"score"] intValue];
						 }
						 
						 if (nScore > nCurrentScore) {
							 
							 CCLog("highscore");
							 [FBRequestConnection startWithGraphPath:[NSString stringWithFormat:@"%s/scores", userID.c_str()] parameters:params HTTPMethod:@"POST" completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
								 CCLog("complete");
								 
								 if (!error) {
									 JsonBox::Value result;
									 result.loadFromString("{\"error\":0}");
									 FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
									 FBDelegator::getInstance()->removeDeleSel(delekey);
									 if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
								 }else{
									 this->setSendPermission(false);
									 JsonBox::Value result;
									 result.loadFromString("{\"error\":5}");
									 FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
									 FBDelegator::getInstance()->removeDeleSel(delekey);
									 if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);								 }
							 }];
						 }else{
							 JsonBox::Value result;
							 result.loadFromString("{\"error\":0}");
							 FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
							 FBDelegator::getInstance()->removeDeleSel(delekey);
							 if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
						 }
					 }else{
						 
						 JsonBox::Value result;
						 result.loadFromString("{\"error\":5}");
						 FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
						 FBDelegator::getInstance()->removeDeleSel(delekey);
						 if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
					 }
					 
				 }];
			 }else{
				 setSendPermission(false);
				 JsonBox::Value result;
				 result.loadFromString("{\"error\":5}");
				 FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
				 FBDelegator::getInstance()->removeDeleSel(delekey);
				 if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
			 }
		 }];
    }else{
        
        CCLog("permission is true");
        NSMutableDictionary* params =   [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                         [NSString stringWithFormat:@"%d", nScore], @"score",
                                         nil];
        
        // Get the score, and only send the updated score if it's highter
        [FBRequestConnection startWithGraphPath:[NSString stringWithFormat:@"%s/scores", userID.c_str()] parameters:params HTTPMethod:@"GET" completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
			if (result && !error) {
				NSLog(@"%@", result);
				int nCurrentScore = 0;
				if([result objectForKey:@"data"]!=NULL && [[result objectForKey:@"data"] count]>0){
					nCurrentScore= [[[[result objectForKey:@"data"] objectAtIndex:0] objectForKey:@"score"] intValue];
				}
				
				if (nScore > nCurrentScore) {
					
					CCLog("highscore");
					[FBRequestConnection startWithGraphPath:[NSString stringWithFormat:@"%s/scores", userID.c_str()] parameters:params HTTPMethod:@"POST" completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
						CCLog("complete");
						
						if (result && !error) {
							JsonBox::Value result;
							result.loadFromString("{\"error\":0}");
							FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
							FBDelegator::getInstance()->removeDeleSel(delekey);
							if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
						}else{
							NSLog(@"%@", error);
							JsonBox::Value result;
							result.loadFromString("{\"error\":5}");
							FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
							FBDelegator::getInstance()->removeDeleSel(delekey);
							if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
						}
					}];
				}else{
					JsonBox::Value result;
					result.loadFromString("{\"error\":0}");
					FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
					FBDelegator::getInstance()->removeDeleSel(delekey);
					if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
				}
			}else{
				JsonBox::Value result;
				result.loadFromString("{\"error\":5}");
				FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
				FBDelegator::getInstance()->removeDeleSel(delekey);
				if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
			}
			
		}];
		
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    FBJniHelper::getInstance()->callJava_fbsendscore(userID.c_str(),nScore,target,selector);
#endif
}

void FBConnector::sendRequest(string fID,string pMsg,string pData,CCObject *target, FBSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication]delegate];
    [[appDelegate facebook] enableFrictionlessRequests];
	
    int delekey=FBDelegator::getInstance()->addDeleSel(target,selector);
    NSString* s2 = [NSString stringWithCString:pMsg.c_str() encoding:NSUTF8StringEncoding];
    NSMutableDictionary* params =
    [NSMutableDictionary dictionaryWithObjectsAndKeys:
     s2, @"message",
     [NSString stringWithFormat:@"%s",fID.c_str()],@"to",
     [NSString stringWithFormat:@"%s",pData.c_str()],@"data",
     [NSString stringWithFormat:@"%d",delekey],@"delekey",
     [NSString stringWithFormat:@"%s",fID.c_str()],@"returnValue",
     nil];
    
	appDelegate->fbDelekey = delekey;
	appDelegate->fbRValue=fID;
	
    [appDelegate.facebook dialog:@"apprequests" andParams:params andDelegate:appDelegate];

    /*
    NSArray *permissions = [[NSArray alloc] initWithObjects:
                            @"publish_actions",@"publish_stream", nil];
    
    [[FBSession activeSession] reauthorizeWithPublishPermissions:permissions defaultAudience:FBSessionDefaultAudienceFriends completionHandler:^(FBSession *session, NSError* error) {
     
     NSMutableDictionary* postParams =
     [[NSMutableDictionary alloc] initWithObjectsAndKeys:
      [NSString stringWithFormat:@"%s",fID.c_str()],@"to",
      @"asdfasdflks", @"message",
      @"https://developers.facebook.com/ios", @"link",
      @"https://developers.facebook.com/attachment/iossdk_logo.png", @"picture",
      @"Facebook SDK for iOS", @"name",
      @"Build great social apps and get more installs.", @"caption",
      @"The Facebook SDK for iOS makes it easier and faster to develop Facebook integrated iOS apps.", @"description",
      nil];
     
     [FBRequestConnection startWithGraphPath:[NSString stringWithFormat:@"%s/apprequests",fID.c_str()]
                                  parameters:postParams
                                  HTTPMethod:@"POST"
                           completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
      NSLog(@"error %@",error);
      
      }];
     
     }];*/
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    FBJniHelper::getInstance()->callJava_sendRequest(fID,pMsg,pData,target,selector);
#endif
}
void FBConnector::sendRequestResult(int result,int delekey,string _value){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    if(delekey!=0){
        FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
        FBDelegator::getInstance()->removeDeleSel(delekey);
        
        NSString *re= [NSString stringWithFormat:@"{\"error\":%d, \"id\":\"%s\"}",result,_value.c_str()];
        JsonBox::Value result;
		result.loadFromString([re cStringUsingEncoding:NSUTF8StringEncoding]);
		
        if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
    }
#endif
}

void FBConnector::sendInvite(string fID,string pAppName,string pInviteMsg,string pLink, string pIconUrl,CCObject *target, FBSelType selector){
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication]delegate];
    [[appDelegate facebook] enableFrictionlessRequests];
    int delekey=FBDelegator::getInstance()->addDeleSel(target,selector);
    
    NSMutableDictionary* params =
    [NSMutableDictionary dictionaryWithObjectsAndKeys:
     [NSString stringWithFormat:@"%d",delekey],@"delekey",
     [NSString stringWithFormat:@"%s",fID.c_str()],@"to",
     [NSString stringWithFormat:@"%s",pAppName.c_str()], @"name",
     @"IOS & Android Game", @"caption",
	 [NSString stringWithCString:pInviteMsg.c_str() encoding:NSUTF8StringEncoding], @"description",
     [NSString stringWithFormat:@"%s",pLink.c_str()], @"link",
     [NSString stringWithCString:pIconUrl.c_str() encoding:NSUTF8StringEncoding], @"picture",
     [NSString stringWithFormat:@"%s",fID.c_str()],@"returnValue",
     nil];
	
	appDelegate->fbDelekey = delekey;
	appDelegate->fbRValue=fID;
	
    [appDelegate.facebook dialog:@"feed" andParams:params andDelegate:appDelegate];
    
    
    
    /*
    NSArray *permissions = [[NSArray alloc] initWithObjects:
                            @"publish_actions",@"publish_stream", nil];
    
    [[FBSession activeSession] reauthorizeWithPublishPermissions:permissions defaultAudience:FBSessionDefaultAudienceFriends completionHandler:^(FBSession *session, NSError* error) {

     
    NSMutableDictionary* postParams = 
    [[NSMutableDictionary alloc] initWithObjectsAndKeys:
     [NSString stringWithFormat:@"%s",fID.c_str()],@"to",
     @"asdfasdflks", @"message",
     @"https://developers.facebook.com/ios", @"link",
     @"https://developers.facebook.com/attachment/iossdk_logo.png", @"picture",
     @"Facebook SDK for iOS", @"name",
     @"Build great social apps and get more installs.", @"caption",
     @"The Facebook SDK for iOS makes it easier and faster to develop Facebook integrated iOS apps.", @"description",
     nil];
    
    [FBRequestConnection startWithGraphPath:[NSString stringWithFormat:@"%s/feed",fID.c_str()]
                                 parameters:postParams
                                 HTTPMethod:@"POST"
                          completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
     NSLog(@"error %@",error);
     
     }];
     
     }];
     */
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    FBJniHelper::getInstance()->callJava_sendInvite(fID,pAppName,pInviteMsg,pLink,pIconUrl,target,selector);
#endif
}


void FBConnector::removeRequests(string rID,CCObject *target,FBSelType selector){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	
	int delekey = FBDelegator::getInstance()->addDeleSel(target, selector);
	
    [FBRequestConnection startWithGraphPath:[NSString stringWithFormat:@"%s",rID.c_str()] parameters:nil HTTPMethod:@"DELETE" completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {

        if (result && !error){
          
			NSString *re= [NSString stringWithFormat:@"{\"error\":%d, \"id\":\"%s\"}",FBSuccess,rID.c_str()];
			JsonBox::Value result;
			result.loadFromString([re cStringUsingEncoding:NSUTF8StringEncoding]);
			FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
			FBDelegator::getInstance()->removeDeleSel(delekey);
			if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
        }else{
			
			NSString *re= [NSString stringWithFormat:@"{\"error\":%d, \"id\":\"%s\"}",FBErrorETC,rID.c_str()];
			JsonBox::Value result;
			result.loadFromString([re cStringUsingEncoding:NSUTF8StringEncoding]);
			FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
			FBDelegator::getInstance()->removeDeleSel(delekey);
			if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
        }
     }];
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    FBJniHelper::getInstance()->callJava_removeRequest(rID,target,selector);
#endif
}

size_t FBConnector::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    
    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    
    if (mem->memory == NULL) {
        /* out of memory! */
        CCLog("not enough memory (realloc returned NULL)");
        exit(EXIT_FAILURE);
    }
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}







void* FBConnector::t_function(void * _caller)
{
	FBConnector* caller = (FBConnector*)_caller;

	CURL *curl_handle;
	CURLcode retCode = curl_global_init(CURL_GLOBAL_ALL);
	if(retCode != CURLcode::CURLE_OK)
		return NULL;
	curl_handle = curl_easy_init();
    while (1) {
        string pURL;
		CCLog("downloadIndex : %d", FBConnector::get()->downloadIndex);
        if(FBConnector::get()->sprites->count()<=FBConnector::get()->downloadIndex){
            FBConnector::get()->imagesDown=false;
            break;
        }else{
            pURL=((FBSprite *)FBConnector::get()->sprites->objectAtIndex(FBConnector::get()->downloadIndex))->url;
        }
        
        MemoryStruct chunk = {(char*)malloc(1), 0};
		int downI = FBConnector::get()->downloadIndex;
		FBConnector::get()->downloadIndex++;
        curl_easy_setopt(curl_handle, CURLOPT_URL, pURL.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, true);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        if(curl_easy_perform(curl_handle) != CURLcode::CURLE_OK)
			break;
		
		caller->chunks.push(pair<MemoryStruct, int>(chunk, downI));
    }
    curl_easy_cleanup(curl_handle);
	curl_global_cleanup();
    return NULL;
}

CCSprite* FBConnector::getProfileSprite(string userid, int fbw, int fbh){
    //CCSize rSize = CCDirector::sharedDirector()->getDisplaySizeInPixels(); in cocos2d-x 1.x
	CCSize rSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	float sc = rSize.width / CCDirector::sharedDirector()->getWinSize().width;
	fbw *= sc;
	fbh *= sc;
  
    
    string pURL = KS_Util::stringWithFormat("graph.facebook.com/%s/picture?type=square", userid.c_str());//, fbw, fbh);
    //string pURL = CCString::createWithFormat("graph.facebook.com/%s/picture?width=%d&height=%d", userid.c_str(), fbw, fbh)->getCString();
    for(int i=0; i<sprites->count();i++){
        FBSprite *spr = (FBSprite *)sprites->objectAtIndex(i);
        if(spr->url==pURL && spr->isDown==true){
			if(spr->realSprite->getTexture()==NULL){
				FBSprite *_new = new FBSprite;
				return _new;
			}else{
			CCSprite* ret = CCSprite::spriteWithTexture(spr->realSprite->getTexture());
			ret->setScaleX(spr->realSpriteScaleX);
			ret->setScaleY(spr->realSpriteScaleY);
			return ret;
			}
        }
    }
    FBSprite *_new = new FBSprite;
//	_new->w = fbw;
//	_new->h = fbh;
    _new->init(pURL);
	_new->realSpriteScaleX=fbw/50.f;
	_new->realSpriteScaleY=fbh/50.f;
	
	sprites->addObject(_new);
    _new->release();
    
    if(imagesDown==false){
        imagesDown=true;
        pthread_t p_thread;
        int thr_id;
        // 쓰레드 생성 아규먼트로 1 을 넘긴다.
        thr_id = pthread_create(&p_thread, NULL, t_function, this);
        if (thr_id < 0)
        {
            perror("thread create error : ");
            exit(0);
        }
    }
    return _new;
    return NULL;
}

void FBConnector::imageDown(float dt){

	while(!chunks.empty())
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CCImage* img = new CCImage;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		CCImage2* img = new CCImage2; //in cocos2d-x 1.x
#endif
		CCTexture2D* texture = new CCTexture2D();
		auto chunk_index = chunks.front();
		chunks.pop();
		try {
			
			if(img->initWithImageData(chunk_index.first.memory, (long)chunk_index.first.size, CCImage::kFmtUnKnown) == false)
				throw "..";
			
			if(texture->initWithImage(img) == false)
				throw "..";
			
			FBSprite *_sprite = (FBSprite *)sprites->objectAtIndex(chunk_index.second);
			
			_sprite->createRealSprite(texture);
			
			if(chunk_index.first.memory)
				free(chunk_index.first.memory);
			
			
			texture->release();
			delete img;// in cocos2d-x 1.x
		} catch (...) {
			if(chunk_index.first.memory)
				free(chunk_index.first.memory);
			
			
			texture->release();
			delete img;// in cocos2d-x 1.x
		}
	}
}

void FBConnector::customize(int delekey)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	
	
	
	CCLog("login customize");
    // Start the facebook request
    [[FBRequest requestForMe]
     startWithCompletionHandler:
     ^(FBRequestConnection *connection, NSDictionary<FBGraphUser> *result, NSError *error)
     {
         // Did everything come back okay with no errors?
         
         if (!error && result)
         {
             // If so we can extract out the player's Facebook ID and first name
             userID = [result.id cStringUsingEncoding:NSUTF8StringEncoding];
             userName = [result.name cStringUsingEncoding:NSUTF8StringEncoding];
             setUser(userID);
             
             [FBRequestConnection startWithGraphPath:[NSString stringWithFormat:@"%s/scores", userID.c_str()] parameters:nil HTTPMethod:@"GET" completionHandler:^(FBRequestConnection *connection, id result2, NSError *error2) {
                 
                 JsonBox::Object resultObj;
                 
                 if (result2 && !error) {
                     string _id = userID;
                     string _name = [result.name cStringUsingEncoding:NSUTF8StringEncoding];
                     int nCurrentScore = 0;
                     
                     
                     
                     if([result2 objectForKey:@"data"]!=NULL && [[result2 objectForKey:@"data"] count]>0){
                         nCurrentScore= [[[[result2 objectForKey:@"data"] objectAtIndex:0] objectForKey:@"score"] intValue];
                     }
                     
                     resultObj["score"]=JsonBox::Value(nCurrentScore);
                     JsonBox::Object userinfo;
                     userinfo["id"]=JsonBox::Value(_id);
                     userinfo["name"]=JsonBox::Value(_name);
                     resultObj["user"]=JsonBox::Value(userinfo);
					 resultObj["error"] = FBSuccess;
                 }else{
                     resultObj["score"]=JsonBox::Value(0);
                     JsonBox::Object userinfo;
                     userinfo["id"]=JsonBox::Value(userID);
                     userinfo["name"]=JsonBox::Value(userName);
                     resultObj["user"]=JsonBox::Value(userinfo);
					 resultObj["error"] = FBSuccess;
                 }
                 
                 
				 CCLog("login success");
				 
				JsonBox::Value _value = JsonBox::Value(resultObj);
				 
				 FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
				 FBDelegator::getInstance()->removeDeleSel(delekey);
				 if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(_value);
             
             }];
             
         }else{
			 
             CCLog("login fail3");
             JsonBox::Value result;
			 result.loadFromString("{\"error\":1}");
			 FBDelegator::DeleSel delesel =  FBDelegator::getInstance()->getDeleSel(delekey);
			 FBDelegator::getInstance()->removeDeleSel(delekey);
			 if(delesel.target!=0 && delesel.selector!=0)((delesel.target)->*(delesel.selector))(result);
			 
         }
         
     }];
#endif
}

