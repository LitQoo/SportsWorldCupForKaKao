//
//  Ring.cpp
//  BasketWorldCup2
//
//  Created by ksoo k on 12. 10. 31..
//
//

#include "BS2Ring.h"

const float Ring::BACKBOARD_E = 0.75f;
void Ring::init(float backBoardLength, float groundLength, float ringSize, float e, float u)
{
	/*
	 
	 cpBody* backBoardBody;
	 cpShape* backBoardShape;
	 
	 cpShape* groundShape;
	 cpShape* ring1Shape;
	 cpShape* ring2Shape;*/
	int easy = -3; // 난이도 하락시키는 직선높이 차이
	this->ringSize = ringSize;
	backBoardBody = cpBodyNew(INFINITY, INFINITY);
	setPosition(cpv(57, 250));
	//backBoardBody->p = cpv(60, 250); // 50
	//backBoardBody->p = cpv(60, 100);
	backBoardShape = cpSegmentShapeNew(backBoardBody, cpv(0,0), cpv(0, backBoardLength), 1.f);
//	CCLog("backboard shape %x", backBoardShape);
	backBoardShape->e = BACKBOARD_E;
	backBoardShape->u = u;
	
	groundShape = cpSegmentShapeNew(backBoardBody, cpv(0,0), cpv(groundLength - 2.f, easy), 1.f);
//	CCLog("ground shape %x", groundShape);
	groundShape->e = e / 3.f;
	groundShape->u = u;
	
	subObject = cpSegmentShapeNew(backBoardBody, cpv(groundLength -2.f, easy), cpv(groundLength - 2.f, -10.f), 1.f); // 길다란 보조 장치, 자연스러운 공경로를 위해
//	CCLog("subobject shape %x", subObject);
	subObject->e = e;
	subObject->u = u;
	
	ring1Shape = cpCircleShapeNew(backBoardBody, 1.f, cpv(groundLength, easy  ));
//	CCLog("ring1 shape %x", ring1Shape);
	ring1Shape->e = e / 3.f;
	ring1Shape->u = u;
	ring2Shape = cpCircleShapeNew(backBoardBody, 1.f, cpv(groundLength + ringSize, 0.f));
//	CCLog("ring2 shape %x", ring2Shape);
	ring2Shape->e = e;
	ring2Shape->u = u;
	
	ring3Shape = cpCircleShapeNew(backBoardBody, 1.f, cpv((2*groundLength + ringSize) / 2.f, 0.f));
//	CCLog("ring3 shape %x", ring2Shape);
	ring3Shape->e = e;
	ring3Shape->u = u;
}

void Ring::attachMe(cpSpace* space)
{
//	cpSpaceAddStaticShape(space, backBoardShape);
//	cpSpaceAddStaticShape(space, groundShape);
//	cpSpaceAddStaticShape(space, ring1Shape);
//	cpSpaceAddStaticShape(space, ring2Shape);
//	cpSpaceAddStaticShape(space, ring3Shape);
	cpSpaceAddShape(space, backBoardShape);
	cpSpaceAddShape(space, groundShape);
	cpSpaceAddShape(space, ring1Shape);
	cpSpaceAddShape(space, ring2Shape);
	cpSpaceAddShape(space, ring3Shape);
	cpSpaceAddShape(space, subObject);
}