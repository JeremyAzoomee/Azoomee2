#ifndef TinizineCommon_ImageDownloaderCacheCleanerLogic_h
#define TinizineCommon_ImageDownloaderCacheCleanerLogic_h

#include <cocos/cocos2d.h>
#include "../Tinizine.h"

NS_TZ_BEGIN

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
  
NS_TZ_END

#endif
