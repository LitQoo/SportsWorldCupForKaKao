#pragma once

#include "cocos2d.h"






template<typename T, typename P = void>
class LoadingScene : public cocos2d::CCLayer
{
private:
	virtual void onEnterTransitionDidFinish()
	{
		cocos2d::CCLayer::onEnterTransitionDidFinish();
		cocos2d::CCDirector::sharedDirector()->replaceScene(T::scene(param));
	}
public:
	P param;
	static cocos2d::CCScene* scene(P pr)
	{
		cocos2d::CCScene * scene = cocos2d::CCScene::node();
		LoadingScene* layer = LoadingScene::node();
		layer->param = pr;
		scene->addChild(layer);
		return scene;
	}
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(LoadingScene);
};

template<typename T>
class LoadingScene<T, void> : public cocos2d::CCLayer
{
private:
	virtual void onEnterTransitionDidFinish()
	{
		cocos2d::CCLayer::onEnterTransitionDidFinish();
		cocos2d::CCDirector::sharedDirector()->replaceScene(T::scene());
	}
public:
	static cocos2d::CCScene* scene()
	{
		cocos2d::CCScene * scene = cocos2d::CCScene::node();
		LoadingScene* layer = LoadingScene::node();
		scene->addChild(layer);
		return scene;
	}
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(LoadingScene);
};

#define ReplaceScene(X) (cocos2d::CCDirector::sharedDirector()->replaceScene(LoadingScene<X>::scene()))
#define PushScene(X) (cocos2d::CCDirector::sharedDirector()->pushScene(LoadingScene<X>::scene()))
#define ReplaceSceneParam(X, P) (cocos2d::CCDirector::sharedDirector()->replaceScene(LoadingScene<X, decltype(P)>::scene(P)))
#define PushSceneParam(X, P) (cocos2d::CCDirector::sharedDirector()->pushScene(LoadingScene<X, decltype(P)>::scene(P)))



