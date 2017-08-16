//
//  ArtImageOnScreenChecker.h
//  azoomee2
//
//  Created by Macauley on 27/07/2017.
//
//

#ifndef ArtImageOnScreenChecker_h
#define ArtImageOnScreenChecker_h

#include <AzoomeeCommon/ImageDownloader/OnScreenChecker.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class ArtImageOnScreenChecker : public OnScreenChecker
{
private:
    static int imageRemovedCount;
protected:
    virtual void elementDisappeared(cocos2d::Node *sender) override;
    virtual void elementAppeared(cocos2d::Node *sender) override;

};

NS_AZOOMEE_END

#endif /* ArtImageOnScreenChecker_h */
