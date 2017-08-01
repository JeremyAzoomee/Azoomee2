#include "TitleBarWidget.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/Strings.h>

using namespace cocos2d;



// TODO: From config
const float kTitleButtonsEdgePadding = 55.0f;



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
    _backButton->setLayoutParameter(CreateLeftCenterRelativeLayoutParam(ui::Margin(kTitleButtonsEdgePadding, 0.0f, 0.0f, 0.0f)));
    addChild(_backButton);
    
    _titleLayout = ui::Layout::create();
    _titleLayout->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _titleLayout->setSizePercent(Vec2(0.5f, 1.0f));
    _titleLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    addChild(_titleLayout);
    
    // Avatar
    _avatarWidget = AvatarWidget::create();
    _avatarWidget->setAnchorPoint(Vec2(0.5f, 0.5f));
    _avatarWidget->setVisible(false);
    _titleLayout->addChild(_avatarWidget);
    
    // Title
    _titleLabel = ui::Text::create();
    _titleLabel->setFontName(Style::Font::Regular);
    _titleLayout->addChild(_titleLabel);
    
    // Image
    _titleImage = ui::ImageView::create();
    _titleImage->setVisible(false);
    _titleLayout->addChild(_titleImage);

    // Message Is Reported Title Bar Including images and text
    // Hidden by default
    ui::Layout* reportChatTitleBarHolder = ui::Layout::create();
    reportChatTitleBarHolder->setSizeType(ui::Widget::SizeType::ABSOLUTE);
    reportChatTitleBarHolder->setLayoutParameter(CreateBottomCenterRelativeLayoutParam());
    reportChatTitleBarHolder->setContentSize(Size(getContentSize().width,100));
    addChild(reportChatTitleBarHolder);
    
    _reportedChatTitleBar = ui::Layout::create();
    _reportedChatTitleBar->setAnchorPoint(Vec2(0.5f, 1.0f));
    _reportedChatTitleBar->setSizeType(ui::Widget::SizeType::ABSOLUTE);
    _reportedChatTitleBar->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _reportedChatTitleBar->setBackGroundColor(Style::Color::watermelon);
    _reportedChatTitleBar->setVisible(false);
    reportChatTitleBarHolder->addChild(_reportedChatTitleBar);
    
    _warningImageLeft = ui::ImageView::create("res/chat/ui/message/flagged.png");
    _warningImageLeft->setAnchorPoint(Vec2(0.5f, 0.5f));
    _reportedChatTitleBar->addChild(_warningImageLeft);
    
    _warningImageRight = ui::ImageView::create("res/chat/ui/message/flagged.png");
    _warningImageRight->setAnchorPoint(Vec2(0.5f, 0.5f));
    _reportedChatTitleBar->addChild(_warningImageRight);
    
    _warningLabel = createLabelWith(StringMgr::getInstance()->getStringForKey(CHAT_CHAT_REPORTED), Style::Font::Regular, Style::Color::black, 64);
    _warningLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    _reportedChatTitleBar->addChild(_warningLabel);
    
    // Reset Reported Chat Button
    _reportResetButton = ui::Button::create("res/chat/ui/buttons/reset_button.png");
    // TODO: Get from Strings
    _reportResetButton->setTitleText("Reset");
    _reportResetButton->setTitleColor(Style::Color::black);
    _reportResetButton->setTitleFontName(Style::Font::Regular);
    _reportResetButton->setTitleFontSize(45.0f);
    _reportResetButton->setScale9Enabled(true);
    _reportResetButton->setTitleAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _reportResetButton->setLayoutParameter(CreateRightCenterRelativeLayoutParam(ui::Margin(0.0f, 0.0f, kTitleButtonsEdgePadding, 0.0f)));
    _reportResetButton->setVisible(false);
    addChild(_reportResetButton);
    
    // Add friend button
    _reportButton = ui::Button::create("res/chat/ui/buttons/report_button_outline.png");
    // TODO: Get from Strings
    _reportButton->setTitleText("Report");
    _reportButton->setTitleColor(Style::Color::brightAqua);
    _reportButton->setTitleFontName(Style::Font::Regular);
    _reportButton->setTitleFontSize(45.0f);
    _reportButton->setScale9Enabled(true);
    
    _reportButton->setTitleAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    
    ui::ImageView* plusIcon = ui::ImageView::create("res/chat/ui/buttons/report_icon.png");
    _reportButton->addChild(plusIcon);
    plusIcon->setAnchorPoint(Vec2(0.5f, 0.5f));
    // Position icon and title
    const auto& addFriendButtonSize = _reportButton->getContentSize();
    plusIcon->setPosition(Vec2(addFriendButtonSize.height * 0.5f, addFriendButtonSize.height * 0.5f));
    _reportButton->getTitleRenderer()->setAnchorPoint(Vec2(0.0f, 0.5f));
    // We need some offset because the title doesn't get centered vertically correctly
    // Likely due to font renderering via TTF
    const float lineHeightOffset = -3.0f;
   _reportButton->getTitleRenderer()->setPosition(Vec2(plusIcon->getPositionX() + (plusIcon->getContentSize().width * 0.5f) + 15.0f, (addFriendButtonSize.height * 0.5f) + lineHeightOffset));
    _reportButton->setLayoutParameter(CreateRightCenterRelativeLayoutParam(ui::Margin(0.0f, 0.0f, kTitleButtonsEdgePadding, 0.0f)));
    _reportButton->setVisible(false);
    addChild(_reportButton);
    
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
    
    const Size& contentSize = getContentSize();
    
    // Size the title layout to fill the remaining space
    _titleLayout->setContentSize(Size(contentSize.width - (_backButton->getContentSize().width * 2) - (kTitleButtonsEdgePadding * 2), contentSize.height));
    
    // Resize the avatar appropriately
    const float avatarHeightPct = 0.6f;
    const float avatarSize = contentSize.height * avatarHeightPct;
    _avatarWidget->setContentSize(Size(avatarSize, avatarSize));
    _titleLayout->forceDoLayout();
    
    //Set correct sizes for Reported Chat Bar
    onSizeChangedReportedBar(contentSize);
    
    Super::onSizeChanged();
}

