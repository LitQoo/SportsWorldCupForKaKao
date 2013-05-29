//
//  KSoundEngine.h
//  HurdleWorldCup
//
//  Created by ksoo k on 12. 5. 31..
//  Copyright (c) 2012³â __MyCompanyName__. All rights reserved.
//



#pragma once
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include <set>
#include <string>
using namespace std;

class KSoundEngine
{
private:
	bool isEffect;
	bool isBGM;
	/*CCSet* bgmList;
	CCSet* effList;*/
	set<string> bgmList;
	set<string> effList;
	//CCMutableSet* bgmList;
	//CCMutableSet* effList;
public:
	bool getIsEffect(){return isEffect;}
	void setIsEffect(bool b){isEffect = b;}
	bool getIsBGM(){return isBGM;}
	void setIsBGM(bool b){isBGM = b;}
	set<string>& getBgmList(){return bgmList;}
	set<string>& getEffList(){return effList;}

	void registerSoundOnBGM(string name);
	void registerSoundOnEffect(string name);
	void playSound(string name);
	void setBackgroundVolume(float volume);
	void setBackgroundVolumeFrom(float v1,float v2,double t);
	float backgroundVolume();
	float effectVolume();
	void stopBackground();
	static KSoundEngine* sharedEngine()
	{
		static KSoundEngine* _ins = 0;
		if(_ins == 0)
			_ins = new KSoundEngine();
		return _ins;
	}
public:
	virtual ~KSoundEngine()
	{
	}
private:
	KSoundEngine()
	{
		isEffect = isBGM = true;
	}
};