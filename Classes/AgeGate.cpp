//
//  AgeGate.cpp
//  Azoomee
//
//  Created by Macauley on 08/07/2019.
//

#include "AgeGate.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string AgeGate::kAgeGateLayerName = "AgeGate";

bool AgeGate::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setName(kAgeGateLayerName);
	
	const Size& contentSize = Director::getInstance()->getVisibleSize();
	setContentSize(contentSize);
	setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	setBackGroundColorType(BackGroundColorType::SOLID);
	setBackGroundColor(Color3B::BLACK);
	setBackGroundColorOpacity(150);
	
	setSwallowTouches(true);
	setTouchEnabled(true);
	
	Size messageBoxSize = Size(1124,1815);  //target size
	const Size& maxSize = Size(contentSize.width * 0.82f, contentSize.height * 0.7f);
	
	const float scaleFactor = MIN(maxSize.width / messageBoxSize.width, maxSize.height / messageBoxSize.height);
	
	//messageBoxSize = messageBoxSize * scaleFactor;
	
	_messageBoxLayout = ui::Layout::create();
	_messageBoxLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_messageBoxLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_messageBoxLayout->setContentSize(messageBoxSize);
	_messageBoxLayout->setScale(scaleFactor);
	this->addChild(_messageBoxLayout);
	
	_oomeeLeft = ui::ImageView::create("res/onboarding/oomee53.png");
	_oomeeLeft->setRotation(-10);
	_oomeeLeft->setScale((messageBoxSize.width / 1.6f) / _oomeeLeft->getContentSize().width);
	_oomeeLeft->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_oomeeLeft->setNormalizedPosition(Vec2(0.18f,1.02f));
	_messageBoxLayout->addChild(_oomeeLeft);
	
	_oomeeRight = ui::ImageView::create("res/onboarding/oomee45.png");
	_oomeeRight->setRotation(10);
	_oomeeRight->setScale((messageBoxSize.width / 1.6f) / _oomeeRight->getContentSize().width);
	_oomeeRight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_oomeeRight->setNormalizedPosition(Vec2(0.82f,1.02f));
	_messageBoxLayout->addChild(_oomeeRight);
	
	_oomeeMiddle = ui::ImageView::create("res/onboarding/oomee20.png");
	_oomeeMiddle->setScale((messageBoxSize.width / 1.6f) / _oomeeMiddle->getContentSize().width);
	_oomeeMiddle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_oomeeMiddle->setNormalizedPosition(Vec2(0.5f,1.04f));
	_messageBoxLayout->addChild(_oomeeMiddle);
	
	_bgBox = ui::Layout::create();
	_bgBox->setBackGroundImage("res/onboarding/rounded_rect_45px.png");
	_bgBox->setBackGroundImageScale9Enabled(true);
	//_bgBox->setClippingEnabled(true);
	_bgBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_bgBox->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_bgBox->setContentSize(Size(messageBoxSize.width, messageBoxSize.height * 0.72f));
	_messageBoxLayout->addChild(_bgBox);
	
	ui::Scale9Sprite* stencil = ui::Scale9Sprite::create("res/onboarding/rounded_rect_45px.png");
	stencil->setContentSize(_bgBox->getContentSize());
	stencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	stencil->setPosition(_bgBox->getContentSize() / 2.0f);
	ClippingNode* clipNode = ClippingNode::create(stencil);
	clipNode->setContentSize(_bgBox->getContentSize());
	clipNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	clipNode->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	clipNode->setAlphaThreshold(0.5f);
	_bgBox->addChild(clipNode);
	
	Sprite* pattern = Sprite::create("res/decoration/main_pattern_small.png");
	pattern->setAnchorPoint(Vec2(0.5, 0.6));
	pattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	pattern->setColor(Color3B(13, 40, 78));
	clipNode->addChild(pattern);
	
	LayerGradient* gradient = LayerGradient::create(Color4B::WHITE, Color4B(255,255,255,100), Vec2(0,-1));
	gradient->setContentSize(Size(_bgBox->getContentSize().width, _bgBox->getContentSize().height * 0.66));
	gradient->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
	clipNode->addChild(gradient);
	
	_titleBox = ui::ImageView::create("res/onboarding/pink_rectangle.png");
	_titleBox->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
	_titleBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_titleBox->setScale((messageBoxSize.width * 0.66f) / _titleBox->getContentSize().width);
	_bgBox->addChild(_titleBox);
	
	ui::Text* text = ui::Text::create(_("For grown ups"), Style::Font::Bold(), 90);
	text->setTextAreaSize(_titleBox->getContentSize() * 0.85f);
	text->setTextHorizontalAlignment(TextHAlignment::CENTER);
	text->setTextVerticalAlignment(TextVAlignment::CENTER);
	text->setTextColor(Color4B::WHITE);
	text->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	Label* label = dynamic_cast<Label*>(text->getVirtualRenderer());
	if(label)
	{
		label->setOverflow(Label::Overflow::SHRINK);
	}
	_titleBox->addChild(text);
	
	_contentBody = ui::Layout::create();
	_contentBody->setContentSize(_bgBox->getContentSize() * 0.75f);
	_contentBody->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_contentBody->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_contentBody->setLayoutType(Type::VERTICAL);
	_bgBox->addChild(_contentBody);
	
	ui::Text* instructionText = ui::Text::create(_("Please answer this question to continue"), Style::Font::Regular(), 58);
	instructionText->setTextAreaSize(Size(_contentBody->getContentSize().width * 0.9f,134));
	instructionText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	instructionText->setTextVerticalAlignment(TextVAlignment::CENTER);
	instructionText->setTextColor(Color4B(130,130,130,255));
	instructionText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	instructionText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	instructionText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	Label* instructionTextlabel = dynamic_cast<Label*>(instructionText->getVirtualRenderer());
	if(instructionTextlabel)
	{
		instructionTextlabel->setOverflow(Label::Overflow::SHRINK);
	}
	_contentBody->addChild(instructionText);
	
	const int val1Set[4] = {4,5,6,7};
	const int val2Set[3] = {3,4,5};
	
	srand(time(NULL));
	
	int val1 = val1Set[rand() % 4];
	int val2 = val2Set[rand() % 3];
	int targetVal =  val1 * val2;
	
	int randAns1 = val1Set[rand() % 4] * val2Set[rand() % 3];
	while(randAns1 == targetVal)
	{
		randAns1 = val1Set[rand() % 4] * val2Set[rand() % 3];
	}
	int randAns2 = val1Set[rand() % 4] * val2Set[rand() % 3];
	while(randAns2 == targetVal || randAns2 == randAns1)
	{
		randAns2 = val1Set[rand() % 4] * val2Set[rand() % 3];
	}
	
	std::vector<int> ans = {targetVal, randAns1, randAns2};
	std::random_shuffle(ans.begin(), ans.end());
	
	ui::Text* questionText = ui::Text::create(StringUtils::format(_("What is %d X %d").c_str(),val1, val2), Style::Font::Bold(), 80);
	questionText->setTextAreaSize(Size(_contentBody->getContentSize().width * 0.9f,100));
	questionText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	questionText->setTextVerticalAlignment(TextVAlignment::CENTER);
	questionText->setTextColor(Color4B::BLACK);
	questionText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	questionText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	questionText->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,50)));
	Label* questionTextlabel = dynamic_cast<Label*>(questionText->getVirtualRenderer());
	if(questionTextlabel)
	{
		questionTextlabel->setOverflow(Label::Overflow::SHRINK);
	}
	_contentBody->addChild(questionText);
	
	for(int i = 0; i < ans.size(); i++)
	{
		int value = ans.at(i);
		ui::Button* ansButton = ui::Button::create("res/onboarding/rounded_button.png");
		ansButton->setScale((_contentBody->getContentSize().width * 0.9f) / ansButton->getContentSize().width);
		ansButton->setColor(Color3B(13, 40, 78));
		ansButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		ansButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
		ansButton->addTouchEventListener([this, value, targetVal](Ref* pSender, ui::Widget::TouchEventType eType){
			if(eType == ui::Widget::TouchEventType::ENDED)
			{
				if(targetVal == value)
				{
					if(_callback)
					{
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
		
		ui::Text* valText = ui::Text::create(StringUtils::format("%d",value), Style::Font::Bold(), ansButton->getContentSize().height * 0.5f);
		valText->setTextAreaSize(ansButton->getContentSize() * 0.8f);
		valText->setTextHorizontalAlignment(TextHAlignment::CENTER);
		valText->setTextVerticalAlignment(TextVAlignment::CENTER);
		valText->setTextColor(Color4B::WHITE);
		valText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
		valText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		Label* valTextlabel = dynamic_cast<Label*>(valText->getVirtualRenderer());
		if(valTextlabel)
		{
			valTextlabel->setOverflow(Label::Overflow::SHRINK);
		}
		ansButton->addChild(valText);
	}
	
	_closeButton = ui::Button::create("res/onboarding/close.png");
	_closeButton->setAnchorPoint(Vec2(-0.3,1.2));
	_closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_closeButton->ignoreContentAdaptWithSize(false);
	_closeButton->setContentSize(Size(messageBoxSize.width * 0.09f,messageBoxSize.width * 0.09f));
	_closeButton->setColor(Color3B(245, 2, 109));
	_closeButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_callback)
			{
				_callback(AgeGateResult::CLOSE);
			}
			else
			{
				this->removeFromParent();
			}
		}
	});
	_bgBox->addChild(_closeButton);
	
	return true;
}
void AgeGate::onEnter()
{
	Super::onEnter();
}
void AgeGate::onExit()
{
	Super::onExit();
}

void AgeGate::onSizeChanged()
{
	Super::onSizeChanged();
	const Size& contentSize = Director::getInstance()->getVisibleSize();
	setContentSize(contentSize);
	
	Size messageBoxSize = Size(1124,1815);  //target size
	const Size& maxSize = Size(contentSize.width * 0.82f, contentSize.height * 0.7f);
	
	const float scaleFactor = MIN(maxSize.width / messageBoxSize.width, maxSize.height / messageBoxSize.height);
	if(_messageBoxLayout)
	{
		_messageBoxLayout->setScale(scaleFactor);
	}
}

void AgeGate::setActionCompletedCallback(const ActionCompletedCallback& callback)
{
	_callback = callback;
}

NS_AZOOMEE_END
