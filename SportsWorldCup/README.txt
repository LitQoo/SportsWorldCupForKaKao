CCScene::CCScene()
{
	m_bIsRelativeAnchorPoint = false;
  setPosition(ccp((CCEGLView::sharedOpenGLView().getSize().width - 480.f) / 2.f, getPosition().y));
	setAnchorPoint(ccp(0.5f, 0.5f));
}

// by ksoo


메인들어올 때
Cocos2d: action seq : getflagranks
Cocos2d: action seq : getweek
action seq : getflagranks

게임 시작시.
action seq : startscores

게임 끝날 때 

action seq : getweek


게임오버 보여줄 때
action seq : getscores
Cocos2d: action seq : getflagranks
action seq : getweek