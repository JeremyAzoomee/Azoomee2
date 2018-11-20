//
//  LanguageSelectScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 01/11/2018.
//

#include "LanguageSelectScene.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include "SceneManagerScene.h"
#include "BackEndCaller.h"
#include "LoginLogicHandler.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::vector<std::pair<cocos2d::Color4B, cocos2d::Color4B>> LanguageSelectScene::kGradientList = {
	{Color4B(0,61,224,255), Color4B(2,150,248,255)},
	{Color4B(255,109,4,255), Color4B(255,175,30,255)},
	{Color4B(107,56,186,255), Color4B(168,94,220,255)},
	{Color4B(0,132,140,255), Color4B(21,229,183,255)},
	{Color4B(206,3,8,255), Color4B(255,74,99,255)}
};

bool LanguageSelectScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	createUI();
	
	return true;
}

void LanguageSelectScene::onEnter()
{
	Super::onEnter();
}

void LanguageSelectScene::onExit()
{
	Super::onExit();
}

void LanguageSelectScene::onSizeChanged()
{
	Super::onSizeChanged();
	
	removeAllChildrenWithCleanup(true);
	
	createUI();
}

void LanguageSelectScene::createUI()
{
	const Size& contentSize = this->getContentSize();
	const bool isPortrait = contentSize.width < contentSize.height;
	
	
	addBackground();
	
	float scrollHeight = contentSize.height - (isPortrait ? 445 : 100);
	
	_contentLayout = ui::Layout::create();
	_contentLayout->setContentSize(contentSize);
	_contentLayout->setLayoutType(ui::Layout::Type::VERTICAL);
	_contentLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_contentLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	this->addChild(_contentLayout);
	
	Label* title = Label::createWithTTF(_("Pick your language"), Style::Font::Regular(), 90);
	title->setWidth(contentSize.width * 0.8f);
	title->setTextColor(Color4B::WHITE);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setVerticalAlignment(TextVAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,isPortrait ? 200 : 100,0,0)));
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	_contentLayout->addChild(titleHolder);
	
	scrollHeight -= titleHolder->getContentSize().height + (isPortrait ? 200 : 100);
	
	addLanguageScrollView();
	
	scrollHeight -= 100;
	
	_scrollButton = ui::Button::create("res/languageSelect/toggle_button_white.png");
	_scrollButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	_scrollButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_scrollButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_scrollButton->getRotation() >= 180)
			{
				_scrollButton->setRotation(0);
				_languageScrollView->scrollToTop(0.5f, true);
			}
			else
			{
				_languageScrollView->scrollToBottom(0.5f, true);
				_scrollButton->setRotation(180);
			}
		}
	});
	_contentLayout->addChild(_scrollButton);
	
	scrollHeight -= _scrollButton->getContentSize().height + 100;
	
	_languageScrollView->setContentSize(Size(_languageScrollView->getContentSize().width, scrollHeight));
	_languageScrollView->getParent()->setContentSize(_languageScrollView->getContentSize());
}

void LanguageSelectScene::addBackground()
{
	const Size& contentSize = this->getContentSize();
	bool isPortrait = contentSize.height > contentSize.width;
	
	LayerColor* bgColour = LayerColor::create(Color4B::BLACK, contentSize.width, contentSize.height);
	this->addChild(bgColour);
	
	auto wireLeft = Sprite::create(StringUtils::format("res/childSelection/wireLeft%s.png", isPortrait ? "_portrait" : ""));
	wireLeft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	wireLeft->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	wireLeft->setScale(contentSize.height / wireLeft->getContentSize().height);
	this->addChild(wireLeft);
	
	auto wireRight = Sprite::create(StringUtils::format("res/childSelection/wireRight%s.png", isPortrait ? "_portrait" : ""));
	wireRight->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	wireRight->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	wireRight->setScale(contentSize.height / wireRight->getContentSize().height);
	this->addChild(wireRight);
	
	auto leftOomee = Sprite::create("res/languageSelect/green_oomee_glasses.png");
	leftOomee->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	leftOomee->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
	this->addChild(leftOomee);
	
	auto rightOomee = Sprite::create("res/languageSelect/custom_oomie_2.png");
	rightOomee->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	rightOomee->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
	this->addChild(rightOomee);
}

