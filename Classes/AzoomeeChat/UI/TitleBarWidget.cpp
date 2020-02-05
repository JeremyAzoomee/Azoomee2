#include "TitleBarWidget.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>

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
    
    setLayoutType(ui::Layout::Type::RELATIVE);
    
    _background = RoundedRectSprite::create();
    _background->setTexture("res/hqscene/chat_banner_bg.png");
    _background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _background->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _background->setCornerRadius(23);
    _background->setRoundedCorners(false, false, true, true);
    _background->setStretchImageEnabled(true);
    addChild(_background);
    
    _bannerShadow = LayerGradient::create();
    _bannerShadow->setStartColor(Style::Color::darkIndigoThree);
    _bannerShadow->setStartOpacity(0);
    _bannerShadow->setEndColor(Style::Color::darkIndigoThree);
    _bannerShadow->setEndOpacity(255);
    _bannerShadow->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _bannerShadow->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
    addChild(_bannerShadow);
    
    // Back button
    _backButton = ui::Button::create("res/hqscene/episode_select_close.png");
    // Enable content adaption - otherwise % size doesn't work
    _backButton->ignoreContentAdaptWithSize(false);
    _backButton->setSizePercent(Vec2(0.0f, 0.624f));
    _backButton->setSizeType(ui::Widget::SizeType::PERCENT);
    _backButton->getRendererNormal()->setStretchEnabled(true);
    _backButton->getRendererClicked()->setStretchEnabled(true);
    _backButton->getRendererDisabled()->setStretchEnabled(true);
    _backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _backButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    addChild(_backButton);
    
    _titleLayout = ui::Layout::create();
    _titleLayout->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _titleLayout->setSizePercent(Vec2(0.5f, 1.0f));
    _titleLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    addChild(_titleLayout);
    
    // Title
    _titleLabel = ui::Text::create();
    _titleLabel->setFontName(Style::Font::PoppinsBold());
    _titleLabel->enableShadow(Color4B(0,0,0,125), Size(4,-8));
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
    _reportedChatTitleBar->setBackGroundColor(Style::Color::strongPink);
    _reportedChatTitleBar->setVisible(false);
    reportChatTitleBarHolder->addChild(_reportedChatTitleBar);
    
    _warningImageLeft = ui::ImageView::create("res/chat/ui/message/flagged.png");
    _warningImageLeft->setAnchorPoint(Vec2(0.5f, 0.5f));
    _reportedChatTitleBar->addChild(_warningImageLeft);
    
    _warningImageRight = ui::ImageView::create("res/chat/ui/message/flagged.png");
    _warningImageRight->setAnchorPoint(Vec2(0.5f, 0.5f));
    _reportedChatTitleBar->addChild(_warningImageRight);
    
    _warningLabel = Label::createWithTTF(_("This chat has been reported. Get your parent to reset it."), Style::Font::Regular(), 64);
    _warningLabel->setTextColor(Color4B(Style::Color::black));
    _warningLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    _reportedChatTitleBar->addChild(_warningLabel);
    
    // Reset Reported Chat Button
    _reportResetButton = ui::Button::create("res/chat/ui/buttons/reset_button.png");
    // TODO: Get from Strings
    _reportResetButton->setTitleText(_("Reset"));
    _reportResetButton->setTitleColor(Style::Color::black);
    _reportResetButton->setTitleFontName(Style::Font::PoppinsMedium());
    _reportResetButton->setTitleFontSize(45.0f);
    _reportResetButton->setScale9Enabled(true);
    _reportResetButton->setTitleAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    _reportResetButton->setLayoutParameter(CreateTopRightRelativeLayoutParam(ui::Margin(0.0f, 20.0f, 20.0f, 0.0f)));
    _reportResetButton->setVisible(false);
	_reportResetButton->setContentSize(Size(_reportResetButton->getTitleLabel()->getContentSize().width + 40, _reportResetButton->getContentSize().height));
	_reportResetButton->ignoreContentAdaptWithSize(false);
    addChild(_reportResetButton);
    
    // Add friend button
    _reportButton = ui::Button::create("res/chat/ui/buttons/report_button_outline.png");
    // TODO: Get from Strings
    _reportButton->setTitleText(_("Report"));
    _reportButton->setTitleColor(Style::Color::white);
    _reportButton->setTitleFontName(Style::Font::PoppinsMedium());
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
    const float lineHeightOffset = -0.0f;
    _reportButton->setLayoutParameter(CreateTopRightRelativeLayoutParam(ui::Margin(0.0f, 20.0f, 20.0f, 0.0f)));
    _reportButton->setVisible(false);
	//_reportButton->ignoreContentAdaptWithSize(false);
	_reportButton->setContentSize(Size(addFriendButtonSize.height + _reportButton->getTitleLabel()->getContentSize().width + 45.0f,_reportButton->getContentSize().height));
	_reportButton->getTitleRenderer()->setPosition(Vec2(plusIcon->getPositionX() + (plusIcon->getContentSize().width * 0.5f) + 15.0f, (addFriendButtonSize.height * 0.5f) + lineHeightOffset));
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
    
    _background->setContentSize(contentSize);
    _bannerShadow->setContentSize(Size(contentSize.width, contentSize.height * 0.5f));
    // Size the title layout to fill the remaining space
    _titleLayout->setContentSize(Size(contentSize.width - (_backButton->getContentSize().width * 2) - (kTitleButtonsEdgePadding * 2), contentSize.height));
    
    _titleLayout->forceDoLayout();
    
    if(_subTitleBarBorder)
    {
        _subTitleBarBorder->setContentSize(Size(contentSize.width, 6.0f));
    }
    
    //Set correct sizes for Reported Chat Bar
    onSizeChangedReportedBar(contentSize);
    
    Super::onSizeChanged();
}

