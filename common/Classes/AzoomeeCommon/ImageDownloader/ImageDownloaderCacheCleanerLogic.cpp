#include "ImageDownloaderCacheCleanerLogic.h"

using namespace cocos2d;


namespace Azoomee
{

static ImageDownloaderCacheCleanerLogic *_sharedImageDownloaderCacheCleanerLogic = NULL;

ImageDownloaderCacheCleanerLogic* ImageDownloaderCacheCleanerLogic::getInstance()
{
    if (! _sharedImageDownloaderCacheCleanerLogic)
    {
        _sharedImageDownloaderCacheCleanerLogic = new ImageDownloaderCacheCleanerLogic();
        _sharedImageDownloaderCacheCleanerLogic->init();
    }
    
    return _sharedImageDownloaderCacheCleanerLogic;
}

ImageDownloaderCacheCleanerLogic::~ImageDownloaderCacheCleanerLogic(void)
{
}

bool ImageDownloaderCacheCleanerLogic::init(void)
{
    numberOfImagesRemoved = 0;
    return true;
}

void ImageDownloaderCacheCleanerLogic::purgeCache()
{
    Director::getInstance()->purgeCachedData();
}

void ImageDownloaderCacheCleanerLogic::imageRemoved()
{
    numberOfImagesRemoved++;
    
    if(numberOfImagesRemoved > 12)
    {
        numberOfImagesRemoved = 0;
        ImageDownloaderCacheCleanerLogic::purgeCache();
    }
}
  
}
