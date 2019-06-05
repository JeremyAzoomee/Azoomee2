#include "RemoteImageSprite.h"
#include "../UI/ElectricDreamsTextStyles.h"
#include "../Strings.h"
#include "../Data/ConfigStorage.h"
#include "ImageDownloaderCacheCleanerLogic.h"

using namespace cocos2d::network;
using namespace cocos2d;


NS_AZOOMEE_BEGIN

RemoteImageSprite::RemoteImageSprite()
{
    imageDownloaderLogic = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
}
    
RemoteImageSprite::~RemoteImageSprite()
{
    if(imageDownloaderLogic)
    {
        imageDownloaderLogic->setDelegate(nullptr);
    }
}

bool RemoteImageSprite::initWithURLAndSize(const std::string& url, const std::string& type, const Size& size, const Vec2& shape)
{
    if(!Sprite::init())
    {
        return false;
    }

    identifier = CCRANDOM_0_1() * 1000 + 1000;
    
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    
    this->addPlaceHolderImage(type, size, shape);
    
    imageUrl = url;
    return true;
}

bool RemoteImageSprite::initWithUrlAndSizeWithoutPlaceholder(const std::string& url, const cocos2d::Size& size, bool useStencil)
{
    if(!Sprite::init())
    {
        return false;
    }
    
    identifier = CCRANDOM_0_1() * 1000 + 1000;
    
    this->setCascadeOpacityEnabled(true);
    this->setContentSize(size);
    
    imageUrl = url;
    return true;
}
    
void RemoteImageSprite::setAttachNewBadgeToImage()
{
    shouldAddNewBadgeToImage = true;
}

void RemoteImageSprite::onEnter()
{
    onScreenChecker = new ImageDownloaderOnScreenChecker();
    onScreenChecker->startCheckingForOnScreenPosition(this);
    
    Super::onEnter();
}

void RemoteImageSprite::startLoadingImage()
{
    if(loadedImage) return;
    imageDownloaderLogic->downloadImage(this, imageUrl);
}

void RemoteImageSprite::removeLoadedImage()
{
    if(loadedImage)
    {
        loadedImage->removeFromParent();
        loadedImage = nullptr;
        
        ImageDownloaderCacheCleanerLogic::getInstance()->imageRemoved();
    }
}

void RemoteImageSprite::resizeImage()
{
    if(loadedImage)
    {
        loadedImage->setPosition(this->getContentSize() / 2);
        if(!_keepAspectRatio)
        {
            loadedImage->setScaleX(this->getContentSize().width/ loadedImage->getContentSize().width);
            loadedImage->setScaleY(this->getContentSize().height/ loadedImage->getContentSize().height);
        }
        else
        {
            loadedImage->setScale(MIN(this->getContentSize().height/ loadedImage->getContentSize().height, this->getContentSize().width/ loadedImage->getContentSize().width));
        }
    }
}

void RemoteImageSprite::addPlaceHolderImage(std::string type, Size contentSize, Vec2 shape)
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

void RemoteImageSprite::addLoadingAnimation()
{
    auto loadingAnimation = LayerColor::create();
    loadingAnimation->setColor(Color3B::BLACK);
    loadingAnimation->setOpacity(150);
    loadingAnimation->setName("loadingAnimation");
    loadingAnimation->setContentSize(this->getContentSize());
    loadingAnimation->setPosition(0, 0);
    this->addChild(loadingAnimation);
    
    auto loadingLabel = createLabelSmallLoading(_("Loading"));
    loadingLabel->setPosition(this->getContentSize() / 2);
    loadingAnimation->addChild(loadingLabel);
}

void RemoteImageSprite::removeLoadingAnimation()
{
    Node* loadingAnimation = this->getChildByName("loadingAnimation");
    if(loadingAnimation)
    {
        this->removeChild(loadingAnimation, true);
    }
}

void RemoteImageSprite::imageAddedToCache(Texture2D* resulting_texture)
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
		
        if(!_keepAspectRatio)
        {
            finalImage->setScaleX(holderContentSize.width / finalImage->getContentSize().width);
            finalImage->setScaleY(holderContentSize.height / finalImage->getContentSize().height);
        }
        else
        {
            finalImage->setScale(MIN(holderContentSize.height / finalImage->getContentSize().height, holderContentSize.width / finalImage->getContentSize().width));
        }
        
        loadedImage = finalImage;
        
        if(shouldAddNewBadgeToImage)
        {
            addNewBadgeToLoadedImage();
        }
        if(!aboutToExit)
        {
            this->addChild(loadedImage);
        }
		
    }
    this->release();
}
    
void RemoteImageSprite::addNewBadgeToLoadedImage()
{
    auto newBadge = Sprite::create("res/hqscene/newBadge.png");
    newBadge->setAnchorPoint(Vec2(0.0f, 1.0f));
    newBadge->setPosition(0, loadedImage->getContentSize().height);
    newBadge->setOpacity(0);
    loadedImage->addChild(newBadge);
    
    newBadge->runAction(FadeIn::create(0.1));
}

void RemoteImageSprite::onExitTransitionDidStart()
{
    aboutToExit = true;
    Super::onExitTransitionDidStart();
}

void RemoteImageSprite::onExit()
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
        imageDownloaderLogic.reset();
    }
    
    Super::onExit();
}

void RemoteImageSprite::setKeepAspectRatio(bool keepAspectRatio)
{
    _keepAspectRatio = keepAspectRatio;
}

void RemoteImageSprite::addClippingNode(bool usingClippingNode)
{
    _clippingNode = ClippingNode::create();
    _stencil = ui::Scale9Sprite::create("res/artapp/popup_bg.png");
    _stencil->setContentSize(this->getContentSize());
    _stencil->setPosition(this->getContentSize()/2);
    _stencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _clippingNode->setStencil(_stencil);
    this->addChild(_clippingNode);
    if(usingClippingNode)
    {
        _clippingNode->setAlphaThreshold(0.5f);
    }
    else
    {
        _clippingNode->setAlphaThreshold(1.0f);
    }
}

#pragma mark - RemoteImageSpriteDelegate

void RemoteImageSprite::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    const std::string& filename = downloader->getLocalImagePath();
    addStarted = true;
    this->setName(filename);
    this->retain();
    Director::getInstance()->getTextureCache()->addImageAsync(filename, CC_CALLBACK_1(RemoteImageSprite::imageAddedToCache, this));
}

void RemoteImageSprite::onImageDownloadFailed()
{
    // do nothing
}

NS_AZOOMEE_END
