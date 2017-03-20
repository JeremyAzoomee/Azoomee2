#include "cocos2d.h"

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
