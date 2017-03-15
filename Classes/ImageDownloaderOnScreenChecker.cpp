#include "ImageDownloaderOnScreenChecker.h"
#include "ImageDownloader.h"

using namespace cocos2d;

void ImageDownloaderOnScreenChecker::startCheckingForOnScreenPosition(cocos2d::Sprite* sender)
{
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule([=](float dt)
                        {
                            ImageDownloaderOnScreenChecker::checkIfElementIsOnScreen();
                        }, this, 0.1f, kRepeatForever, 0.0f, false, "myCallbackKey");
}

//------------------------------------All methods are private below this line------------------

void ImageDownloaderOnScreenChecker::checkIfElementIsOnScreen()
{
    CCLOG("check is running");
    
}

void ImageDownloaderOnScreenChecker::elementAppeared()
{
    if(elementOnScreen) return;
    elementOnScreen = true;
}

void ImageDownloaderOnScreenChecker::elementDisappeared()
{
    if(!elementOnScreen) return;
    elementOnScreen = false;
}
