#include "ImageDownloader.h"
#include <AzoomeeCommon/Data/Cookie/CookieDataStorage.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "ImageDownloaderCacheCleanerLogic.h"

USING_NS_CC;
using namespace network;
using namespace cocos2d;
using namespace Azoomee;


bool ImageDownloader::initWithURLAndSize(std::string url, std::string type, Size size, Vec2 shape)
{
    if ( !Sprite::init() )
    {
        return false;
    }
    
    identifier = CCRANDOM_0_1() * 1000 + 1000;
    
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    this->addPlaceHolderImage(type, size, shape);
    
    imageUrl = url;
    imageDownloaderLogic = new ImageDownloaderLogic();
    
    return true;
}

bool ImageDownloader::initWithUrlAndSizeWithoutPlaceholder(std::string url, cocos2d::Size size)
{
    identifier = CCRANDOM_0_1() * 1000 + 1000;
    
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    
    imageUrl = url;
    imageDownloaderLogic = new ImageDownloaderLogic();
    imageDownloaderLogic->groupLogo = true;
    
    return true;
}

void ImageDownloader::onEnter()
{
    onScreenChecker = new ImageDownloaderOnScreenChecker();
    onScreenChecker->startCheckingForOnScreenPosition(this);
    
    Node::onEnter();
}

void ImageDownloader::startLoadingImage()
{
    if(loadedImage) return;
    imageDownloaderLogic->startProcessingImage(this, imageUrl);
}

void ImageDownloader::removeLoadedImage()
{
    if(loadedImage)
    {
        loadedImage->getParent()->removeChild(loadedImage);
        loadedImage = nullptr;
        
        ImageDownloaderCacheCleanerLogic::getInstance()->imageRemoved();
    }
}

void ImageDownloader::addPlaceHolderImage(std::string type, Size contentSize, Vec2 shape)
{
    std::string placeholderImageFile = StringUtils::format("%s%.fX%.f.png",ConfigStorage::getInstance()->getPlaceholderImageForContentItemInCategory(type).c_str(),shape.x,shape.y);
    auto placeHolderImage = Sprite::create(placeholderImageFile);
    placeHolderImage->setPosition(this->getContentSize() / 2);
    placeHolderImage->setName("placeHolderImage");
    placeHolderImage->setScaleX(this->getContentSize().width / placeHolderImage->getContentSize().width);
    placeHolderImage->setScaleY(this->getContentSize().height / placeHolderImage->getContentSize().height);
    this->addChild(placeHolderImage);
}

void ImageDownloader::addLoadingAnimation()
{
    auto loadingAnimation = LayerColor::create();
    loadingAnimation->setColor(Color3B::BLACK);
    loadingAnimation->setOpacity(150);
    loadingAnimation->setName("loadingAnimation");
    loadingAnimation->setContentSize(this->getContentSize());
    loadingAnimation->setPosition(0, 0);
    this->addChild(loadingAnimation);
    
    auto loadingLabel = createLabelSmallLoading(StringMgr::getInstance()->getStringForKey(LOADING_LABEL));
    loadingLabel->setPosition(this->getContentSize() / 2);
    loadingAnimation->addChild(loadingLabel);
}

void ImageDownloader::imageAddedToCache(Texture2D* resulting_texture)
{
    if ( (resulting_texture) && (!aboutToExit))
    {
        if((identifier < 999)||(identifier > 2001))
        {
            return;
        }
        Size holderContentSize = this->getContentSize();
        
        auto finalImage = Sprite::createWithTexture( resulting_texture );
        finalImage->setPosition(holderContentSize / 2);
        finalImage->setOpacity(0);
        finalImage->setScaleX(holderContentSize.width / finalImage->getContentSize().width);
        finalImage->setScaleY(holderContentSize.height / finalImage->getContentSize().height);
        
        if(!aboutToExit) this->addChild(finalImage);
        loadedImage = finalImage;
        
        finalImage->runAction(FadeIn::create(0.1));
    }
}

void ImageDownloader::addDownloadedImage(std::string fileName)
{
    addStarted = true;
    this->setName(fileName);
    Director::getInstance()->getTextureCache()->addImageAsync(fileName, CC_CALLBACK_1(ImageDownloader::imageAddedToCache, this));
}

void ImageDownloader::onExitTransitionDidStart()
{
    aboutToExit = true;
    Node::onExitTransitionDidStart();
}

void ImageDownloader::onExit()
{
    if(onScreenChecker)
    {
        onScreenChecker->endCheck();
        onScreenChecker->release();
    }
    
    aboutToExit = true;
    if(imageDownloaderLogic) imageDownloaderLogic->senderDeleted = true;
    Node::onExit();
}
