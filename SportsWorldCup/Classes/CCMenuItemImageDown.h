// CCMenuItemImageDown.h
// hsnks100@gmail.com Litqoo.한경수

#ifndef __SportsWorldCup__CCMenuItemDown__
#define __SportsWorldCup__CCMenuItemDown__

#include "cocos2d.h"
USING_NS_CC;


class CC_DLL CCMenuItemImageDown : public CCMenuItemSprite
{
private:
	virtual void selected()
	{
		CCMenuItem::selected();
		runAction(CCScaleTo::actionWithDuration(0.1f, 0.90f));
	}
	virtual void unselected()
	{
		CCMenuItem::unselected();
		runAction(CCScaleTo::actionWithDuration(0.1f, 1.f));
	}
public:
	
	CCMenuItemImageDown(){}
	virtual ~CCMenuItemImageDown(){}
	/** creates a menu item with a normal and selected image*/
	static CCMenuItemImageDown* itemFromNormalImage(const char *normalImage, const char *selectedImage)
	{
		return CCMenuItemImageDown::itemFromNormalImage(normalImage, selectedImage, NULL, NULL, NULL);
	}
	/** creates a menu item with a normal,selected  and disabled image*/
	static CCMenuItemImageDown* itemFromNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage)
	{
		CCMenuItemImageDown *pRet = new CCMenuItemImageDown();
		if (pRet && pRet->initFromNormalImage(normalImage, selectedImage, disabledImage, NULL, NULL))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
	/** creates a menu item with a normal and selected image with target/selector */
	static CCMenuItemImageDown* itemFromNormalImage(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector)
	{
		return CCMenuItemImageDown::itemFromNormalImage(normalImage, selectedImage, NULL, target, selector);
	}
	/** creates a menu item with a normal,selected  and disabled image with target/selector */
	static CCMenuItemImageDown* itemFromNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject* target, SEL_MenuHandler selector)
	{
		CCMenuItemImageDown *pRet = new CCMenuItemImageDown();
		if (pRet && pRet->initFromNormalImage(normalImage, selectedImage, disabledImage, target, selector))
		{
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
	/** initializes a menu item with a normal, selected  and disabled image with target/selector */
	bool initFromNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject* target, SEL_MenuHandler selector)
	{
		CCNode *normalSprite = CCSprite::spriteWithFile(normalImage);
		CCNode *selectedSprite = NULL;
		CCNode *disabledSprite = NULL;
		
		if (selectedImage)
		{
			selectedSprite = CCSprite::spriteWithFile(selectedImage);
		}
		
		if(disabledImage)
		{
			disabledSprite = CCSprite::spriteWithFile(disabledImage);
		}
		return initFromNormalSprite(normalSprite, selectedSprite, disabledSprite, target, selector);
	}
};


#endif /* defined(__SportsWorldCup__CCMenuItemDown__) */
