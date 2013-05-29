#include "KS_Util.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	#import <UIKit/UIKit.h>
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	#include "android/jni/JniHelper.h"
	#include "jni.h"
	#include "android/jni/SystemInfoJni.h"
#endif
namespace KS
{
	CCSprite* spriteWithSpriteFrameName(const char *pszSpriteFrameName)
	{
		CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
		
		if(pFrame)
			return CCSprite::spriteWithSpriteFrame(pFrame);
		else
			return 0;
	}
	
	string getLocalCode()
	{
		const char* tempCode;
		
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
		NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
		NSString *currentLanguage = [languages objectAtIndex:0];
		
		// get the current language code.(such as English is "en", Chinese is "zh" and so on)
		NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:currentLanguage];
		NSString * languageCode = [temp objectForKey:NSLocaleLanguageCode];
		
		tempCode = [languageCode cStringUsingEncoding:NSASCIIStringEncoding];
		
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		tempCode = getCurrentLanguageJNI();
		
#endif
		return tempCode;
	}
	
	void setAllVisible(CCNode* n, bool b)
	{
		if(n)
		{
			n->setIsVisible(b);
			CCArray* ch = n->getChildren();
			if(ch)
			{
				for(int i=0; i<ch->count(); i++)
				{
					CCNode* chn = (CCNode*)ch->objectAtIndex(i);
					setAllVisible(chn, b);
				}
			}
			
		}
		else
		{
			
		}
	}
}

