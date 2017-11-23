#ifndef AzoomeeCommon_ImageDownloaderOnScreenChecker_h
#define AzoomeeCommon_ImageDownloaderOnScreenChecker_h

#include "OnScreenChecker.h"
#include <cocos/cocos2d.h>


namespace Azoomee
{

class ImageDownloaderOnScreenChecker : public OnScreenChecker
{
    
protected:
    virtual void elementDisappeared(cocos2d::Node *sender) override;
    virtual void elementAppeared(cocos2d::Node *sender) override;
};
  
}

#endif
