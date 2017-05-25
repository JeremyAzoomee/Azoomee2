#include "ImageDownloader.h"
#include "../Data/Cookie/CookieDataStorage.h"
#include "../UI/ElectricDreamsTextStyles.h"
#include "../Strings.h"
#include "../Data/ConfigStorage.h"
#include "ImageDownloaderCacheCleanerLogic.h"

using namespace cocos2d::network;
using namespace cocos2d;


NS_AZOOMEE_BEGIN
    
ImageDownloader::~ImageDownloader()
{
    if(imageDownloaderLogic)
    {
        imageDownloaderLogic->release();
        imageDownloaderLogic = nullptr;
    }
}

bool ImageDownloader::initWithURLAndSize(const std::string& url, const std::string& type, const Size& size, const Vec2& shape)
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
    imageDownloaderLogic = ImageDownloaderLogic::create();
    imageDownloaderLogic->retain();
    
    return true;
}

bool ImageDownloader::initWithUrlAndSizeWithoutPlaceholder(const std::string& url, const cocos2d::Size& size)
{
    identifier = CCRANDOM_0_1() * 1000 + 1000;
    
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    
    imageUrl = url;
    imageDownloaderLogic = new ImageDownloaderLogic();
    
    return true;
}
    
void ImageDownloader::setAttachNewBadgeToImage()
{
    shouldAddNewBadgeToImage = true;
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
    imageDownloaderLogic->downloadImage(this, imageUrl);
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

void ImageDownloader::removeLoadingAnimation()
{
    Node* loadingAnimation = this->getChildByName("loadingAnimation");
    if(loadingAnimation)
    {
        this->removeChild(loadingAnimation, true);
    }
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
        
        loadedImage = finalImage;
        
        if(shouldAddNewBadgeToImage)
            addNewBadgeToLoadedImage();
        if(!aboutToExit) this->addChild(loadedImage);
        
        finalImage->runAction(FadeIn::create(0.1));
    }
}
    
void ImageDownloader::addNewBadgeToLoadedImage()
{
    auto newBadge = Sprite::create("res/hqscene/newIcon2X2.png");
    newBadge->setAnchorPoint(Vec2(0.0, 0.5));
    newBadge->setPosition(0, loadedImage->getContentSize().height - newBadge->getContentSize().height *.75);
    newBadge->setOpacity(0);
    loadedImage->addChild(newBadge);
    
    newBadge->runAction(FadeIn::create(0.1));
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
    if(imageDownloaderLogic)
    {
        imageDownloaderLogic->setDelegate(nullptr);
        imageDownloaderLogic->release();
        imageDownloaderLogic = nullptr;
    }
    
    Node::onExit();
}

#pragma mark - ImageDownloaderDelegate

void ImageDownloader::onImageDownloadComplete(ImageDownloaderLogic* downloader)
{
    
    const std::string& filename = downloader->getLocalImagePath();
    addStarted = true;
    this->setName(filename);
    Director::getInstance()->getTextureCache()->addImageAsync(filename, CC_CALLBACK_1(ImageDownloader::imageAddedToCache, this));
}

NS_AZOOMEE_END