void TitleBarWidget::onSizeChangedReportedBar(const Size& contentSize)
{
    // Get max width, to analyse if changes needed.
    float maxReportLabelWidth = contentSize.width - _warningImageRight->getContentSize().width*2 - kTitleButtonsEdgePadding*4;
    
    _warningLabel->setString(_("This chat has been reported. Get your parent to reset it."));
    _warningLabel->setBMFontSize(64);
    if(_warningLabel->getContentSize().width > maxReportLabelWidth)
    {
        //Add Label over 2 lines
		_warningLabel->setWidth(maxReportLabelWidth);
        _warningLabel->setBMFontSize(54);
    }

    float reportedBarHeight = _warningLabel->getContentSize().height + kTitleButtonsEdgePadding;
    _reportedChatTitleBar->setContentSize(Size(contentSize.width,reportedBarHeight));
    _warningImageLeft->setPosition(Vec2(kTitleButtonsEdgePadding+_warningImageLeft->getContentSize().width/2,reportedBarHeight/2));
    _warningImageRight->setPosition(Vec2(contentSize.width-kTitleButtonsEdgePadding - _warningImageRight->getContentSize().width/2,reportedBarHeight/2));
    _warningLabel->setPosition(Vec2(contentSize.width/2,reportedBarHeight/2));
}

void TitleBarWidget::updateTitleLayout()
{
    _titleLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    
    _titleLabel->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _titleImage->setLayoutParameter(CreateCenterRelativeLayoutParam());
}

#pragma mark - Public

void TitleBarWidget::setTitleString(const std::string& title)
{
    _titleLabel->setString(title);
    _titleLabel->setFontSize(126.0f);
    
    _titleLabel->setVisible(true);
    _titleImage->setVisible(false);
    
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
    
    updateTitleLayout();
}

void TitleBarWidget::setTitleAvatar(const FriendRef& friendData)
{
    _titleLabel->setString(friendData->friendName());
    setTitleColor(Style::Color::white);
    _titleLabel->setFontSize(80.0f);

    _titleLabel->setVisible(true);
    _titleImage->setVisible(false);
    
    updateTitleLayout();
}

void TitleBarWidget::underlineTitleBar()
{
    // Border at bottom
    _subTitleBarBorder = ui::Layout::create();
    _subTitleBarBorder->setLayoutParameter(CreateBottomCenterRelativeLayoutParam());
    _subTitleBarBorder->setSizeType(ui::Widget::SizeType::ABSOLUTE);
    _subTitleBarBorder->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _subTitleBarBorder->setBackGroundColor(Style::Color::macaroniAndCheese);
    addChild(_subTitleBarBorder);
}

void TitleBarWidget::addBackButtonEventListener(const cocos2d::ui::Widget::ccWidgetClickCallback& callback)
{
    _backButton->addClickEventListener(callback);
}

void TitleBarWidget::addReportButtonEventListener(const cocos2d::ui::Widget::ccWidgetClickCallback& callback)
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
    //Set Report Bar and Reset to Visible
    _reportButton->setVisible(false);
    _reportResetButton->setVisible(!_chatReportingIsForbidden);
    _reportedChatTitleBar->setVisible(true);
}

void TitleBarWidget::setChatToActive()
{
    //Hide Report Bar, and sent Report Button to visible
    _reportButton->setVisible(!_chatReportingIsForbidden);
    _reportResetButton->setVisible(false);
    _reportedChatTitleBar->setVisible(false);
}

void TitleBarWidget::setChatReportingToForbidden()
{
    _reportButton->setVisible(false);
    _reportResetButton->setVisible(false);
    _chatReportingIsForbidden = true;
}

void TitleBarWidget::onChatActivityHappened()
{
    if(_chatReportingIsForbidden) return;
    
    //If the chat is reported (_reportedChatTitleBar->isVisible()) Do NOTHING.
    if(!_reportedChatTitleBar->isVisible()) setChatToActive();
}

NS_AZOOMEE_CHAT_END
