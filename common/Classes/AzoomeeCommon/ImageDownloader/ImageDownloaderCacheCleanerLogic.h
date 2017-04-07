#ifndef AzoomeeCommon_ImageDownloaderCacheCleanerLogic_h
#define AzoomeeCommon_ImageDownloaderCacheCleanerLogic_h

#include <cocos/cocos2d.h>


namespace Azoomee
{

class ImageDownloaderCacheCleanerLogic : public cocos2d::Ref
{
    
public:
    static ImageDownloaderCacheCleanerLogic* getInstance(void);
    virtual ~ImageDownloaderCacheCleanerLogic();
    bool init(void);
    
    void imageRemoved();
    
    
private:
    void purgeCache();
    int numberOfImagesRemoved;
};
  
}

#endif
