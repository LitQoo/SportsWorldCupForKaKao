#include "KSModalAlert.h"
#include "PriorityMenu.h"

int KSCoverLayer::touchPriority = INT_MIN + 1000;
void KSModalAlert(	char const* title,	char const *message, int nargs, ...)
{
	CCScene* runningScene = CCDirector::sharedDirector()->getRunningScene();
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize(); //CCEGLView::sharedOpenGLView().getSize();
	
	KSCoverLayer* coverLayer = KSCoverLayer::node();
	
	int zOrder = INT_MAX;
	coverLayer->setAnchorPoint(ccp(0.5, 0.5));
	coverLayer->setPosition(ccp(240, 160));
	runningScene->addChild(coverLayer, zOrder);

	float fontSize = 20;
	int fontWidth = 240;

	CCLabelTTF* titleText = CCLabelTTF::labelWithString(title,  defaultFont, fontSize);

	titleText->setScale(1);
	titleText->setColor(ccc3(255,255,255));
	CCSize dimension = CCSizeMake(300, 0);
	CCLabelTTF* contentText = CCLabelTTF::labelWithString(message, dimension, CCTextAlignmentCenter, defaultFont, fontSize);
	contentText->setColor(ccc3(255, 255, 255));

	
	float btnHeight;
	{ // get btnHeight;
		CCSprite* button1 = new CCSprite();
		button1->init();
		button1->initWithFile("dialogbutton.png");
		btnHeight = button1->boundingBox().size.height;
		button1->release();
	}

	int margin = 30;

	CCSprite* newWindow = CCSprite::spriteWithFile("alert.png");
//	newWindow->init();
//	newWindow->setTextureRect(CCRectMake(0, 0, contentText->boundingBox().size.width + margin*2,
//		margin + titleText->boundingBox().size.height + margin + contentText->boundingBox().size.height + 
//		margin + btnHeight + margin));
//	newWindow->setColor(ccc3(255, 255, 255));

//	CCSprite* backNewWindow = new CCSprite();
//	backNewWindow->init();
//	int borderSize = 4;
//	backNewWindow->setTextureRect(CCRectMake(0, 0, contentText->boundingBox().size.width + margin*2 + borderSize, 
//		margin + titleText->boundingBox().size.height + margin + contentText->boundingBox().size.height + 
//		margin + btnHeight + margin + borderSize));
//	backNewWindow->setColor(ccc3(0, 0, 0));

//	coverLayer->addChild(backNewWindow, INT_MAX);
//	backNewWindow->release();
//	backNewWindow->setPosition(ccp(screenSize.width / 2, screenSize.height / 2));
	coverLayer->addChild(newWindow, zOrder);

//	newWindow->release();
	
	newWindow->setPosition(ccp(screenSize.width / 2, screenSize.height / 2));
	newWindow->addChild(titleText);
	newWindow->addChild(contentText);

	titleText->setAnchorPoint(ccp(0.5, 1));
	contentText->setAnchorPoint(ccp(0.5, 1));

	titleText->setPosition(ccp(newWindow->boundingBox().size.width / 2., newWindow->boundingBox().size.height - margin));
	contentText->setPosition(ccp(newWindow->boundingBox().size.width / 2., 155.f));
	//instead of 155.f, newWindow->boundingBox().size.height - titleText->boundingBox().size.height - margin*2));
	
	KS::KSLog("button P = %", KSCoverLayer::touchPriority - 1);
	PriorityMenu* menus = PriorityMenu::menuWithItems( (KSCoverLayer::touchPriority - 1), 0);
	//CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(menus, -301, true);
	menus->setAnchorPoint(ccp(0.5, 0));
	menus->setPosition(ccp( CCRect::CCRectGetMidX(newWindow->getTextureRect()),
		CCRect::CCRectGetMinY(newWindow->getTextureRect()) + margin ));

	CCMutableArray<CCMenuItem*>* menuItems = new CCMutableArray<CCMenuItem*>();
	///////////////////
	
	va_list argptr; 
	va_start(argptr, nargs);        /* 가변 인자 처리 시작 */
	for (int i = 0; i < nargs; i++) // menuItems 에 아이템을 추가
	{
		char const* opt = va_arg(argptr, char const*);
		CCObject* target = va_arg(argptr, CCObject*);
		SEL_CallFunc selector = va_arg(argptr, SEL_CallFunc);
		// nargs 는 버튼의 개수.
		CCSprite* button1 = CCSprite::spriteWithFile("dialogbutton.png");
		CCSprite* button2 = CCSprite::spriteWithFile("dialogbutton.png");

		{
			CCLabelTTF* buttonLabel = CCLabelTTF::labelWithString(opt, defaultFont, fontSize);
			//buttonLabel->setScale(1);

			buttonLabel->setPosition(ccp( CCRect::CCRectGetMidX(button1->getTextureRect()),
				CCRect::CCRectGetMidY(button1->getTextureRect()) ));
			buttonLabel->setColor(ccc3(255, 255, 255));
			button1->addChild(buttonLabel, zOrder);
		}
		{
			CCLabelTTF* buttonLabel = CCLabelTTF::labelWithString(opt, defaultFont, fontSize);
			buttonLabel->setColor(ccc3(255, 255, 255));
			//buttonLabel->setScale(1);
			buttonLabel->setPosition(ccp( CCRect::CCRectGetMidX(button2->getTextureRect()),
				CCRect::CCRectGetMidY(button2->getTextureRect()) ));
			button2->addChild(buttonLabel, zOrder);
		}

		KSCallBack* kcb = KSCallBack::callBackWithOption(coverLayer, target, selector);
		CCMenuItemSprite* menu = CCMenuItemSprite::itemFromNormalSprite(button1, button2, kcb, menu_selector(KSCallBack::Execute));
		menu->setScale(1);
		menuItems->addObject(menu);	
	}
	////////////////////////////////////////////////////////////////////////////////////////
	for(auto iter = menuItems->begin(); iter != menuItems->end(); ++iter)
	{
		menus->addChild(*iter, 1);
	}

	menus->alignItemsHorizontally();
	newWindow->addChild(menus, zOrder);
	//coverLayer->addChild(menus, INT_MAX);

	menuItems->release();
	//menus->release();
	va_end(argptr);                 /* 가변 인자 처리 끝 */


	/////////////////////////////////////

}



void KSCallBack::Execute(CCObject *menu_item)
{
	//CCLog("Excute");
	CC_UNUSED_PARAM(menu_item);
	
	if(selectorTarget && selector)
		(selectorTarget->*selector)();
	CCNode* par = this->getParent();//->getParent(); // cover ?? ???? scene ?? a??
	par->removeFromParentAndCleanup(true);
}
















