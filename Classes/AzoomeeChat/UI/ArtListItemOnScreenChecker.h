//
//  ArtListItemOnScreenChecker.h
//  AzoomeeChat
//
//  Created by Macauley on 24/10/2017.
//
//

#ifndef ArtListItemOnScreenChecker_h
#define ArtListItemOnScreenChecker_h

#include "../AzoomeeChat.h"
#include <AzoomeeCommon/ImageDownloader/OnScreenChecker.h>

USING_NS_TZ

NS_AZ_CHAT_BEGIN

class ArtListItemOnScreenChecker: public OnScreenChecker
{
protected:
    virtual void elementDisappeared(cocos2d::Node *sender) override;
    virtual void elementAppeared(cocos2d::Node *sender) override;
};

NS_AZ_CHAT_END

#endif /* ArtListItemOnScreenChecker_h */
