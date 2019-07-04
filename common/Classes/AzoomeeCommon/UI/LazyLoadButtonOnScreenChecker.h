//
//  LazyLoadButtonOnScreenChecker.h
//  AzoomeeCommon
//
//  Created by Macauley on 01/03/2018.
//

#ifndef LazyLoadButtonOnScreenChecker_h
#define LazyLoadButtonOnScreenChecker_h

#include "../Azoomee.h"
#include "../ImageDownloader/OnScreenChecker.h"

NS_AZOOMEE_BEGIN

class LazyLoadButtonOnScreenChecker : public OnScreenChecker
{
private:
    static int imageRemovedCount;
protected:
    
    virtual void elementDisappeared(cocos2d::Node *sender) override;
    virtual void elementAppeared(cocos2d::Node *sender) override;
};


NS_AZOOMEE_END


#endif /* LazyLoadButtonOnScreenChecker_h */
