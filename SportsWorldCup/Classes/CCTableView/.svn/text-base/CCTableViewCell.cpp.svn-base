/*
 *  CCTableViewCell.cpp
 *  SkeletonX
 *
 *  Created by Vincent on 11-11-21.
 *  Copyright 2011 GeekStudio. All rights reserved.
 *
 */

#include "CCTableView.h"
#include "CCTableViewCell.h"


CCTableViewCell::CCTableViewCell(const char * mCellIdentifier)
{
	std::string tmpIdentify(mCellIdentifier);
	this->setCellIdentifier(tmpIdentify);
	
	this->setEditable(false);
	
	parentView = NULL;
	
	delBtnColor = ccRED;
	
	CCLabelTTF * labelDelete = CCLabelTTF::labelWithString("Delete", "Arial", 16);
	deleteButton = CCMenuItemLabel::itemWithLabel(labelDelete, this,menu_selector(CCTableViewCell::deleteButtonSelector));
	menuItem = CCMenu::menuWithItem(deleteButton);
	this->addChild(menuItem,20);
	this->showDeleteButton(false);
}

CCTableViewCell::~CCTableViewCell()
{
	this->removeAllChildrenWithCleanup(true);
}

void CCTableViewCell::resetDeleteButtonPosition()
{
	if (deleteButton != NULL) {
		CCPoint tmpPos = ccp(this->getContentSize().width - deleteButton->getContentSize().width/2.0 - 8,
							 this->getContentSize().height/2);
		deleteButton->getParent()->setPosition(tmpPos);
		((CCMenuItemLabel*)deleteButton)->setColor(delBtnColor);
	}
}

void CCTableViewCell::deleteButtonSelector(CCObject * mObject)
{
	if (parentView != NULL) {
		parentView->deleteCCTableViewCell(this);
	}
}

bool CCTableViewCell::isDeleteButtonShow()
{
	return (this->menuItem->getIsVisible());
}

void CCTableViewCell::showDeleteButton(bool mShow)
{
	this->menuItem->setIsVisible(mShow);
	this->deleteButton->setIsVisible(mShow);
}

/*
void CCTableViewCell::onEnter()
{
	CCLay::onEnter();
}

void CCTableViewCell::onExit()
{
	CCLayerColor::onExit();
}
*/