#include "ForceUpdateAppLockScene.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "ForceUpdateSingleton.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ForceUpdateAppLockScene::init()
{
    if ( !Super::init() )
    {
        return false;
    }
    
    ui::Layout* bgColour = ui::Layout::create();
    bgColour->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    bgColour->setBackGroundColor(Style::Color::darkIndigo);
    bgColour->setSizeType(ui::Layout::SizeType::PERCENT);
    bgColour->setSizePercent(Vec2(1.0f,1.0f));
    addChild(bgColour);
    
    _bgPattern = RoundedRectSprite::create();
    _bgPattern->setTexture("res/decoration/pattern_stem_tile.png");
    _bgPattern->setColor(Style::Color::azure);
    _bgPattern->setRoundedCorners(false, false, false, false);
    _bgPattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bgPattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _bgPattern->setScaleMode(RoundedRectSprite::ScaleMode::TILE);
    addChild(_bgPattern);
    
    _bgGradient = LayerGradient::create();
    _bgGradient->setStartColor(Style::Color::darkIndigo);
    _bgGradient->setStartOpacity(155);
    _bgGradient->setEndColor(Style::Color::darkIndigo);
    _bgGradient->setEndOpacity(0);
    _bgGradient->setVector(Vec2(0,1));
    _bgGradient->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
    _bgGradient->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    addChild(_bgGradient);
    
    const float padding = 20.0f;
    
    _textLayout = ui::Layout::create();
    _textLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    _textLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _textLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    addChild(_textLayout);

    _headerText = DynamicText::create(_("Azoomee needs to be updated!"), Style::Font::PoppinsBold(), 102);
    _headerText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _headerText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _headerText->setTextColor(Color4B::WHITE);
    _headerText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _headerText->setMaxLineWidth(1000);
    _textLayout->addChild(_headerText);
    
    _bodyText = DynamicText::create(_("It looks like you are using an old version of Azoomee.\nTo continue using Azoomee, please ask a grown-up to update it to the latest version by tapping the button below."), Style::Font::PoppinsBold(), 50);
    _bodyText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _bodyText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _bodyText->setTextColor(Color4B::WHITE);
    _bodyText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,padding,0,padding)));
    _bodyText->setMaxLineWidth(1000);
    _textLayout->addChild(_bodyText);
    
    _updateButton = CTAButton::create("res/onboarding/rounded_button.png");
    _updateButton->ignoreContentAdaptWithSize(false);
    _updateButton->setScale9Enabled(true);
    _updateButton->setContentSize(Size(600,140));
    _updateButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _updateButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,padding,0,0)));
    _updateButton->setColor(Style::Color::white);
    _updateButton->setTextColour(Color4B::BLACK);
    _updateButton->setTextFontInfo(Style::Font::PoppinsBold(), 66);
    _updateButton->setTextAreaSizePercent(Vec2(0.9f,0.8f));
    _updateButton->setText(_("Update"));
    _updateButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            Application::getInstance()->openURL(ForceUpdateSingleton::getInstance()->getUpdateUrlFromFile());
        }
    });
    _textLayout->addChild(_updateButton);
    
    _textLayout->setContentSize(Size(0, _headerText->getContentSize().height + _bodyText->getContentSize().height + _updateButton->getContentSize().height + (3 * padding)));

    return true;
}

void ForceUpdateAppLockScene::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    
    _bgPattern->setContentSize(contentSize);
    _bgGradient->setContentSize(contentSize);
    
    _headerText->setMaxLineWidth(contentSize.width * 0.7f);
    _bodyText->setMaxLineWidth(contentSize.width * 0.7f);
    
    const float padding = 20;
    _textLayout->setContentSize(Size(0, _headerText->getContentSize().height + _bodyText->getContentSize().height + _updateButton->getContentSize().height + (3 * padding)));
}

void ForceUpdateAppLockScene::onExit()
{
    Director::getInstance()->replaceScene(ForceUpdateAppLockScene::create()); //prevent this scene to be removed by any others.
    Super::onExit();
}

NS_AZOOMEE_END
