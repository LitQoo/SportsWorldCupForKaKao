//
//  KSoundEngine.m
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 5. 31..
//  Copyright (c) 2012³â __MyCompanyName__. All rights reserved.
//

#include "KSoundEngine.h"

using namespace CocosDenshion;
USING_NS_CC;
class Fade
{
public:
	float v1,v2;
	double duration;
};



void KSoundEngine::registerSoundOnBGM(string name)
{
	
	KSoundEngine* obj = KSoundEngine::sharedEngine();
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(name.c_str());
	
	obj->getBgmList().insert(name);	
}

void KSoundEngine::registerSoundOnEffect(string name)
{
	KSoundEngine* obj = KSoundEngine::sharedEngine();
	SimpleAudioEngine::sharedEngine()->preloadEffect(name.c_str());
	obj->getEffList().insert(name);
}

void KSoundEngine::playSound(string name)
{
	KSoundEngine* obj = KSoundEngine::sharedEngine();
	
	if(obj->getIsEffect() && obj->getEffList().find(name) != obj->getEffList().end()) 
	{
		SimpleAudioEngine::sharedEngine()->playEffect(name.c_str());
	}
	
	if(obj->getIsBGM() && obj->getBgmList().find(name) != obj->getBgmList().end())
	{
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(name.c_str(), true);
	}
}
float KSoundEngine::backgroundVolume()
{
	return SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume();
}

float KSoundEngine::effectVolume()
{
	return SimpleAudioEngine::sharedEngine()->getEffectsVolume();
}

void KSoundEngine::stopBackground()
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}
void KSoundEngine::setBackgroundVolume(float volume)
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume);
}

void KSoundEngine::setBackgroundVolumeFrom(float v1, float v2, double t)
{
	// # 
	/*Fade* f = Fade::alloc()->init()->autorelease();
	f->v1 = v1;
	f->v2 = v2;
	f->duration = t;
	CCThread::detachNewThreadSelector(selector(_fadeTh), this, f);*/
}

//void KSoundEngine::_fadeTh(Fade* f) // #
//{
//	double prevFrameTime = CFAbsoluteTimeGetCurrent();
//	
//	float A = f->v1;
//	float B = f->v2;
//	double dx = fabs( (A-B)/f->duration );
//	int flag = 1;
//	if(A>B)
//	{
//		A *= -1;
//		B *= -1;
//		flag = -1;
//	}
//	while(1) // A<=B
//	{
//		//CCLog("A : %f,B : %f",A,B);
//
//		double t = CFAbsoluteTimeGetCurrent();
//		//CCLog("%lf - %lf",t,currentFrameTime);
//		if(t - prevFrameTime >= 1/60.)
//		{
//			double dt = t - prevFrameTime;
//			prevFrameTime = t - (t - prevFrameTime - 1/60.);
//			//////////////////////////////////////////////////
//			A += dx * dt;
//			if(A >= B)
//			{
//				A = B;
//				SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(A*flag);
//				break;
//			}
//			SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(A*flag);
////			CCLog("vol : %f",A*flag);
//		}
//	}
//	
//	//CCLog("A : %f,B : %f -- final",A*flag,B*flag);
//}
