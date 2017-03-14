#include "ImageDownloader.h"
#include "CookieDataStorage.h"
#include "ElectricDreamsTextStyles.h"
#include "StringMgr.h"
#include "ConfigStorage.h"

USING_NS_CC;
using namespace network;
using namespace cocos2d;

bool ImageDownloader::initWithURLAndSize(std::string url, std::string type, Size size, Vec2 shape)
{
    if ( !Sprite::init() )
    {
        return false;
    }
    
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    this->addPlaceHolderImage(type, size, shape);
    this->retain();
    //this->addLoadingAnimation();
    
    imageDownloaderLogic = new ImageDownloaderLogic();
    imageDownloaderLogic->startProcessingImage(this, url);
    
    return true;
}

bool ImageDownloader::initWithUrlAndSizeWithoutPlaceholder(std::string url, cocos2d::Size size)
{
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    this->retain();
    
    imageDownloaderLogic = new ImageDownloaderLogic();
    imageDownloaderLogic->groupLogo = true;
    imageDownloaderLogic->startProcessingImage(this, url);
    
    return true;
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
    
    if(!addStarted) return;
    
    if ( (resulting_texture) && (!aboutToExit))
    {
        Size holderContentSize = this->getContentSize();
        
        auto finalImage = Sprite::createWithTexture( resulting_texture );
        finalImage->setPosition(holderContentSize / 2);
        finalImage->setOpacity(0);
        finalImage->setScaleX(holderContentSize.width / finalImage->getContentSize().width);
        finalImage->setScaleY(holderContentSize.height / finalImage->getContentSize().height);
        
        if(!aboutToExit) this->addChild(finalImage);
        
        finalImage->runAction(FadeIn::create(0.1));
        
        if(aboutToExit) this->release(); //If onExit was called in the meantime, we release this after adding the image.
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
    
    CCLOG("Sending false to downloadLogic");
    aboutToExit = true;
}

void ImageDownloader::onExit()
{
    CCLOG("onExit called");
    aboutToExit = true;
    if(imageDownloaderLogic) imageDownloaderLogic->senderDeleted = true;
    if(!addStarted) this->release();
    Sprite::onExit();
}
