//
//  OomeeDisplay.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 07/10/2019.
//

#include "OomeeDisplay.h"
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include "SceneManagerScene.h"
#include "Style.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const cocos2d::Size OomeeDisplay::kKidCodeFrameSize = Size(334,217);
const cocos2d::Size OomeeDisplay::kKidCodeFramePadding = Size(10,10);

bool OomeeDisplay::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0f, 1.0f));
    setClippingEnabled(true);
    
    const Color3B& bgColour = Colours::Color_3B::darkIndigoThree;
    
    _background = RoundedRectSprite::create();
    _background->setTexture("res/decoration/white_1px.png");
    _background->setCornerRadius(60);
    _background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _background->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _background->setColor(bgColour);
    addChild(_background);
    
    _stencil = Sprite::create("res/hqscene/circle.png");
    _patternClipper = ClippingNode::create(_stencil);
    _patternClipper->setAlphaThreshold(0.9f);
    addChild(_patternClipper);
    
    _bgPattern = RoundedRectSprite::create();
    _bgPattern->setTexture("res/decoration/pattern_stem_tile.png");
    _bgPattern->setCornerRadius(0);
    _bgPattern->setScaleMode(RoundedRectSprite::ScaleMode::TILE);
    _bgPattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bgPattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _patternClipper->addChild(_bgPattern);
    
    _circleGradient = LayerRadialGradient::create();
    _circleGradient->setStartColor(bgColour);
    _circleGradient->setStartOpacity(0);
    _circleGradient->setEndColor(bgColour);
    _circleGradient->setEndOpacity(255);
    addChild(_circleGradient);
    
    _oomee = Sprite::create();
    _oomee->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _oomee->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    addChild(_oomee);
    
    _oomeeShadow = Sprite::create("res/OomeeHQ/oomee_hq_shadow.png");
    _oomeeShadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _oomeeShadow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _oomee->addChild(_oomeeShadow, -1);
    
    _kidCodeFrame = RoundedRectSprite::create();
    _kidCodeFrame->setTexture("res/decoration/white_1px.png");
    _kidCodeFrame->setCornerRadius(20);
    _kidCodeFrame->setAnchorPoint(Vec2(-0.1f, 1.15f));
    _kidCodeFrame->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    _kidCodeFrame->setContentSize(kKidCodeFrameSize);
    addChild(_kidCodeFrame);
    
    _kidCodeBody = RoundedRectSprite::create();
    _kidCodeBody->setTexture("res/decoration/white_1px.png");
    _kidCodeBody->setCornerRadius(20);
    _kidCodeBody->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _kidCodeBody->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _kidCodeBody->setContentSize(kKidCodeFrameSize - kKidCodeFramePadding);
    _kidCodeBody->setColor(bgColour);
    _kidCodeFrame->addChild(_kidCodeBody);
    
    _kidCodeTitle = DynamicText::create(_("Kid Code"), Style::Font::PoppinsBold(), 49);
    _kidCodeTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _kidCodeTitle->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    _kidCodeTitle->setTextColor(Color4B::WHITE);
    _kidCodeTitle->setTextAreaSize(Size(_kidCodeBody->getContentSize().width * 0.86f, _kidCodeBody->getContentSize().height * 0.44f));
    _kidCodeTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _kidCodeTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _kidCodeTitle->setOverflow(Label::Overflow::SHRINK);
    _kidCodeBody->addChild(_kidCodeTitle);
    
    _kidCode = DynamicText::create("", Style::Font::PoppinsBold(), 63);
    _kidCode->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _kidCode->setTextVerticalAlignment(TextVAlignment::TOP);
    _kidCode->setTextColor(Color4B::WHITE);
    _kidCode->setTextAreaSize(_kidCodeTitle->getTextAreaSize());
    _kidCode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _kidCode->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _kidCode->setOverflow(Label::Overflow::SHRINK);
    _kidCodeBody->addChild(_kidCode);
    
    _imgDownloader = ImageDownloader::create(ImageDownloader::kImageCachePath, ImageDownloader::CacheMode::File);
    
    return true;
}

void OomeeDisplay::onEnter()
{
    Super::onEnter();
}

void OomeeDisplay::onExit()
{
    _imgDownloader->setDelegate(nullptr);
    Super::onExit();
}

void OomeeDisplay::onSizeChanged()
{
    Super::onSizeChanged();
    const Size& contentSize = getContentSize();
    const Size& patternRadiusSize = contentSize * 1.2f;
    _background->setContentSize(contentSize);
    _bgPattern->setContentSize(contentSize);
    _patternClipper->setContentSize(contentSize);
    _circleGradient->setContentSize(contentSize);
    _circleGradient->setCenter(Vec2(contentSize / 2.0f));
    _circleGradient->setRadius(MIN(patternRadiusSize.height / 2.0f, patternRadiusSize.width / 2.0f));
    _stencil->setPosition(_circleGradient->getCenter());
    _stencil->setContentSize(Size(_circleGradient->getRadius() * 2.0f, _circleGradient->getRadius() * 2.0f) - Size(4,4)); // bring in by couple of px to prevent artifats
    resizeOomee();
    
}

void OomeeDisplay::setKidCode(const std::string& kidCode)
{
    _kidCode->setString(kidCode);
}

void OomeeDisplay::setBgPatternColour(const cocos2d::Color3B& colour)
{
    _bgPattern->setColor(colour);
    _kidCodeFrame->setColor(colour);
}

void OomeeDisplay::setOomeeImgUrl(const std::string& oomeeImageUrl)
{
    _imgDownloader->downloadImage(this, oomeeImageUrl);
}

void OomeeDisplay::resizeOomee()
{
    const Size& contentSize = getContentSize();
    _oomee->setScale(MIN((contentSize.width * 0.8f) / _oomee->getContentSize().width, (contentSize.height * 0.8f) / _oomee->getContentSize().height));
    _oomeeShadow->setScale((_oomee->getContentSize().width * 0.7f) / _oomeeShadow->getContentSize().width);
}

//deleagte functions
void OomeeDisplay::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    Director::getInstance()->getTextureCache()->reloadTexture(downloader->getLocalImagePath());
    _oomee->setTexture(downloader->getLocalImagePath());
    resizeOomee();
}

void OomeeDisplay::onImageDownloadFailed()
{
    
}

NS_AZOOMEE_END