void TitleBarWidget::onSizeChangedReportedBar(const Size& contentSize)
{
    // Get max width, to analyse if changes needed.
    float maxReportLabelWidth = contentSize.width - _warningImageRight->getContentSize().width*2 + kTitleButtonsEdgePadding*4;
    
    _warningLabel->setString(StringMgr::getInstance()->getStringForKey(CHAT_CHAT_REPORTED));
    _warningLabel->setBMFontSize(64);
    
    if(_warningLabel->getContentSize().width > maxReportLabelWidth)
    {
        //Add Label over 2 lines
        _warningLabel->setBMFontSize(54);
        _warningLabel->setString(StringMgr::getInstance()->getStringForKey(CHAT_CHAT_REPORTED_MULTILINE));
    }

    float reportedBarHeight = _warningLabel->getContentSize().height + kTitleButtonsEdgePadding;
    _reportedChatTitleBar->setContentSize(Size(contentSize.width,reportedBarHeight));
    _warningImageLeft->setPosition(Vec2(kTitleButtonsEdgePadding+_warningImageLeft->getContentSize().width/2,reportedBarHeight/2));
    _warningImageRight->setPosition(Vec2(contentSize.width-kTitleButtonsEdgePadding - _warningImageRight->getContentSize().width/2,reportedBarHeight/2));
    _warningLabel->setPosition(Vec2(contentSize.width/2,reportedBarHeight/2));
}

void TitleBarWidget::updateTitleLayout()
{
    if(_avatarWidget->isVisible())
    {
        _titleLayout->setLayoutType(ui::Layout::Type::HORIZONTAL);
        
        _avatarWidget->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(85.0f, 0.0f, 0.0f, 0.0f)));
        _titleLabel->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(30.0f, 0.0f, 0.0f, 0.0f)));
    }
    else
    {
        _titleLayout->setLayoutType(ui::Layout::Type::RELATIVE);
        
        _titleLabel->setLayoutParameter(CreateCenterRelativeLayoutParam());
        _titleImage->setLayoutParameter(CreateCenterRelativeLayoutParam());
    }
}

#pragma mark - Public

void TitleBarWidget::setTitleString(const std::string& title)
{
    _titleLabel->setString(title);
    _titleLabel->setFontSize(126.0f);
    
    _titleLabel->setVisible(true);
    _titleImage->setVisible(false);
    _avatarWidget->setVisible(false);
    
    updateTitleLayout();
}

void TitleBarWidget::setTitleColor(const cocos2d::Color3B& color)
{
    _titleLabel->setTextColor(Color4B(color));
}

void TitleBarWidget::setTitleImage(const std::string& imagePath)
{
    _titleImage->loadTexture(imagePath);
    
    _titleLabel->setVisible(false);
    _titleImage->setVisible(true);
    _avatarWidget->setVisible(false);
    
    updateTitleLayout();
}

void TitleBarWidget::setTitleAvatar(const FriendRef& friendData)
{
    _titleLabel->setString(friendData->friendName());
    setTitleColor(Style::Color::brightAqua);
    _titleLabel->setFontSize(80.0f);
    _avatarWidget->setAvatarForFriend(friendData);
    
    _titleLabel->setVisible(true);
    _titleImage->setVisible(false);
    _avatarWidget->setVisible(true);
    
    updateTitleLayout();
}

void TitleBarWidget::addBackButtonEventListener(const cocos2d::ui::Widget::ccWidgetClickCallback& callback)
{
    _backButton->addClickEventListener(callback);
}

void TitleBarWidget::addAlertButtonEventListener(const cocos2d::ui::Widget::ccWidgetClickCallback& callback)
{
    _reportButton->addClickEventListener(callback);
}

void TitleBarWidget::addReportResetButtonEventListener(const cocos2d::ui::Widget::ccWidgetClickCallback& callback)
{
    _reportResetButton->addClickEventListener(callback);
}

#pragma mark - Chat Reporting Functions

void TitleBarWidget::setChatToInModeration()
{
    if(chatReportingIsForbidden) return;
    
    //Set Report Bar and Reset to Visible
    _reportButton->setVisible(false);
    _reportResetButton->setVisible(true);
    _reportedChatTitleBar->setVisible(true);
}

void TitleBarWidget::setChatToActive()
{
    if(chatReportingIsForbidden) return;
    
    //Hide Report Bar, and sent Report Button to visible
    _reportButton->setVisible(true);
    _reportResetButton->setVisible(false);
    _reportedChatTitleBar->setVisible(false);
}

void TitleBarWidget::setChatReportingToForbidden()
{
    setChatToActive();
    _reportButton->setVisible(false);
    chatReportingIsForbidden = true;
}

void TitleBarWidget::onChatActivityHappened()
{
    if(chatReportingIsForbidden) return;
    
    //If the chat is reported (_reportedChatTitleBar->isVisible()) Do NOTHING.
    if(!_reportedChatTitleBar->isVisible()) setChatToActive();
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
