#ifndef AzoomeeCommon_ImageDownloaderOnScreenChecker_h
#define AzoomeeCommon_ImageDownloaderOnScreenChecker_h

#include "OnScreenChecker.h"
#include <cocos/cocos2d.h>
#include "../Tinizine.h"

NS_TZ_BEGIN

class ImageDownloaderOnScreenChecker : public OnScreenChecker
{
    
protected:
    virtual void elementDisappeared(cocos2d::Node *sender) override;
    virtual void elementAppeared(cocos2d::Node *sender) override;
};
  
NS_TZ_END

#endif
