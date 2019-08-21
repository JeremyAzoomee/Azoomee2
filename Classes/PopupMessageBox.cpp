//
//  PopupMessageBox.cpp
//  Azoomee
//
//  Created by Macauley on 22/07/2019.
//

#include "PopupMessageBox.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string PopupMessageBox::kPopupMessageBoxName = "messageBox";

bool PopupMessageBox::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setName(kPopupMessageBoxName);

	setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	setSizeType(SizeType::PERCENT);
	setSizePercent(Vec2(1.0f,1.0f));
	
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
	
	_titleText->setTextAreaSize(_titleBox->getContentSize() * 0.8f);
	_contentText->setTextAreaSize(Size(_contentBody->getContentSize().width * 0.8f, _contentBody->getContentSize().height * 0.4f));
	_titleGradient->setContentSize(_titleBox->getContentSize());
}
void PopupMessageBox::onExit()
{
	Super::onExit();
}
void PopupMessageBox::onSizeChanged()
{
	Super::onSizeChanged();
	const Size& contentSize = Director::getInstance()->getVisibleSize();
	
	const Size& messageBoxSizeWithPadding = Size(1236,1236);
	const Size& maxSize = Size(MIN(contentSize.width * 0.95f, messageBoxSizeWithPadding.width), MIN(contentSize.height * 0.95f, messageBoxSizeWithPadding.height));
	
	const float scaleFactor = MIN(maxSize.width / messageBoxSizeWithPadding.width, maxSize.height / messageBoxSizeWithPadding.height);
	
	if(_messageBoxBg)
	{
		_messageBoxBg->setScale(scaleFactor);
	}
	if(_messageBoxClipper)
	{
		_messageBoxClipper->setScale(scaleFactor);
	}
	if(_titleText)
	{
		_titleText->setTextAreaSize(_titleBox->getContentSize() * 0.8f);
	}
	if(_contentText)
	{
		_contentText->setTextAreaSize(Size(_contentBody->getContentSize().width * 0.8f, _contentBody->getContentSize().height * 0.4f));
	}
	if(_titleGradient)
	{
		_titleGradient->setContentSize(_titleBox->getContentSize());
	}
}

void PopupMessageBox::createMessageBox()
{
    const Size& contentSize = Director::getInstance()->getVisibleSize();
    
    const Size& messageBoxSize = Size(1096,1096);  //target size
    const Size& messageBoxSizeWithPadding = Size(1236,1236);
    const Size& maxSize = Size(MIN(contentSize.width * 0.95f, messageBoxSizeWithPadding.width), MIN(contentSize.height * 0.95f, messageBoxSizeWithPadding.height));
    
    const float scaleFactor = MIN(maxSize.width / messageBoxSizeWithPadding.width, maxSize.height / messageBoxSizeWithPadding.height);
    
    _messageBoxBg = ui::Scale9Sprite::create("res/onboarding/rounded_rect_45px.png");
    _messageBoxBg->setContentSize(messageBoxSizeWithPadding - Size(4,4)); //bring in a tad to prevent stray overhanging pixels
    _messageBoxBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _messageBoxBg->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _messageBoxBg->setScale(scaleFactor);
    addChild(_messageBoxBg);
    
    ui::Scale9Sprite* stencil = ui::Scale9Sprite::create("res/onboarding/rounded_rect_45px.png");
    stencil->setContentSize(messageBoxSizeWithPadding);
    stencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    stencil->setPosition(messageBoxSize / 2.0f);
    _messageBoxClipper = ClippingNode::create(stencil);
    _messageBoxClipper->setAlphaThreshold(0.5f);
    _messageBoxClipper->setContentSize(messageBoxSize);
    _messageBoxClipper->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _messageBoxClipper->setIgnoreAnchorPointForPosition(false);
    _messageBoxClipper->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _messageBoxClipper->setScale(scaleFactor);
    addChild(_messageBoxClipper);
    
    _messageBoxLayout = ui::Layout::create();
    _messageBoxLayout->setSizeType(SizeType::PERCENT);
    _messageBoxLayout->setSizePercent(Vec2(1.0f,1.0f));
    _messageBoxLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _messageBoxLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _messageBoxLayout->setBackGroundColorType(BackGroundColorType::SOLID);
    _messageBoxLayout->setBackGroundColor(Color3B::WHITE);
    _messageBoxClipper->addChild(_messageBoxLayout);
    
    createTitle();
    
    createBody();
}
void PopupMessageBox::createTitle()
{
    _titleBox = ui::Layout::create();
    _titleBox->setSizeType(SizeType::PERCENT);
    _titleBox->setSizePercent(Vec2(1.0f, 0.38f));
    _titleBox->setBackGroundColorType(BackGroundColorType::SOLID);
    _titleBox->setBackGroundColor(Style::Color::darkIndigo);
    _titleBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _titleBox->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _titleBox->setClippingEnabled(true);
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
    _contentBody->setSizeType(SizeType::PERCENT);
    _contentBody->setSizePercent(Vec2(1.0f,0.62f));
    _contentBody->setBackGroundColorType(BackGroundColorType::SOLID);
    _contentBody->setBackGroundColor(Color3B::WHITE);
    _contentBody->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _contentBody->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _messageBoxLayout->addChild(_contentBody);
    
    _contentText = DynamicText::create("", Style::Font::PoppinsRegular(), 50);
    _contentText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentText->setNormalizedPosition(Vec2(0.5f,0.71f));
    _contentText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _contentText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _contentText->setTextColor(Color4B(Style::Color::brownGrey));
    _contentText->setOverflow(Label::Overflow::SHRINK);
    _contentBody->addChild(_contentText);
    
    _actionButton = CTAButton::create("res/onboarding/rounded_button.png");
    _actionButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _actionButton->setScale9Enabled(true);
    _actionButton->setNormalizedPosition(Vec2(0.5f,0.3f));
    _actionButton->ignoreContentAdaptWithSize(false);
    _actionButton->setContentSize(Size(700,140));
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
}

void PopupMessageBox::setButtonPressedCallback(const ButtonPressedCallback& callback)
{
	_callback = callback;
}

void PopupMessageBox::setTitle(const std::string& title)
{
	_titleText->setString(title);
}
void PopupMessageBox::setBody(const std::string& body)
{
	_contentText->setString(body);
}
void PopupMessageBox::setButtonText(const std::string& buttonText)
{
	_actionButton->setText(buttonText);
}
void PopupMessageBox::setButtonColour(const Color3B& colour)
{
	_actionButton->setColor(colour);
}
void PopupMessageBox::setPatternColour(const Color3B& colour)
{
	_titlePattern->setColor(colour);
}

NS_AZOOMEE_END
