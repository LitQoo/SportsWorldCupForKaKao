

#include "HWHurdleMapLayer.h"
#include "HWSharingClass.h"
#include "NSDefault.h"
#include "KS_Util.h"
#include "HWPlay.h"

USING_NS_CC;





//////////////////////////////////////////////////

int HurdleMapLayer::BEGIN_HEIGHT()
{
	return 120;
	//return BEGIN_HEIGHT;
}


int HurdleMapLayer::getMinHurdleMargin()
{
	return 230 + (hwplay->speedCombo  ) * 7 ;
}

int HurdleMapLayer::getNormalHurdleMargin()
{
	return 210 + (hwplay->speedCombo  ) *7;
}
int HurdleMapLayer::getCrazyTrackMargin(float diff)
{
	if(diff > -10.f)
	{
		KS::KSLog("% % %", __FUNCTION__, __LINE__, diff);
		return /*65*/65 + 8 * hwplay->speedCombo;
	}
	else if(diff > -50.f)
	{
		if(rand()%2)
		{
			KS::KSLog("% % %", __FUNCTION__, __LINE__, diff);
			return 40 + 5 * hwplay->speedCombo;
		}
		else
		{
			KS::KSLog("% % %", __FUNCTION__, __LINE__, diff);
			return 80 + 12 * hwplay->speedCombo;
		}
	}
	else
	{
		KS::KSLog("% % %", __FUNCTION__, __LINE__, diff);
		return /*65*/120 + 10 * hwplay->speedCombo;
	}
}
int HurdleMapLayer::getTrackMargin()
{
	return 55 + 8 * hwplay->speedCombo;
}
void HurdleMapLayer::majorIsHurdle(CCMutableArray<Obstacle*>* newObject)
{
//	int dynamic = rand()%3 + 2;
//	for(int _i=1; _i<=dynamic; _i++)
//	{
		Obstacle* obs = new Obstacle();
		obs->autorelease();
		obs->type = this->obsTypeDeterminer();
		obs->setHeight(max(MINHEIGHT, prevHeight + ((rand()%2)?1:-1) * rand()%MAXBELOWHEIGHTDIFF));
		//obs->setHeight(max(MINHEIGHT, prevHeight + 60));

		// 처음 생성하는 맵이라면
		if(startCreationPosition == 0)
		{
			obs->setLength(STARTLENGTH);
			obs->setHeight(this->BEGIN_HEIGHT());
		}
		else if(reviveEvent)
		{
			obs->setLength(STARTLENGTH);
			obs->setHeight(prevHeight);
			revivePosition = startCreationPosition;
		}
		else
		{
			obs->setLength(rand()%LENGTHRANGE + MINLENGTH); // #
		}
		prevHeight = obs->height; // ø©±‚ø©æﬂ «—¥Ÿ.
		obs->setStart(startCreationPosition);

		if(startCreationPosition != 0) // ¥Ÿ∏• ∫Œ¿ß
		{
			this->addHurdles(obs);
		}
		startCreationPosition += obs->length * patternWidth + (rand()%MARGINRANGE + getTrackMargin());
		newObject->addObject(obs);	

//		if(startCreationPosition >= 480 * 3 * (screenCount+1))
//			break;
//	}
}

void HurdleMapLayer::majorIsTrack(CCMutableArray<Obstacle*>* newObject)
{	
	//1 ->
	int MINLENGTH_2;
	if(hwplay->combo.getVar() >= 20 ) // Ω∫≈◊¿Ã¡ˆ∞° ø√∂Û∞°∏È ø√∂Û∞• ºˆ∑œ ¿Ê¿∫ ∫Ûµµ∑Œ if ∞…∏≤
		MINLENGTH_2 = 1;
	else {
		if(rand()%2)
			MINLENGTH_2 = 2;
		else
			MINLENGTH_2 = 1;
	}
	int dynamic = rand()%2 + 1;
	for(int _i=1; _i<=dynamic; _i++){
		Obstacle* obs = new Obstacle();
		obs->autorelease();
		obs->type = this->obsTypeDeterminer();
		int tempPrevHeight = prevHeight;

		obs->setHeight(max(MINHEIGHT, prevHeight + ((rand()%2)?1:-1) * (rand()%MAXBELOWHEIGHTDIFF_2 +30) )); 
		//obs->setHeight(max(MINHEIGHT, prevHeight + 60 ));
		if(startCreationPosition == 0){
			obs->setLength(STARTLENGTH);
			obs->setHeight(this->BEGIN_HEIGHT());
		}
		else if(reviveEvent){
			obs->setLength(STARTLENGTH);
			obs->setHeight(prevHeight);
			revivePosition = startCreationPosition;
		}
		else {
			if(prevHeight - obs->height > 80) // ≥ª∑¡ø¿¥¬∞‘ 80 ¿Ã∏È.
				obs->setLength(MINLENGTH);
			else 
				obs->setLength(rand()%LENGTHRANGE_2 + MINLENGTH_2);
			
		}
		prevHeight = obs->height;
		if(obs->height - prevHeight < 35)
			obs->setStart(startCreationPosition);
		else {
			obs->setStart(prevCreationPosition + 350.f);
		}
		prevCreationPosition = startCreationPosition + obs->length*patternWidth;
		startCreationPosition += obs->length * patternWidth + (rand()%MARGINRANGE_2 + getCrazyTrackMargin(tempPrevHeight - obs->height));
		if(startCreationPosition != 0) {
			this->addHurdles(obs);
		}
		newObject->addObject(obs);	
//		if(startCreationPosition >= 480 * 3 * (screenCount+1))
//			break;
	}
}


