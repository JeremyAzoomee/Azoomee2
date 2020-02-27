//
//  AgeGate.cpp
//  Azoomee
//
//  Created by Macauley on 08/07/2019.
//

#include "AgeGate.h"
#include <TinizineCommon/Utils/LocaleManager.h>
#include <TinizineCommon/UI/Colour.h>
#include <TinizineCommon/UI/LayoutParams.h>
#include <TinizineCommon/Analytics/AnalyticsSingleton.h>
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

AgeGateValueSet::AgeGateValueSet()
{
	const int val1Set[4] = {4,5,6,7};
	const int val2Set[3] = {3,4,5};
	
	srand((int)time(NULL));
	
	_val1 = val1Set[rand() % 4];
	_val2 = val2Set[rand() % 3];
	_targetVal =  _val1 * _val2;
	
	int randAns1 = val1Set[rand() % 4] * val2Set[rand() % 3];
	while(randAns1 == _targetVal)
	{
		randAns1 = val1Set[rand() % 4] * val2Set[rand() % 3];
	}
	int randAns2 = val1Set[rand() % 4] * val2Set[rand() % 3];
	while(randAns2 == _targetVal || randAns2 == randAns1)
	{
		randAns2 = val1Set[rand() % 4] * val2Set[rand() % 3];
	}
	
	_answers = {_targetVal, randAns1, randAns2};
	std::random_shuffle(_answers.begin(), _answers.end());
	
}

const std::string AgeGate::kAgeGateLayerName = "AgeGate";

bool AgeGate::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setName(kAgeGateLayerName);
	
	setSizeType(SizeType::PERCENT);
	setSizePercent(Vec2(1.0f,1.0f));
	setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	setBackGroundColorType(BackGroundColorType::SOLID);
	setBackGroundColor(Color3B::BLACK);
	setBackGroundColorOpacity(150);
	
	setSwallowTouches(true);
	setTouchEnabled(true);
	
    createMessageBox();
    
	_closeButton = ui::Button::create("res/onboarding/close.png");
	_closeButton->setAnchorPoint(Vec2(-0.3,1.2));
	_closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_closeButton->ignoreContentAdaptWithSize(false);
	_closeButton->setContentSize(Size(120,120));
	_closeButton->setColor(Color3B::WHITE);
	_closeButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_callback)
			{
                AnalyticsSingleton::getInstance()->genericButtonPressEvent("AgeGate_Close");
				_callback(AgeGateResult::CLOSE);
			}
			else
			{
				this->removeFromParent();
			}
		}
	});
	_messageBoxLayout->addChild(_closeButton);
	
	return true;
}
void AgeGate::onEnter()
{
	Super::onEnter();
	
	_titleText->setTextAreaSize(_titleBox->getContentSize() * 0.9f);
	_titleGradient->setContentSize(_titleBox->getContentSize());
}
void AgeGate::onExit()
{
	Super::onExit();
}

void AgeGate::onSizeChanged()
{
	Super::onSizeChanged();
	const Size& contentSize = Director::getInstance()->getVisibleSize();
	
	const Size& messageBoxSizeWithPadding = Size(1236,1696);
    
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
}

