//
//  ChipSprites.h
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 12. 7..
//
//

#ifndef __BasketWorldCup2__ChipSprites__
#define __BasketWorldCup2__ChipSprites__
#include <vector>
#include <algorithm>
#include "cocos2d.h"
using namespace std;
USING_NS_CC;
class ChipSprites
{
public:
	enum {REMOVING = 99};
	const static unsigned int D_ANGLE = 1;
	const static unsigned int D_POSITION = 2;
	vector<pair<CCNode*, unsigned int>> sprites;
	
	void setPosition(CCPoint pt)
	{
		for(auto iter = sprites.begin(); iter != sprites.end(); ++iter)
		{
			auto _e = *iter;
			{
				if(_e.second & D_POSITION)
				{
					_e.first->setPosition(pt);
				}
			}
		}
	}
	void setRotation(float angle)
	{
		for(auto iter = sprites.begin(); iter != sprites.end(); ++iter)
		{
			auto _e = *iter;
			
			{				
				if(_e.second & D_ANGLE)
				{
					_e.first->setRotation(angle);
				}
			}
		}
	}
	void addCCNodeWithFlag(CCNode* _s, unsigned flag = ChipSprites::D_ANGLE | ChipSprites::D_POSITION)
	{
		pair<CCNode*, unsigned> p(_s, flag);
		sprites.push_back(p);
	}
	void removeAllNode()
	{
		for(auto iter = sprites.begin(); iter != sprites.end(); ++iter)
		{
			auto _e = *iter;
			_e.first->getParent()->removeChild(_e.first, true);
		}
	}
	CCNode* getReader(){
		CCAssert(sprites.size() >= 1, "size >= 1");
		return sprites[0].first;
	}
	CCNode* getObjFromIndex(int i){
		CCAssert(sprites.size() > i, "size...");
		return sprites[i].first;
	}
};

class ChipSpritesFactory
{
private:
	vector<ChipSprites*> pool;
public:
	ChipSprites* create()
	{
		ChipSprites* _chip = new ChipSprites;
		pool.push_back(_chip);
		return _chip;
	}
	void clear()
	{
		for(auto iter = pool.begin(); iter != pool.end(); ++iter)
		{
			ChipSprites* _delete = *iter;
			delete _delete;
		}
	}
};

#endif /* defined(__BasketWorldCup2__ChipSprites__) */
