#include "TitleBarWidget.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>


using namespace cocos2d;



NS_AZOOMEE_CHAT_BEGIN

bool TitleBarWidget::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Style::Color::dark);
    setLayoutType(ui::Layout::Type::RELATIVE);
    
    // Back button
    _backButton = ui::Button::create("res/chat/ui/buttons/back_button.png");
    // Enable content adaption - otherwise % size doesn't work
    _backButton->ignoreContentAdaptWithSize(false);
    _backButton->setSizePercent(Vec2(0.0f, 0.624f));
    _backButton->setSizeType(ui::Widget::SizeType::PERCENT);
    _backButton->getRendererNormal()->setStrechEnabled(true);
    _backButton->getRendererClicked()->setStrechEnabled(true);
    _backButton->getRendererDisabled()->setStrechEnabled(true);
    _backButton->setLayoutParameter(CreateLeftCenterRelativeLayoutParam(ui::Margin(55.0f, 0.0f, 0.0f, 0.0f)));
    addChild(_backButton);
    
    // Title
    _titleLabel = ui::Text::create();
    _titleLabel->setFontName(Style::Font::Regular);
    _titleLabel->setFontSize(126.0f);
    _titleLabel->setLayoutParameter(CreateCenterRelativeLayoutParam());
    addChild(_titleLabel);
    
    // Alert
    _alertButton = ui::Button::create("res/chat/ui/buttons/alert.png");
    // Enable content adaption - otherwise % size doesn't work
    _alertButton->ignoreContentAdaptWithSize(false);
    _alertButton->setSizePercent(_backButton->getSizePercent());
    _alertButton->setSizeType(ui::Widget::SizeType::PERCENT);
    _alertButton->getRendererNormal()->setStrechEnabled(true);
    _alertButton->getRendererClicked()->setStrechEnabled(true);
    _alertButton->getRendererDisabled()->setStrechEnabled(true);
    _alertButton->setLayoutParameter(CreateRightCenterRelativeLayoutParam(ui::Margin(0.0f, 0.0f, 55.0f, 0.0f)));
    addChild(_alertButton);
    // Hidden by default
    _alertButton->setVisible(false);
    
    // Add a drop shadpw
    createDropShadow(0.04f);
  
    return true;
}

void TitleBarWidget::onEnter()
{
    Super::onEnter();
}

void TitleBarWidget::onExit()
{
    Super::onExit();
}

void TitleBarWidget::onSizeChanged()
{
    // Ensure % size buttons keeps their aspect ratio
    // TODO: Find a way to make this automatic
    SetSizePercentWidthForSquareAspectRatio(_backButton);
    SetSizePercentWidthForSquareAspectRatio(_alertButton);
    
    Super::onSizeChanged();
}

#pragma mark - Public

void TitleBarWidget::setTitleString(const std::string& title)
{
    _titleLabel->setString(title);
}

void TitleBarWidget::setTitleColor(const cocos2d::Color3B& color)
{
    _titleLabel->setTextColor(Color4B(color));
}

void TitleBarWidget::showAlertButton(bool enable)
{
    _alertButton->setVisible(enable);
}

void TitleBarWidget::addBackButtonEventListener(const cocos2d::ui::Widget::ccWidgetClickCallback& callback)
{
    _backButton->addClickEventListener(callback);
}

#pragma mark - UI Creation

//Color3B GetOpaqueColorFromNormalBlend(const Color4F& source, const Color4F& bgColor = Color4F::WHITE)
//{
//    Color4F targetColorNormalised;
//    targetColorNormalised.r = ((1.0f - source.a) * bgColor.r) + (source.a * source.r);
//    targetColorNormalised.g = ((1.0f - source.a) * bgColor.g) + (source.a * source.g);
//    targetColorNormalised.b = ((1.0f - source.a) * bgColor.b) + (source.a * source.b);
//    return Color3B(targetColorNormalised);
//}

void TitleBarWidget::createDropShadow(float heightPercent)
{
//    // Calculate drop shadow color until we use an image
//    Color4B dropShadowColor = Color4B(48, 30, 52, 35);
//    Color3B dropShadowOpaqueColor = GetOpaqueColorFromNormalBlend(Color4F(dropShadowColor), Color4F::WHITE);
//    cocos2d::log("Color: %d, %d, %d", dropShadowOpaqueColor.r, dropShadowOpaqueColor.g, dropShadowOpaqueColor.b);
    
    const Color3B& dropShadowOpaqueColor = Color3B(76, 73, 77);
    const GLubyte dropShadowOpacity = 255 * 0.5f;
    
    // Add a drop shadow that sits outside the titlebar below it
    // Because cocos doesn't have % margins, we use an "adapator" to position the shadow by a % margin
    // This saves us from having to adjust the margins on every layout call.
    ui::Layout* dropShadowMargin = ui::Layout::create();
    dropShadowMargin->setSizeType(ui::Widget::SizeType::PERCENT);
    dropShadowMargin->setSizePercent(Vec2(1.0f, heightPercent));
    dropShadowMargin->setLayoutParameter(CreateBottomCenterRelativeLayoutParam());
    addChild(dropShadowMargin);
    
    ui::Layout* dropShadow = ui::Layout::create();
    dropShadow->setAnchorPoint(Vec2(0.0f, 1.0f));
    dropShadow->setSizeType(ui::Widget::SizeType::PERCENT);
    dropShadow->setSizePercent(Vec2(1.0f, 1.0f));
    // Note position % only works when the parent is not using a layoutType
    dropShadow->setPositionType(ui::Widget::PositionType::PERCENT);
    dropShadow->setPositionPercent(Vec2(0.0f, 0.0f));
    dropShadow->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    dropShadow->setBackGroundColor(dropShadowOpaqueColor);
    dropShadow->setBackGroundColorOpacity(dropShadowOpacity);
    dropShadowMargin->addChild(dropShadow);
}

NS_AZOOMEE_CHAT_END