void LanguageSelectScene::addLanguageScrollView()
{
	const Size& contentSize = _contentLayout->getContentSize();
	bool isPortrait = contentSize.height > contentSize.width;
	
	int langsPerRow = isPortrait ? 2 : 3;
	
	_languageScrollView = ui::ListView::create();
	_languageScrollView->setContentSize(Size(contentSize.width * 0.8f,contentSize.height - 700));
	_languageScrollView->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_languageScrollView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_languageScrollView->setBounceEnabled(true);
	_languageScrollView->setItemsMargin(isPortrait ? 150 : 100);
	_languageScrollView->setTopPadding(100);
	_languageScrollView->setBottomPadding(100);
	
	const auto& langsData = StringMgr::kLanguageParams;
	
	int i = 0;
	while(i < langsData.size())
	{
		ui::Layout* itemRow = ui::Layout::create();
		itemRow->setLayoutType(ui::Layout::Type::ABSOLUTE);
		itemRow->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
		itemRow->setContentSize(Size(0, 0));
		
		for(int column = 0; column < langsPerRow; column++)
		{
			if(i < langsData.size())
			{
				ui::Layout* button = createLanguageButton(langsData.at(i), i % kGradientList.size());
				button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
				button->setNormalizedPosition(Vec2((column + 0.5) / langsPerRow, 0.5));
				itemRow->addChild(button);
				itemRow->setContentSize(Size(_languageScrollView->getContentSize().width, button->getContentSize().height));
				i++;
			}
		}
		_languageScrollView->pushBackCustomItem(itemRow);
	}
	
	ui::Layout* langSVHolder = ui::Layout::create();
	langSVHolder->setContentSize(_languageScrollView->getContentSize());
	langSVHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	langSVHolder->addChild(_languageScrollView);
	
	Sprite* verticalScrollGradientTop = Sprite::create("res/decoration/TopNavGrad.png");
	verticalScrollGradientTop->setAnchorPoint(Vec2(0.5, 1.0));
	verticalScrollGradientTop->setScaleX(langSVHolder->getContentSize().width / verticalScrollGradientTop->getContentSize().width);
	verticalScrollGradientTop->setColor(Color3B::BLACK);
	verticalScrollGradientTop->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
	langSVHolder->addChild(verticalScrollGradientTop);
	
	Sprite* verticalScrollGradientBot = Sprite::create("res/decoration/TopNavGrad.png");
	verticalScrollGradientBot->setAnchorPoint(Vec2(0.5, 1.0));
	verticalScrollGradientBot->setRotation(180);
	verticalScrollGradientBot->setScaleX(langSVHolder->getContentSize().width / verticalScrollGradientBot->getContentSize().width);
	verticalScrollGradientBot->setColor(Color3B::BLACK);
	verticalScrollGradientBot->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	langSVHolder->addChild(verticalScrollGradientBot);
	
	_contentLayout->addChild(langSVHolder);
	
}

cocos2d::ui::Layout* LanguageSelectScene::createLanguageButton(const LanguageParams& params, int colourIndex)
{
	ui::Layout* langButton = ui::Layout::create();
	langButton->setLayoutType(ui::Layout::Type::VERTICAL);
	
	Sprite* stencil = Sprite::create("res/languageSelect/Speech_Bubble_White.png");
	stencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	ClippingNode* bubble = ClippingNode::create(stencil);
	bubble->setAlphaThreshold(0.5f);
	bubble->setContentSize(stencil->getContentSize());
	
	LayerGradient* gradient = LayerGradient::create(kGradientList.at(colourIndex).first, kGradientList.at(colourIndex).second, Vec2(0.0f,1.0f));
	gradient->setContentSize(stencil->getContentSize());
	bubble->addChild(gradient);
	
	const auto& font = params._identifier == "gre" ? Style::Font::ArialRegular : Style::Font::SofiaRegular;
	
	Label* helloText = Label::createWithTTF(params._text, font, 75);
	helloText->setTextColor(Color4B::WHITE);
	helloText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	helloText->setNormalizedPosition(Vec2(0.5f,0.85f));
	gradient->addChild(helloText);
	
	ui::Layout* bubbleHolder = ui::Layout::create();
	bubbleHolder->setContentSize(bubble->getContentSize());
	bubbleHolder->addChild(bubble);
	langButton->addChild(bubbleHolder);
	
	Label* nameLabel = Label::createWithTTF(params._name, font, 59);
	nameLabel->setTextColor(Color4B::WHITE);
	nameLabel->setOpacity(200);
	nameLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	nameLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* nameHolder = ui::Layout::create();
	nameHolder->setContentSize(nameLabel->getContentSize());
	nameHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,10,0,0)));
	nameHolder->addChild(nameLabel);
	langButton->addChild(nameHolder);
	
	langButton->setContentSize(bubbleHolder->getContentSize() + Size(0, nameHolder->getContentSize().height + 10));
	langButton->setTouchEnabled(true);
	langButton->addTouchEventListener([params](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			StringMgr::getInstance()->changeLanguage(params._identifier);
			if(ConfigStorage::getInstance()->shouldShowFirstSlideShowScene())
			{
				if(ParentDataProvider::getInstance()->isLoggedInParentAnonymous())
				{
					Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
				}
				else
				{
					BackEndCaller::getInstance()->anonymousDeviceLogin();
				}
			}
			else
			{
				if(ParentDataProvider::getInstance()->isLoggedInParentAnonymous())
				{
					Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
				}
				else
				{
					LoginLogicHandler::getInstance()->doLoginLogic();
				}
			}
		}
	});
	
	return langButton;
	
}

NS_AZOOMEE_END
