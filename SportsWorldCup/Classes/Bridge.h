//
//  MultiLanguage.h
//  AngryKeeper
//
//  Created by ksoo k on 12. 9. 20..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#ifndef AngryKeeper_MultiLanguage_h
#define AngryKeeper_MultiLanguage_h
#include "cocos2d.h"
#include <string>
#include "JNIKelper.h"



USING_NS_CC;
using namespace std;



class ML
{
public:
	static string key(const string& k);
};

void showMoreApps();
void moreapps();
void write_review();
void otherApp(string appcode);
extern const char* defaultFont;
#endif
