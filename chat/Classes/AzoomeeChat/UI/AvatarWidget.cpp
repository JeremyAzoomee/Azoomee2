#include "AvatarWidget.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>


using namespace cocos2d;



NS_AZOOMEE_CHAT_BEGIN

bool AvatarWidget::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    // TODO: get positions and sizes from config
    
    // Placeholder for when avatar is loading or not recognised
    _avatarPlaceholder = ui::ImageView::create();
    _avatarPlaceholder->ignoreContentAdaptWithSize(false); // stretch the image
    _avatarPlaceholder->setAnchorPoint(Vec2(0.5f, 0.5f));
    _avatarPlaceholder->setSizeType(ui::Widget::SizeType::PERCENT);
    _avatarPlaceholder->setSizePercent(Vec2(1.0f, 1.0f));
    _avatarPlaceholder->setPositionType(ui::Widget::PositionType::PERCENT);
    _avatarPlaceholder->setPositionPercent(Vec2(0.5f, 0.5f));
    _avatarPlaceholder->loadTexture("res/chat/ui/avatar/contact_inknown_icon.png");
    addChild(_avatarPlaceholder);
    
    
    // Main avatar roundel
    
    _outerFrame = ui::ImageView::create();
    _outerFrame->ignoreContentAdaptWithSize(false); // stretch the image
    _outerFrame->setAnchorPoint(Vec2(0.5f, 0.5f));
    _outerFrame->setSizeType(ui::Widget::SizeType::PERCENT);
    _outerFrame->setSizePercent(Vec2(1.0f, 1.0f));
    _outerFrame->setPositionType(ui::Widget::PositionType::PERCENT);
    _outerFrame->setPositionPercent(Vec2(0.5f, 0.5f));
    addChild(_outerFrame);
    
    _background = ui::ImageView::create("res/chat/ui/avatar/avatar_background.png");
    _background->ignoreContentAdaptWithSize(false); // stretch the image
    _background->setAnchorPoint(Vec2(0.5f, 0.5f));
    _background->setSizeType(ui::Widget::SizeType::PERCENT);
    _background->setSizePercent(Vec2(1.0f, 1.0f)); //0.86f, 0.86f));
    _background->setPositionType(ui::Widget::PositionType::PERCENT);
    _background->setPositionPercent(Vec2(0.5f, 0.5f));
    _outerFrame->addChild(_background);
    
    // Setup the clipping mask
    _clippingNode = ClippingNode::create();
    const Size& clipSize = _background->getVirtualRendererSize();
    _clippingNode->setContentSize(clipSize);
    _clippingNode->setAnchorPoint(Vec2(0.5f, 0.5f));
    _background->addChild(_clippingNode);
    
    // The mask is created to match the size of the background
    _stencilMask = DrawNode::create();
    _stencilMask->setContentSize(clipSize);
    // Circle to cover the main area
    const float circleResolution = 64.0f; // todo: config
    _stencilMask->drawSolidCircle(Vec2(clipSize.width * 0.5f, clipSize.height * 0.5f),
                                  clipSize.width * 0.5f, 0.0f, circleResolution,
                                  Color4F::GREEN);
    // A rect above the circle half way point, since some oomees have ears sticking outside the circle
    _stencilMask->drawSolidRect(Vec2(clipSize.width * -0.5f, clipSize.height * 0.5f),
                                Vec2(clipSize.width * 2.0f, clipSize.height * 1.5f),
                                Color4F::GREEN);
    _clippingNode->setStencil(_stencilMask);
    
    // The avatar image
    _avatarImage = ui::ImageView::create();
    _avatarImage->ignoreContentAdaptWithSize(false); // stretch the image
    _avatarImage->setAnchorPoint(Vec2(0.5f, 0.5f));
    _avatarImage->setPosition(Vec2(clipSize.width * 0.5f, clipSize.height * 0.6f));
    _avatarImage->setScale(0.55f); //0.865f);
    _clippingNode->addChild(_avatarImage);
    
    
    // Front frame to cover aliasing on the clipping circle
    _frameFront = ui::ImageView::create("res/chat/ui/avatar/l_avatar_frame_front.png");
    _frameFront->ignoreContentAdaptWithSize(false); // stretch the image
    _frameFront->setAnchorPoint(Vec2(0.0f, 0.0f));
    _background->addChild(_frameFront);
    
    
    // Default data
    setAvatarForFriend(nullptr);
  
    return true;
}

void AvatarWidget::onEnter()
{
    Super::onEnter();
}

void AvatarWidget::onExit()
{
    Super::onExit();
    
    if(_avatarDownloader)
    {
        _avatarDownloader->setDelegate(nullptr);
    }
}

#pragma mark - Size changes

void AvatarWidget::onSizeChanged()
{
    Super::onSizeChanged();
    
    // Since clipping node isn't a ui widget, we need to resize it here
    // We use scale so that the stencil is also resized together
    const Size& normalClipSize = _clippingNode->getContentSize();
    const Size& displaySize = _background->getContentSize();
    const float clipSizePerc = 0.938f; // todo: from config
    const float scaleClipBy = displaySize.width / normalClipSize.width * clipSizePerc;
    _clippingNode->setScale(scaleClipBy);
    
    // Make sure the clipping node is centered
    const Size& clipParentSize = _clippingNode->getParent()->getContentSize();
    _clippingNode->setPosition(Vec2(clipParentSize.width * 0.5f, clipParentSize.height * 0.5f));
    
    // Resize the frame front, keeping aspect ratio
    const Size& frameFrontImageSize = _frameFront->getVirtualRendererSize();
    float frameFrontAspectRatio = frameFrontImageSize.height / frameFrontImageSize.width;
    _frameFront->setContentSize(Size(displaySize.width, displaySize.width * frameFrontAspectRatio));
}

#pragma mark - Public

void AvatarWidget::setAvatarForFriend(const FriendRef& friendData)
{
    const std::string& avatarURL = (friendData) ? friendData->avatarURL() : "";
    _avatarPlaceholder->setVisible(avatarURL.empty());
    _outerFrame->setVisible(!avatarURL.empty());
    
    if(_avatarDownloader)
    {
        _avatarDownloader->setDelegate(nullptr);
        _avatarDownloader.reset();
    }
    
    // Download image (will auto get from cache)
    if(!avatarURL.empty())
    {
        _avatarDownloader = ImageDownloader::create("avatars", ImageDownloader::CacheMode::File);
        _avatarDownloader->downloadImage(this, avatarURL);
    }
}

#pragma mark - ImageDownloaderDelegate

void AvatarWidget::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _avatarImage->loadTexture(downloader->getLocalImagePath());
    
    if(_avatarDownloader)
    {
        _avatarDownloader->setDelegate(nullptr);
        _avatarDownloader.reset();
    }
}

NS_AZOOMEE_CHAT_END
