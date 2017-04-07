#ifndef AzoomeeCommon_ImageDownloaderOnScreenChecker_h
#define AzoomeeCommon_ImageDownloaderOnScreenChecker_h

#include <cocos/cocos2d.h>


namespace Azoomee
{

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
  
}

#endif
