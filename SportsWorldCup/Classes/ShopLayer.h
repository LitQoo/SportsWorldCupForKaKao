//
//  ShopLayer.h
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 12. 21..
//
//

#ifndef __BasketWorldCup2__ShopLayer__
#define __BasketWorldCup2__ShopLayer__

#include "cocos2d.h"
#include "KSBaseScene.h"
USING_NS_CC;

class ShopLayer : public CCLayer
{
public:
	virtual void notifiedTime(string t);
	virtual void notifiedHeart(int h);
	virtual void addHeartSprite();
	virtual void initHeartSprite();
	CCSprite* loadingSprite;
	LAYER_NODE_FUNC(ShopLayer);
	virtual bool init()
	{
		return CCLayer::init();
	}
protected:
	void deleteSprite(CCNode* _n)
	{
		_n->getParent()->removeChild(_n, true);
	}
	CCLabelBMFont* rechargeRemainTimeFnt;
	CCLabelBMFont* remainHeartFnt;
	CCLabelBMFont* coinFnt;
	CCLabelBMFont* rubyFnt;
	vector<CCSprite*> hearts;
	void initFnt();
	void initLoadingSprite();
	

};

#endif /* defined(__BasketWorldCup2__ShopLayer__) */
