//
//  PopupMessageBox.cpp
//  Azoomee
//
//  Created by Macauley on 22/07/2019.
//

#include "PopupMessageBox.h"
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string PopupMessageBox::kPopupMessageBoxName = "messageBox";
const float PopupMessageBox::kPopupSize = 1096.0f;
const float PopupMessageBox::kTitleBarHeight = 416.0f;
const cocos2d::Size PopupMessageBox::kButtonSize = cocos2d::Size(700.0f, 140.0f);
const float PopupMessageBox::kPopupPadding = 70.0f;
const float PopupMessageBox::kButtonSpacing = 35.0f;
const float PopupMessageBox::kContentBodyPadding = 70.0f;

bool PopupMessageBox::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setName(kPopupMessageBoxName);
    
    // Message box root is full screen, with a semi transparent black background
	setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0f, 1.0f));
	
	setBackGroundColorType(BackGroundColorType::SOLID);
	setBackGroundColor(Color3B::BLACK);
	setBackGroundColorOpacity(150);
	
	setSwallowTouches(true);
	setTouchEnabled(true);
	
    createMessageBox();
	
	return true;
}

void PopupMessageBox::onEnter()
{
	Super::onEnter();
	
    updatePopupSize();
}

void PopupMessageBox::onExit()
{
	Super::onExit();
}

void PopupMessageBox::onSizeChanged()
{
	Super::onSizeChanged();
    
    if(isRunning())
    {
        updatePopupSize();
    }
}

void PopupMessageBox::updatePopupSize()
{
    const bool hasTwoButtons = _secondActionButton->isVisible();
    
    // Make the popup slightly bigger if it has a second button
    const Size& messageBoxSize = Size(kPopupSize, kPopupSize + (hasTwoButtons ? kButtonSize.height + kButtonSpacing : 0));
    const Size& messageBoxPadding = Size((kPopupPadding * 2), (kPopupPadding * 2));
    const Size& messageBoxSizeWithPadding = messageBoxSize + messageBoxPadding;
    
    const Size& contentSize = Director::getInstance()->getVisibleSize();
    const Size& maxSize = Size(MIN(contentSize.width * 0.95f, messageBoxSizeWithPadding.width), MIN(contentSize.height * 0.95f, messageBoxSizeWithPadding.height));
    const float scaleFactor = MIN(maxSize.width / messageBoxSizeWithPadding.width, maxSize.height / messageBoxSizeWithPadding.height);
    
    _messageBoxBg->setContentSize(messageBoxSizeWithPadding - Size(4, 4)); //bring in a tad to prevent stray overhanging pixels
    _messageBoxBg->setScale(scaleFactor);
    
    _messageBoxStencil->setContentSize(messageBoxSizeWithPadding);
    _messageBoxStencil->setPosition(messageBoxSize * 0.5f);
    _messageBoxClipper->setContentSize(messageBoxSize);
    _messageBoxClipper->setScale(scaleFactor);
    _messageBoxLayout->setContentSize(messageBoxSize);
    
    _titleBox->setContentSize(Size(messageBoxSize.width, kTitleBarHeight));
    _titleGradient->setContentSize(_titleBox->getContentSize());
    _titleText->setTextAreaSize(_titleBox->getContentSize() * 0.8f);
    
    // Calculate content body height
    const Size contentBodySize(messageBoxSize.width - (kContentBodyPadding * 2), messageBoxSize.height - kTitleBarHeight - kContentBodyPadding);
    _contentBody->setContentSize(contentBodySize);
    
    // Calculate height of the text
    float spaceForButtons = kButtonSize.height + kButtonSpacing;
    if(hasTwoButtons)
    {
        spaceForButtons += kButtonSize.height + kButtonSpacing;
    }
    float availableHeightForText = contentBodySize.height - spaceForButtons;
    
    _contentText->setTextAreaSize(Size(contentBodySize.width, availableHeightForText));
}

void PopupMessageBox::createMessageBox()
{
    _messageBoxBg = ui::Scale9Sprite::create("res/onboarding/rounded_rect_45px.png");
    _messageBoxBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _messageBoxBg->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    addChild(_messageBoxBg);
    
    _messageBoxStencil = ui::Scale9Sprite::create("res/onboarding/rounded_rect_45px.png");
    _messageBoxStencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _messageBoxClipper = ClippingNode::create(_messageBoxStencil);
    _messageBoxClipper->setAlphaThreshold(0.5f);
    _messageBoxClipper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _messageBoxClipper->setIgnoreAnchorPointForPosition(false);
    _messageBoxClipper->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    addChild(_messageBoxClipper);
    
    _messageBoxLayout = ui::Layout::create();
    _messageBoxLayout->setLayoutType(Type::VERTICAL);
    _messageBoxLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _messageBoxLayout->setBackGroundColor(Color3B::WHITE);
    _messageBoxClipper->addChild(_messageBoxLayout);
    
    createTitle();
    createBody();
}

