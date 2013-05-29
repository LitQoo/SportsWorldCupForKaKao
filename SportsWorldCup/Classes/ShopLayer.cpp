//
//  ShopLayer.cpp
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 12. 21..
//
//

#include "ShopLayer.h"
#include "NSDefault.h"
#include "KS_Util.h"

void ShopLayer::initFnt()
{
	rechargeRemainTimeFnt = CCLabelBMFont::labelWithString("", "mainfont2.fnt");
	remainHeartFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", NSDefault::getHeartNumber()).c_str(), "mainfont2.fnt");
	coinFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", NSDefault::getGold()).c_str(), "mainfont1.fnt");
	rubyFnt = CCLabelBMFont::labelWithString(KS_Util::stringWithFormat("%d", NSDefault::getRuby()).c_str(), "mainfont1.fnt");
	
	coinFnt->setAnchorPoint(ccp(1, 0.5));
	rubyFnt->setAnchorPoint(ccp(1, 0.5));
	
	rechargeRemainTimeFnt->setPosition(ccp(379, 120));
	remainHeartFnt->setPosition(ccp(435, 120));
	coinFnt->setPosition(ccp(454, 185));
	rubyFnt->setPosition(ccp(454, 225));
	
	addChild(rechargeRemainTimeFnt);
	addChild(remainHeartFnt);
	addChild(coinFnt);
	addChild(rubyFnt);
}

void ShopLayer::initLoadingSprite()
{
	auto animator = SceneUtil::playAnimation("loading.png", 0.1, 9, 9, 56, 56, true);
	animator.second->runAction(animator.first);
	addChild(animator.second, INT_MAX);
	animator.second->setPosition(ccp(240, 160));
	loadingSprite = animator.second;
	loadingSprite->setIsVisible(false);
}
void ShopLayer::notifiedTime(string t)
{
	rechargeRemainTimeFnt->setString(t.c_str());
}
void ShopLayer::notifiedHeart(int h)
{
	for(auto iter = hearts.begin(); iter != hearts.end(); ++iter)
	{
		(*iter)->getParent()->removeChild(*iter, true);
	}
	hearts.clear();
	
	for(int i=0; i<h; i++)
	{
		if(hearts.size() < 5)
			addHeartSprite();
		else
			break;
	}
	
	remainHeartFnt->setString(KS_Util::stringWithFormat("%d", h).c_str());
}

void ShopLayer::addHeartSprite()
{
	CCPoint coord [] = {CCPoint(354, 147), CCPoint(380, 147), CCPoint(405, 147), CCPoint(429, 147), CCPoint(454, 147)};
	CCAssert(hearts.size() < 5, "...");
	CCPoint heartPos = coord[hearts.size()];
	CCSprite* h = CCSprite::spriteWithFile("heart.png");
	addChild(h, 1);
	h->setPosition(heartPos);
	hearts.push_back(h);
}

void ShopLayer::initHeartSprite()
{
	// 356, 132
	// 380, "
	// 405,
	// 429,
	// 454,
	CCPoint coord [] = {CCPoint(354, 147), CCPoint(380, 147), CCPoint(405, 147), CCPoint(429, 147), CCPoint(454, 147)};
	int fillNumber = MIN(NSDefault::getHeartNumber(), GameSystem::DEFAULT_MAX_HEART);
	for(int i=0; i<fillNumber; i++)
	{
		CCPoint heartPos = coord[i];
		CCSprite* h = CCSprite::spriteWithFile("heart.png");
		addChild(h, 1);
		h->setPosition(heartPos);
		hearts.push_back(h);
	}
	
	//CCPoint t = {356, 132}
	
	
}