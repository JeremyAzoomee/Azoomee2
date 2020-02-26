//
//  LazyLoadButtonOnScreenChecker.h
//  AzoomeeCommon
//
//  Created by Macauley on 01/03/2018.
//

#ifndef LazyLoadButtonOnScreenChecker_h
#define LazyLoadButtonOnScreenChecker_h

#include "../Tinizine.h"
#include "../ImageDownloader/OnScreenChecker.h"

NS_TZ_BEGIN

class LazyLoadButtonOnScreenChecker : public OnScreenChecker
{
private:
    static int imageRemovedCount;
protected:
    
    virtual void elementDisappeared(cocos2d::Node *sender) override;
    virtual void elementAppeared(cocos2d::Node *sender) override;
};


NS_TZ_END


#endif /* LazyLoadButtonOnScreenChecker_h */
