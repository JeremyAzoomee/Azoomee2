#include "RemoteImageSprite.h"
#include "../UI/ElectricDreamsTextStyles.h"
#include "../Utils/LocaleManager.h"
#include "../Data/ConfigStorage.h"
#include "ImageDownloaderCacheCleanerLogic.h"

using namespace cocos2d::network;
using namespace cocos2d;


NS_AZOOMEE_BEGIN

RemoteImageSprite::RemoteImageSprite()
{
	
}
    
RemoteImageSprite::~RemoteImageSprite()
{
    if(_imageDownloaderLogic)
    {
        _imageDownloaderLogic->setDelegate(nullptr);
    }
}

bool RemoteImageSprite::initWithURLAndSize(const std::string& url, const std::string& type, const Size& size, const Vec2& shape)
{
    if(!Super::init())
    {
        return false;
    }
    ignoreContentAdaptWithSize(false);
    _imageDownloaderLogic = ImageDownloader::create(ImageDownloader::kImageCachePath, ImageDownloader::CacheMode::File);
    
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    
    this->addPlaceHolderImage(type, size, shape);
    
    _imageUrl = url;
	
    return true;
}

bool RemoteImageSprite::initWithUrlAndSizeWithoutPlaceholder(const std::string& url, const cocos2d::Size& size, bool useStencil)
{
    if(!Super::init())
    {
        return false;
    }
    ignoreContentAdaptWithSize(false);
    _imageDownloaderLogic = ImageDownloader::create(ImageDownloader::kImageCachePath, ImageDownloader::CacheMode::File);
    
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    
    _imageUrl = url;
    return true;
}

void RemoteImageSprite::onEnter()
{
	startCheckingForOnScreenPosition(this);

    Super::onEnter();
}

void RemoteImageSprite::startLoadingImage()
{
    if(_loadedImage || !_imageDownloaderLogic)
	{
		return;
	}
    _imageDownloaderLogic->downloadImage(this, _imageUrl);
}

void RemoteImageSprite::removeLoadedImage()
{
    if(_loadedImage)
    {
        _loadedImage->removeFromParent();
        _loadedImage = nullptr;
        
        ImageDownloaderCacheCleanerLogic::getInstance()->imageRemoved();
    }
}

void RemoteImageSprite::resizeImage()
{
    if(_loadedImage)
    {
        const Size& contentSize = getContentSize();
        const Size& imageSize = _loadedImage->getContentSize();
        _loadedImage->setPosition(this->getContentSize() / 2);
        if(_keepAspectRatio)
        {
            _loadedImage->setScale(MIN(contentSize.height/ imageSize.height, contentSize.width / imageSize.width));
        }
        else
        {
            _loadedImage->setScaleX(contentSize.width/ imageSize.width);
            _loadedImage->setScaleY(contentSize.height/ imageSize.height);
        }
    }
}

void RemoteImageSprite::addPlaceHolderImage(const std::string& type, const Size& contentSize, const Vec2& shape)
{
    std::string placeholderImageFile = StringUtils::format("%s%.fX%.f.png",ConfigStorage::getInstance()->getPlaceholderImageForContentItemInCategory(type).c_str(),shape.x,shape.y);
    if(!FileUtils::getInstance()->isFileExist(placeholderImageFile))
    {
        placeholderImageFile = StringUtils::format("%s1X1.png",ConfigStorage::getInstance()->getPlaceholderImageForContentItemInCategory(type).c_str());
    }
    
    auto placeHolderImage = Sprite::create(placeholderImageFile);
    placeHolderImage->setPosition(this->getContentSize() / 2);
    placeHolderImage->setName("placeHolderImage");
    placeHolderImage->setScaleX(this->getContentSize().width / placeHolderImage->getContentSize().width);
    placeHolderImage->setScaleY(this->getContentSize().height / placeHolderImage->getContentSize().height);
    this->addChild(placeHolderImage);
}

void RemoteImageSprite::imageAddedToCache(const std::string& localPath)
{
    const Size& holderContentSize = this->getContentSize();
    
    auto finalImage = Sprite::create(localPath);
    if(finalImage == nullptr)
    {
        return;
    }
    const Size& imageSize = finalImage->getContentSize();
    
    finalImage->setPosition(holderContentSize / 2);
    
    if(_keepAspectRatio)
    {
        finalImage->setScale(MIN(holderContentSize.height / imageSize.height, holderContentSize.width / imageSize.width));
    }
    else
    {
        finalImage->setScaleX(holderContentSize.width / imageSize.width);
        finalImage->setScaleY(holderContentSize.height / imageSize.height);
    }
    
    _loadedImage = finalImage;
    

    addChild(_loadedImage);
}

void RemoteImageSprite::onExit()
{
    if(_imageDownloaderLogic)
    {
        _imageDownloaderLogic->setDelegate(nullptr);
    }
    
    Super::onExit();
}

void RemoteImageSprite::setKeepAspectRatio(bool keepAspectRatio)
{
    _keepAspectRatio = keepAspectRatio;
}

void RemoteImageSprite::elementAppeared(cocos2d::Node *sender)
{
    startLoadingImage();
}

void RemoteImageSprite::elementDisappeared(cocos2d::Node *sender)
{
    removeLoadedImage();
}

#pragma mark - RemoteImageSpriteDelegate

void RemoteImageSprite::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    const std::string& filename = downloader->getLocalImagePath();
    this->setName(filename);
    imageAddedToCache(downloader->getLocalImagePath());
}

void RemoteImageSprite::onImageDownloadFailed()
{
    // do nothing
}

NS_AZOOMEE_END
