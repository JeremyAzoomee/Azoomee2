//
//  ShareInChatLayer.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 18/11/2019.
//

#include "ShareInChatLayer.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/Utils/DirUtil.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

const float ShareInChatLayer::kSavedImgWidth = 640.0f;

bool ShareInChatLayer::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setSwallowTouches(true);
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0f,1.0f));
    setBackGroundColorType(BackGroundColorType::SOLID);
    setBackGroundColor(Colours::Color_3B::darkIndigo);
    
    _popupBody = ui::Layout::create();
    _popupBody->setSizeType(SizeType::PERCENT);
    _popupBody->setSizePercent(Vec2(0.98f, 0.99f));
    _popupBody->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _popupBody->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    addChild(_popupBody);
    
    _bgColour = RoundedRectSprite::create();
    _bgColour->setTexture("res/decoration/white_1px.png");
    _bgColour->setCornerRadius(26);
    _bgColour->setColor(Colours::Color_3B::darkIndigoThree);
    _bgColour->setRoundedCorners(false, false, true, true);
    _bgColour->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _bgColour->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _popupBody->addChild(_bgColour);
    
    _bgPattern = RoundedRectSprite::create();
    _bgPattern->setTexture("res/decoration/pattern_stem_tile.png");
    _bgPattern->setCornerRadius(26);
    _bgPattern->setScaleMode(RoundedRectSprite::ScaleMode::TILE);
    _bgPattern->setColor(Colours::Color_3B::azure);
    _bgPattern->setRoundedCorners(false, false, true, true);
    _bgPattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _bgPattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _popupBody->addChild(_bgPattern);
    
    _bgGradient = LayerGradient::create();
    _bgGradient->setStartColor(Colours::Color_3B::darkIndigoThree);
    _bgGradient->setStartOpacity(0);
    _bgGradient->setEndColor(Colours::Color_3B::darkIndigoThree);
    _bgGradient->setEndOpacity(255);
    _bgGradient->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _bgGradient->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
    _popupBody->addChild(_bgGradient);
    
    _bodyLayout = ui::Layout::create();
    _bodyLayout->setSizeType(SizeType::PERCENT);
    _bodyLayout->setSizePercent(Vec2(1.0f, 1.0f));
    _bodyLayout->setLayoutType(Type::VERTICAL);
    _popupBody->addChild(_bodyLayout);
    
    _closeButton = ui::Button::create("res/hqscene/episode_select_close.png");
    _closeButton->setLayoutParameter(CreateLeftLinearLayoutParam());
    _closeButton->setSizeType(SizeType::PERCENT);
    _closeButton->setSizePercent(Vec2(0.0f, 0.1f));
    _closeButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_callback)
            {
                _callback();
            }
        }
    });
    _bodyLayout->addChild(_closeButton);
    
    _imageHolder = ui::Layout::create();
    _imageHolder->setSizeType(SizeType::PERCENT);
    _imageHolder->setSizePercent(Vec2(0.8f, 0.25f));
    _imageHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _bodyLayout->addChild(_imageHolder);
    
    _contentImage = RoundedRectSprite::create();
    _contentImage->setCornerRadius(30);
    _contentImage->setScaleMode(RoundedRectSprite::ScaleMode::FILL);
    _contentImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentImage->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _imageHolder->addChild(_contentImage);
    
    _shareMessage = DynamicText::create(_("Share with a friend"), Style::Font::poppinsBold, 92);
    _shareMessage->setTextColor(Color4B::WHITE);
    _shareMessage->setOverflow(Label::Overflow::SHRINK);
    _shareMessage->setTextVerticalAlignment(TextVAlignment::CENTER);
    _shareMessage->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _shareMessage->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,50,0)));
    _bodyLayout->addChild(_shareMessage);
    
    _friendListScroll = ui::ListView::create();
    _friendListScroll->setSizeType(SizeType::PERCENT);
    _friendListScroll->setSizePercent(Vec2(1.0f,0.5f));
    _friendListScroll->setLayoutParameter(CreateCenterVerticalLinearLayoutParam());
    _friendListScroll->setItemsMargin(32);
    _friendListScroll->setBounceEnabled(true);
    _friendListScroll->setBottomPadding(160);
    _bodyLayout->addChild(_friendListScroll);
    
    _friendsList = FriendsList::create();
    _friendsList->setTileSize(Size(300,450));
    _friendsList->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _friendsList->setFriendSelectedCallback([this](Chat::FriendRef friendData) {
        Chat::MessageRef message = nullptr;
        if(_shareType == CONTENT)
        {
            message = Chat::Message::createContentMessage(_contentId);
        }
        else
        {
            Sprite* artImage = Sprite::create(_imgFilename);
            artImage->setScale(kSavedImgWidth/artImage->getContentSize().width);
            artImage->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            
            RenderTexture* renderTex = RenderTexture::create(kSavedImgWidth, artImage->getContentSize().height * artImage->getScale());
            renderTex->beginWithClear(0, 0, 0, 0);
            artImage->visit();
            renderTex->end();
            renderTex->saveToFile(DirUtil::getCachesPath() + "temp.png", Image::Format::PNG);
            Director::getInstance()->getRenderer()->render();
            
            char* str = nullptr;
            const std::string& filecont =  FileUtils::getInstance()->getStringFromFile(DirUtil::getCachesPath() + "temp.png");
            base64Encode((unsigned char*)filecont.c_str(), (unsigned int)filecont.length(), &str);
            FileUtils::getInstance()->removeFile(DirUtil::getCachesPath() + "temp.png");
            message = Chat::Message::createArtMessage(std::string(str));
        }
        
        if(message)
        {
            AudioMixer::getInstance()->playEffect("laser_whoosh_ripple.mp3");
            Chat::ChatAPI::getInstance()->sendMessage(friendData, message);
        }
        if(_callback)
        {
            _callback();
        }
    });
    
    _friendsList->setShowAddFriendTile(false);
    _friendListScroll->addChild(_friendsList);
    
    return true;
}

