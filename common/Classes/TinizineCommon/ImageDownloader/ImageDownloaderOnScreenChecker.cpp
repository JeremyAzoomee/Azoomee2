#include "ImageDownloaderOnScreenChecker.h"
#include "RemoteImageSprite.h"

using namespace cocos2d;


NS_TZ_BEGIN
//------------------------------------All methods are private below this line------------------

void ImageDownloaderOnScreenChecker::elementAppeared(Node* sender)
{
    RemoteImageSprite* owner = (RemoteImageSprite*)sender;
    owner->startLoadingImage();
}

void ImageDownloaderOnScreenChecker::elementDisappeared(Node* sender)
{
    RemoteImageSprite* owner = (RemoteImageSprite*)sender;
    owner->removeLoadedImage();
}
  
NS_TZ_END
