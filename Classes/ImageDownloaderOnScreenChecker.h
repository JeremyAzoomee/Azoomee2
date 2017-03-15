#ifndef __IMAGEDOWNLOADER_ONSCREENCHECKER_H__
#define __IMAGEDOWNLOADER_ONSCREENCHECKER_H__

#include "cocos2d.h"

class ImageDownloaderOnScreenChecker : public cocos2d::Ref
{
public:
    void startCheckingForOnScreenPosition(cocos2d::Node* sender);
    bool checkIfElementIsOnScreen(cocos2d::Node *item);
    void endCheck();
    
private:
    void elementDisappeared(cocos2d::Node *sender);
    void elementAppeared(cocos2d::Node *sender);
    bool elementOnScreen;
};

#endif
