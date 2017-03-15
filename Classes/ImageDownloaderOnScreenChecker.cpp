#include "ImageDownloaderOnScreenChecker.h"
#include "HQSceneElementVisual.h"

using namespace cocos2d;

void ImageDownloaderOnScreenChecker::startCheckingForOnScreenPosition(Node* sender)
{
    elementOnScreen = !ImageDownloaderOnScreenChecker::checkIfElementIsOnScreen(sender);
    
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule([=](float dt)
                        {
                            bool isVisible = ImageDownloaderOnScreenChecker::checkIfElementIsOnScreen(sender);
                            
                            if((isVisible)&&(!elementOnScreen))
                            {
                                elementOnScreen = true;
                                ImageDownloaderOnScreenChecker::elementAppeared(sender);
                            }
                            else if((!isVisible)&&(elementOnScreen))
                            {
                                elementOnScreen = false;
                                ImageDownloaderOnScreenChecker::elementDisappeared(sender);
                            }
                        }, this, 0.5f, kRepeatForever, 0.0f, false, "myCallbackKey");
}

bool ImageDownloaderOnScreenChecker::checkIfElementIsOnScreen(Node* itemToCheck)
{
    Size extraBoundary = Size(-200,-200);
    
    Point directPosition = itemToCheck->getPosition();
    Point worldPosition = itemToCheck->getParent()->convertToWorldSpace(itemToCheck->getPosition());
    Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Size itemSize = itemToCheck->getBoundingBox().size;
    
    Point visibleRectStartPoint = Point(visibleOrigin.x - extraBoundary.width - itemSize.width, visibleOrigin.y - extraBoundary.height - itemSize.height);
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

//------------------------------------All methods are private below this line------------------

void ImageDownloaderOnScreenChecker::elementAppeared(Node* sender)
{
    CCLOG("element appeared!");
    HQSceneElementVisual* owner = (HQSceneElementVisual*)sender;
    owner->startLoadingImage();
}

void ImageDownloaderOnScreenChecker::elementDisappeared(Node* sender)
{
    CCLOG("element disappeared!");
    HQSceneElementVisual* owner = (HQSceneElementVisual*)sender;
    owner->removeLoadedImage();
}