void ShareInChatLayer::onEnter()
{
    _friendsList->setFriendsList(Chat::ChatAPI::getInstance()->getFriendList());
    
    Super::onEnter();
}

void ShareInChatLayer::onExit()
{
    Super::onExit();
}

void ShareInChatLayer::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();
    const Size& contentSize = getContentSize();
    bool isPortrait = contentSize.width < contentSize.height;
    
    const Rect& safeAreaRect = Director::getInstance()->getSafeAreaRect();
    const float safeZonePaddingPercentTop = !isPortrait ? (contentSize.width - (safeAreaRect.origin.x + safeAreaRect.size.width)) / contentSize.width  : (contentSize.height - (safeAreaRect.origin.y + safeAreaRect.size.height)) / contentSize.height;
    const float safeZonePaddingPercentBottom = !isPortrait ? (safeAreaRect.origin.x - origin.x) / contentSize.width  : (safeAreaRect.origin.y - origin.y) / contentSize.height;
    if(!isPortrait)
    {
        _popupBody->setSizePercent(Vec2(0.98f - (2 * MAX(safeZonePaddingPercentTop, safeZonePaddingPercentBottom)), 0.99f));
    }
    else
    {
        _popupBody->setSizePercent(Vec2(0.98f, 0.99f - safeZonePaddingPercentTop));
    }
    
    const Size& windowSize = Size(contentSize.width * _popupBody->getSizePercent().x, contentSize.height * _popupBody->getSizePercent().y);
    SetSizePercentWidthAspectRatio(_closeButton, Vec2(1.0f,1.0f));
    _bgColour->setContentSize(windowSize);
    _bgPattern->setContentSize(Size(windowSize.width, windowSize.height * (isPortrait ? 0.6f : 0.2f)));
    _bgGradient->setContentSize(Size(windowSize.width, windowSize.height * (isPortrait ? 0.6f : 0.2f)));
    _bgGradient->setNormalizedPosition(Vec2(0.0f, (isPortrait ? 0.4f : 0.8f)));
    const Size& imgPxSize = _contentImage->getTexture()->getContentSizeInPixels();
    const float imgScale = MIN(_imageHolder->getContentSize().width / imgPxSize.width, _imageHolder->getContentSize().height / imgPxSize.height);
    _contentImage->setContentSize(imgPxSize * imgScale);
    const float minCols = isPortrait ? 3 : 5;
    _friendsList->setTileSize(Size(windowSize.width / minCols, (windowSize.width / minCols)));
    _friendsList->setMinColumns(minCols);
    _friendsList->setContentSize(Size(windowSize.width * (isPortrait ? 0.9f : 0.85f),0));
}

void ShareInChatLayer::setShareType(const ShareType &shareType)
{
    _shareType = shareType;
}

void ShareInChatLayer::setContentImage(const std::string& imgFilename)
{
    _imgFilename = imgFilename;
    _contentImage->setTexture(imgFilename);
}

void ShareInChatLayer::setContentId(const std::string& contentId)
{
    _contentId = contentId;
}

void ShareInChatLayer::setOnCompleteCallback(const OnCompleteCallbackCallback& callback)
{
    _callback = callback;
}

NS_AZ_END