void HurdleMapLayer::onEnterTransitionDidFinish()
{
	CCLayer::onEnterTransitionDidFinish();
	this->setIsTouchEnabled(true);
	setBalance(0);

	//Obstacles = NSMutableArray::alloc()->init();
	Obstacles = new CCMutableArray<Obstacle*>();

	patternWidth = 100;
	prevHeight = 200;
	startCreationPosition = 0;
	this->cachingPList();
	this->createScreen();
}

HurdleMapLayer::~HurdleMapLayer()
{
	Obstacles->release();
//	CCArray* children = this->getChildren();
//
//	for(int i=0; i<children->count(); i++)
//	{
//		CCObject* tempNode = children->objectAtIndex(i);
//		if(tempNode != nil && 1<tempNode->retainCount())
//		{
//			while(1<tempNode->retainCount())
//				tempNode->release();
//		}
//	}
//
//	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
//	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//	CCTextureCache::sharedTextureCache()->removeAllTextures();
}

int HurdleMapLayer::getFirstAttitude()
{
	Obstacle* t = Obstacles->getObjectAtIndex(0);
	if(t == nil)
		return this->BEGIN_HEIGHT();
	return t->height;
}

bool HurdleMapLayer::createObstacle(float scale)
{
	float movedis = this->getPosition().x;
	//CCLog("screen position : %f",movedis);
	movedis = fabsf(movedis); // øÚ¡˜¿Œ ∞≈∏Æ
	if(movedis > startCreationPosition - 500.f/scale)
	{
		KS::KSLog("CREATE MAP!!");
		this->createScreen();
		return true;
	}
	return false;
}


void HurdleMapLayer::removeSprite()
{
	float moveDis = fabsf(this->getPosition().x);
	
	for(int i=0; i<Obstacles->count(); i++)
	{
		Obstacle* ob = Obstacles->getObjectAtIndex(i);
		CCMutableArray<Hurdle*>* removes = new CCMutableArray<Hurdle*>();
		removes->autorelease();
		
		for(auto iter = ob->getHurdles()->begin(); iter != ob->getHurdles()->end(); ++iter)
		{
			Hurdle* h = *iter;
			if(h->getHurdleSprite())
			{
				if(h->getHurdleSprite()->getPosition().x < moveDis - 100)
				{
					h->getHurdleSprite()->removeFromParentAndCleanup(true);
//					batch->removeChild(h->getHurdleSprite(), true);
					removes->addObject(h);
					//h->getHurdleSprite()->removeFromParentAndCleanup(true);
				}
				
			}
		}
		for(auto iter = removes->begin(); iter != removes->end(); ++iter)
		{
			ob->getHurdles()->removeObject(*iter);
		}
		//ob->getHurdles()->removeObjectsInArray(removes);
	}
	
	
	CCMutableArray<Obstacle*>* removes = new CCMutableArray<Obstacle*>();
	removes->autorelease();
	for(auto iter = Obstacles->begin(); iter != Obstacles->end(); ++iter)
	{
		Obstacle* ob = *iter;
		if(ob->start + ob->getLength() * getPatternWidth() < moveDis)
		{
			removes->addObject(*iter);
			for(auto spIter = ob->sprites.begin(); spIter != ob->sprites.end(); ++spIter)
			{
				(*spIter)->removeFromParentAndCleanup(true);
			}
		}
	}
	Obstacles->removeObjectsInArray(removes);
}

void HurdleMapLayer::setBalance(int combo)
{
	MINLENGTH = 12;// = 9;	   // 트랙최소길이
	LENGTHRANGE = 35; // 트랙길이의 유동성
	MINHEIGHT = 20;  // 트랙의 최소 높이


	// 망나니 모드 변수
	LENGTHRANGE_2 = 3; // 트랙길이의 유동성
	MARGINRANGE = MIN(80 + hwplay->speedCombo * 10, 130);
	MARGINRANGE_2 = MIN(80 + hwplay->speedCombo * 3, 100);
	MAXBELOWHEIGHTDIFF = MIN(25 + hwplay->speedCombo * 8, 80);
	MAXBELOWHEIGHTDIFF_2 = MIN(10 + hwplay->speedCombo * 2, 40);
}
void HurdleMapLayer::setPosition(CCPoint position)
{
	prevPosition = this->getPosition();
	CCLayer::setPosition(position);
}