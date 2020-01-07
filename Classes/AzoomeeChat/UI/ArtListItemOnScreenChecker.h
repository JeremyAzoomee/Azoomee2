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

NS_AZOOMEE_CHAT_BEGIN

class ArtListItemOnScreenChecker: public OnScreenChecker
{
protected:
    virtual void elementDisappeared(cocos2d::Node *sender) override;
    virtual void elementAppeared(cocos2d::Node *sender) override;
};

NS_AZOOMEE_CHAT_END

#endif /* ArtListItemOnScreenChecker_h */
