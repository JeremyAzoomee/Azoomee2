//
//  OnScreenChecker.cpp
//  TinizineCommon
//
//  Created by Macauley on 27/07/2017.
//
//

#include "OnScreenChecker.h"

using namespace cocos2d;


NS_TZ_BEGIN

bool OnScreenChecker::kUseStrictBoundry = false;
	
	OnScreenChecker::~OnScreenChecker()
	{
		stopCheckingOnScreenPosition();
	}
	
    void OnScreenChecker::startCheckingForOnScreenPosition(Node* sender)
    {
        elementOnScreen = false;
        auto scheduler = Director::getInstance()->getScheduler();
        scheduler->schedule([=](float dt)
                            {
                                if(!sender->isVisible())
                                {
                                    return;
                                }
                                bool isVisible = OnScreenChecker::checkIfElementIsOnScreen(sender);
                                
                                if((isVisible)&&(!elementOnScreen))
                                {
                                    elementOnScreen = true;
                                    elementAppeared(sender);
                                }
                                else if((!isVisible)&&(elementOnScreen))
                                {
                                    elementOnScreen = false;
                                    elementDisappeared(sender);
                                }
                            }, this, 0.5f, kRepeatForever, 0.0f, false, "onScreenCheck");
    }
    
    bool OnScreenChecker::checkIfElementIsOnScreen(Node* itemToCheck)
    {
        Size extraBoundary = kUseStrictBoundry ? Size(0,0) : Size(100,800);
        
        Point worldPosition = itemToCheck->getParent()->convertToWorldSpace(itemToCheck->getPosition());
        Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Size itemSize = itemToCheck->getBoundingBox().size;
        
        Point visibleRectStartPoint = Point(visibleOrigin.x - extraBoundary.width - itemSize.width / 2, visibleOrigin.y - extraBoundary.height - itemSize.height / 2);
        Size visibleRectSize = Size(visibleSize.width + 2*extraBoundary.width + itemSize.width, visibleSize.height + 2*extraBoundary.height + itemSize.height);
        
        Rect visibleRect = Rect(visibleRectStartPoint.x, visibleRectStartPoint.y, visibleRectSize.width, visibleRectSize.height);
        
        if(visibleRect.containsPoint(worldPosition))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    void OnScreenChecker::stopCheckingOnScreenPosition()
    {
        Director::getInstance()->getScheduler()->unschedule("onScreenCheck", this);
    }
    
    
    void OnScreenChecker::elementAppeared(Node* sender)
    {
        
    }
    
    void OnScreenChecker::elementDisappeared(Node* sender)
    {
        
    }
NS_TZ_END
