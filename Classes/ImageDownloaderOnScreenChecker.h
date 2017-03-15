#include "cocos2d.h"

class ImageDownloaderOnScreenChecker : public cocos2d::Ref
{
public:
    void startCheckingForOnScreenPosition(cocos2d::Sprite* sender);
    
private:
    void checkIfElementIsOnScreen();
    void elementDisappeared();
    void elementAppeared();
    bool elementOnScreen;
};