void AgeGate::createMessageBox()
{
    const Size& contentSize = Director::getInstance()->getVisibleSize();
    
    const Size& messageBoxSize = Size(1096,1556);  //target size
    const Size& messageBoxSizeWithPadding = Size(1236,1696);
    const Size& maxSize = Size(MIN(contentSize.width * 0.95f, messageBoxSizeWithPadding.width), MIN(contentSize.height * 0.95f, messageBoxSizeWithPadding.height));
    
    const float scaleFactor = MIN(maxSize.width / messageBoxSizeWithPadding.width, maxSize.height / messageBoxSizeWithPadding.height);
    
    _messageBoxBg = ui::Scale9Sprite::create("res/onboarding/rounded_rect_45px.png");
    _messageBoxBg->setContentSize(messageBoxSizeWithPadding - Size(4,4));
    _messageBoxBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _messageBoxBg->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _messageBoxBg->setScale(scaleFactor);
    addChild(_messageBoxBg);
    
    ui::Scale9Sprite* stencil = ui::Scale9Sprite::create("res/onboarding/rounded_rect_45px.png");
    stencil->setContentSize(messageBoxSizeWithPadding);
    stencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    stencil->setPosition(messageBoxSize/ 2.0f);
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
void AgeGate::createTitle()
{
    _titleBox = ui::Layout::create();
    _titleBox->setSizeType(SizeType::PERCENT);
    _titleBox->setSizePercent(Vec2(1.0f, 0.27f));
    _titleBox->setBackGroundColorType(BackGroundColorType::SOLID);
    _titleBox->setBackGroundColor(Colours::Color_3B::darkIndigo);
    _titleBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _titleBox->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _titleBox->setClippingEnabled(true);
    _messageBoxLayout->addChild(_titleBox);
    
    _titlePattern = ui::ImageView::create("res/decoration/main_pattern_small.png");
    _titlePattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _titlePattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _titlePattern->setColor(Colours::Color_3B::strongPink);
    _titleBox->addChild(_titlePattern);
    
    _titleGradient = LayerGradient::create();
    _titleGradient->setStartColor(Colours::Color_3B::darkIndigo);
    _titleGradient->setStartOpacity(0);
    _titleGradient->setEndColor(Colours::Color_3B::darkIndigo);
    _titleGradient->setEndOpacity(90);
    _titleGradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _titleGradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _titleGradient->setIgnoreAnchorPointForPosition(false);
    _titleBox->addChild(_titleGradient);
    
    _titleText = DynamicText::create(_("Hey parents, unlock all content!"), Style::Font::PoppinsBold(), 110);
    _titleText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _titleText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _titleText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _titleText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _titleText->setColor(Color3B::WHITE);
    _titleText->setOverflow(Label::Overflow::SHRINK);
    _titleBox->addChild(_titleText);
    
}
void AgeGate::createBody()
{
    _contentBody = ui::Layout::create();
    _contentBody->setSizeType(SizeType::PERCENT);
    _contentBody->setSizePercent(Vec2(1.0f,0.73f));
    _contentBody->setBackGroundColorType(BackGroundColorType::SOLID);
    _contentBody->setBackGroundColor(Color3B::WHITE);
    _contentBody->setLayoutType(Type::VERTICAL);
    _messageBoxLayout->addChild(_contentBody);
    
    DynamicText* instructionText = DynamicText::create(_("Please answer this question to continue"), Style::Font::PoppinsRegular(), 53);
    instructionText->setTextAreaSize(Size(700,140));
    instructionText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    instructionText->setTextVerticalAlignment(TextVAlignment::CENTER);
    instructionText->setTextColor(Color4B(Colours::Color_3B::brownGrey));
    instructionText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    instructionText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,80,0,60)));
    instructionText->setOverflow(Label::Overflow::SHRINK);
    _contentBody->addChild(instructionText);
    
    auto ansValues = AgeGateValueSet();
    
    DynamicText* questionText = DynamicText::create(StringUtils::format(_("What is %d X %d?").c_str(),ansValues._val1, ansValues._val2), Style::Font::PoppinsBold(), 70);
    questionText->setTextAreaSize(Size(700,100));
    questionText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    questionText->setTextVerticalAlignment(TextVAlignment::CENTER);
    questionText->setTextColor(Color4B::BLACK);
    questionText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    questionText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,100)));
    questionText->setOverflow(Label::Overflow::SHRINK);
    _contentBody->addChild(questionText);
    
    for(int i = 0; i < ansValues._answers.size(); i++)
    {
        int value = ansValues._answers.at(i);
        TextButton* ansButton = TextButton::create("res/onboarding/rounded_button.png");
        ansButton->ignoreContentAdaptWithSize(false);
        ansButton->setContentSize(Size(700,140));
        ansButton->setColor(Colours::Color_3B::strongPink);
        ansButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        ansButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,60)));
        ansButton->setText(StringUtils::format("%d",value));
        ansButton->setTextFontInfo(Style::Font::PoppinsBold(), 70);
        ansButton->setTextColour(Color4B::WHITE);
        ansButton->addTouchEventListener([this, value, ansValues](Ref* pSender, ui::Widget::TouchEventType eType){
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                if(ansValues._targetVal == value)
                {
                    if(_callback)
                    {
                        AnalyticsSingleton::getInstance()->genericButtonPressEvent("AgeGate_Success");
                        _callback(AgeGateResult::SUCCESS);
                    }
                    else
                    {
                        this->removeFromParent();
                    }
                }
                else
                {
                    if(_callback)
                    {
                        AnalyticsSingleton::getInstance()->genericButtonPressEvent("AgeGate_Incorrect");
                        _callback(AgeGateResult::FAIL);
                    }
                    else
                    {
                        this->removeFromParent();
                    }
                }
            }
        });
        _contentBody->addChild(ansButton);
    }
}

void AgeGate::setActionCompletedCallback(const ActionCompletedCallback& callback)
{
	_callback = callback;
}

NS_AZ_END