void PopupMessageBox::createTitle()
{
    _titleBox = ui::Layout::create();
    _titleBox->setBackGroundColorType(BackGroundColorType::SOLID);
    _titleBox->setBackGroundColor(Style::Color::darkIndigo);
    _titleBox->setClippingEnabled(true);
    _titleBox->setLayoutParameter(CreateTopLinearLayoutParam());
    _messageBoxLayout->addChild(_titleBox);
    
    _titlePattern = ui::ImageView::create("res/decoration/main_pattern_small.png");
    _titlePattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _titlePattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _titlePattern->setColor(Style::Color::azure);
    _titleBox->addChild(_titlePattern);
    
    _titleGradient = LayerGradient::create();
    _titleGradient->setStartColor(Style::Color::darkIndigo);
    _titleGradient->setStartOpacity(0);
    _titleGradient->setEndColor(Style::Color::darkIndigo);
    _titleGradient->setEndOpacity(90);
    _titleGradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _titleGradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _titleGradient->setIgnoreAnchorPointForPosition(false);
    _titleBox->addChild(_titleGradient);
    
    _titleText = DynamicText::create("", Style::Font::PoppinsBold(), 110);
    _titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _titleText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _titleText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _titleText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _titleText->setColor(Color3B::WHITE);
    _titleText->setOverflow(Label::Overflow::SHRINK);
    _titleBox->addChild(_titleText);
}

void PopupMessageBox::createBody()
{
    _contentBody = ui::Layout::create();
    _contentBody->setBackGroundColorType(BackGroundColorType::SOLID);
    _contentBody->setBackGroundColor(Color3B::WHITE);
    _contentBody->setLayoutParameter(CreateTopLinearLayoutParam(cocos2d::ui::Margin(kContentBodyPadding, 0, kContentBodyPadding, kContentBodyPadding)));
    _contentBody->setLayoutType(Type::VERTICAL);
    _messageBoxLayout->addChild(_contentBody);
    
    _contentText = DynamicText::create("", Style::Font::PoppinsRegular(), 50);
    _contentText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _contentText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _contentText->setTextColor(Color4B(Style::Color::brownGrey));
    _contentText->setOverflow(Label::Overflow::SHRINK);
    _contentText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(cocos2d::ui::Margin(0, 0, 0, 0)));
    _contentBody->addChild(_contentText);
    
    _actionButton = CTAButton::create("res/onboarding/rounded_button.png");
    _actionButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(cocos2d::ui::Margin(0, 0, 0, 0)));
    _actionButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _actionButton->setScale9Enabled(true);
    _actionButton->ignoreContentAdaptWithSize(false);
    _actionButton->setContentSize(kButtonSize);
    _actionButton->setColor(Style::Color::darkIndigo);
    _actionButton->setTextFontInfo(Style::Font::PoppinsBold(), 70);
    _actionButton->setTextColour(Color4B::WHITE);
    _actionButton->setTextAreaSizePercent(Vec2(0.8f,0.8f));
    _actionButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_callback)
            {
                _callback(this);
            }
        }
    });
    _contentBody->addChild(_actionButton);
    
    _secondActionButton = CTAButton::create("res/onboarding/rounded_button.png");
    _secondActionButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(cocos2d::ui::Margin(0, kButtonSpacing, 0, 0)));
    _secondActionButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _secondActionButton->ignoreContentAdaptWithSize(false);
    _secondActionButton->setScale9Enabled(true);
    _secondActionButton->setContentSize(kButtonSize);
    _secondActionButton->setColor(Style::Color::darkIndigo);
    _secondActionButton->setTextFontInfo(Style::Font::PoppinsBold(), 70);
    _secondActionButton->setTextColour(Color4B::WHITE);
    _secondActionButton->setTextAreaSizePercent(Vec2(0.8f,0.8f));
    _secondActionButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_secondCallback)
            {
                _secondCallback(this);
            }
        }
    });
    _contentBody->addChild(_secondActionButton);
    _secondActionButton->setVisible(false);
}

void PopupMessageBox::setTitle(const std::string& title)
{
	_titleText->setString(title);
}

void PopupMessageBox::setBody(const std::string& body)
{
	_contentText->setString(body);
}

void PopupMessageBox::setPatternColour(const Color3B& colour)
{
    _titlePattern->setColor(colour);
}


void PopupMessageBox::setButtonText(const std::string& buttonText)
{
	_actionButton->setText(buttonText);
}

void PopupMessageBox::setButtonColour(const Color3B& colour)
{
	_actionButton->setColor(colour);
}

void PopupMessageBox::setButtonPressedCallback(const ButtonPressedCallback& callback)
{
    _callback = callback;
}

void PopupMessageBox::setSecondButtonText(const std::string& buttonText)
{
    _secondActionButton->setText(buttonText);
    _secondActionButton->setVisible(buttonText != "");
    
    if(isRunning())
    {
        updatePopupSize();
    }
}

void PopupMessageBox::setSecondButtonColour(const cocos2d::Color3B& colour)
{
    _secondActionButton->setColor(colour);
}

void PopupMessageBox::setSecondButtonPressedCallback(const ButtonPressedCallback& callback)
{
    _secondCallback = callback;
}

NS_AZOOMEE_END
