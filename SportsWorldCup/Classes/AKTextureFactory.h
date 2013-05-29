//
//  BirdFactory.h
//  AngryKeeper
//
//  Created by ksoo k on 12. 10. 17..
//
//

#ifndef __AngryKeeper__BirdFactory__
#define __AngryKeeper__BirdFactory__

#include "AppDelegate.h"
#include "cocos2d.h"

#include "KSProtectVar.h"
#include "KS_Util.h"

#include <string>
using namespace std;
USING_NS_CC;

class TextureFactory
{
private:
	map<string, CCTexture2D*> textures;
public:
	CCTexture2D* getTexture(string filename)
	{
		if(textures.find(filename) == textures.end())
		{
			
			CCTexture2D* birdTexture = new CCTexture2D();
			
			CCImage* birdImage = new CCImage();
			birdImage->initWithImageFile(filename.c_str());
			birdTexture->initWithImage(birdImage);
			delete birdImage;
			textures[filename] = birdTexture;
			return textures[filename];
		}
		else
			return textures[filename];
	}
	static TextureFactory* sharedObject()
	{
		static TextureFactory* _ins = 0;
		if(_ins == 0)
		{
			_ins = new TextureFactory();
		}
		return _ins;
	}
	
	void reloadTexture()
	{
		for(auto iter = textures.begin(); iter != textures.end(); ++iter)
		{
			string k = iter->first;
			delete textures[k];
			
			CCTexture2D* birdTexture = new CCTexture2D();
			
			CCImage* birdImage = new CCImage();
			
			birdImage->initWithImageFile(k.c_str());
			birdTexture->initWithImage(birdImage);
			delete birdImage;
			
			textures[k] = birdTexture;
		}
	}
private:
	TextureFactory()
	{

	}	
};

#endif /* defined(__AngryKeeper__BirdFactory__) */
