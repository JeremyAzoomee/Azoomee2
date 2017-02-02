#include "ImageDownloader.h"
#include "CookieDataStorage.h"
#include "ImageDownloaderLogic.h"

USING_NS_CC;
using namespace network;
using namespace cocos2d;

bool ImageDownloader::initWithURLAndSize(std::string url, Size size)
{
    if ( !Sprite::init() )
    {
        return false;
    }
    
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    this->addPlaceHolderImage();
    //this->addLoadingAnimation();
    
    ImageDownloaderLogic *imageDownloaderLogic = new ImageDownloaderLogic();
    imageDownloaderLogic->startProcessingImage(this, url);
    
    return true;
}

bool ImageDownloader::initWithUrlAndSizeWithoutPlaceholder(std::string url, cocos2d::Size size)
{
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    
    ImageDownloaderLogic *imageDownloaderLogic = new ImageDownloaderLogic();
    imageDownloaderLogic->groupLogo = true;
    imageDownloaderLogic->startProcessingImage(this, url);
    
    return true;
}

void ImageDownloader::addPlaceHolderImage()
{
    auto placeHolderImage = Sprite::create("res/hqscene/placeholder.png");
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
    
    auto loadingLabel = Label::createWithTTF("Loading", "fonts/azoomee.ttf", 20);
    loadingLabel->setPosition(this->getContentSize() / 2);
    loadingAnimation->addChild(loadingLabel);
}

void ImageDownloader::addDownloadedImage(std::string fileName)
{
    if(!aboutToExit)
    {
        auto finalImage = Sprite::create(fileName);
        finalImage->setPosition(this->getContentSize() / 2);
        finalImage->setOpacity(0);
        finalImage->setScaleX(this->getContentSize().width / finalImage->getContentSize().width);
        finalImage->setScaleY(this->getContentSize().height / finalImage->getContentSize().height);
        this->addChild(finalImage);
        
        finalImage->runAction(FadeIn::create(0.1));
    }
}

void ImageDownloader::onExitTransitionDidStart()
{
    aboutToExit = true;
}
