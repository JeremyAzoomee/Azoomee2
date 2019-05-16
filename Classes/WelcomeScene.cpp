//
//  WelcomeScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 22/01/2019.
//

#include "WelcomeScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Tutorial/TutorialController.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "SceneManagerScene.h"
#include "BackEndCaller.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool WelcomeScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& contentSize = this->getContentSize();
	bool isPortrait = contentSize.height > contentSize.width;
	
	_bgColour = LayerColor::create(Color4B(0,7,4,255), contentSize.width, contentSize.height);
	this->addChild(_bgColour);

	_tilesNode = Node::create();
	_tilesNode->setContentSize(contentSize);
	_tilesNode->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_tilesNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(_tilesNode);
	
	addAnimatedTiles();
	
	_bottomGradient = Sprite::create("res/decoration/TopNavGrad.png");
	_bottomGradient->setContentSize(Size(contentSize.width, 400));
	_bottomGradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_bottomGradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_bottomGradient->setColor(Style::Color::skyBlue);
	_bottomGradient->setRotation(180);
	this->addChild(_bottomGradient);

	_body = ui::Layout::create();
	_body->setLayoutType(ui::Layout::Type::VERTICAL);
	_body->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_body->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	this->addChild(_body);
	
	_logo = ui::ImageView::create("res/introAssets/Azoomee_WHITE.png");
	_logo->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_logo->setOpacity(0);
	_logo->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5), FadeIn::create(1.5f)));
	_body->addChild(_logo);
	
	_textHolder = ui::Layout::create();
	_textHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
	_body->addChild(_textHolder);
	
	_text = Label::createWithTTF(_("introText"), Style::Font::Bold(), 60);
	_text->setWidth(contentSize.width * 0.8f);
	_text->setOverflow(Label::Overflow::RESIZE_HEIGHT);
	_text->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	_text->setTextColor(Color4B::WHITE);
	_text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_text->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_text->setOpacity(0);
	_text->setLineSpacing(20);
	_text->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0), FadeIn::create(1.5f)));
	_textHolder->setContentSize(_text->getContentSize());
	_textHolder->addChild(_text);
	
	_button = ui::Button::create("res/settings/rounded_button.png");
	_button->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
	_button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_button->setCascadeOpacityEnabled(true);
	_button->setOpacity(0);
	_button->ignoreContentAdaptWithSize(false);
	_button->setColor(Style::Color::skyBlue);
	_button->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5), FadeIn::create(1.5f)));
	_button->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			BackEndCaller::getInstance()->anonymousDeviceLogin();
		}
	});
	_body->addChild(_button);
	
	Label* buttonText = Label::createWithTTF(_("introButton"), Style::Font::Bold(), 70);
	buttonText->setTextColor(Color4B::WHITE);
	buttonText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	buttonText->setNormalizedPosition(Vec2(0.5f,0.5f));
	buttonText->setOverflow(Label::Overflow::SHRINK);
	buttonText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	_button->addChild(buttonText);
	_button->setContentSize(Size(_logo->getContentSize().width, _button->getContentSize().height * (_logo->getContentSize().width / _button->getContentSize().width)));
	buttonText->setDimensions(_button->getContentSize().width * 0.66, buttonText->getContentSize().height);
	
	_body->setContentSize(Size(contentSize.width, _logo->getContentSize().height + 100 + _text->getContentSize().height + _button->getContentSize().height));
	
	ui::Button* loginButton = ui::Button::create("res/settings/rounded_button.png");
	loginButton->setAnchorPoint(Vec2(1.15, 1.75f));
	loginButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	loginButton->setColor(Style::Color::skyBlue);
	loginButton->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
		}
	});
	
	this->addChild(loginButton);
	
	Label* loginText = Label::createWithTTF(_("Log in"), Style::Font::Regular(), 60);
	loginText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	loginText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	loginText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	loginText->setOverflow(Label::Overflow::SHRINK);
	loginText->setDimensions(loginButton->getContentSize().width * 0.7f, loginButton->getContentSize().height * 0.7f);
	loginText->setTextColor(Color4B::WHITE);
	loginButton->addChild(loginText);
	
	return true;
}

void WelcomeScene::onEnter()
{
	Super::onEnter();
}

void WelcomeScene::onExit()
{
	Super::onExit();
}

void WelcomeScene::onSizeChanged()
{
	Super::onSizeChanged();
	
	const Size& contentSize = this->getContentSize();
	
	_bgColour->setContentSize(contentSize);
	
	_text->setWidth(contentSize.width * 0.8f);
	_textHolder->setContentSize(_text->getContentSize());
	
	_bottomGradient->setContentSize(Size(contentSize.width, 400));
	
	_tilesNode->setContentSize(contentSize);
	_tilesNode->removeAllChildren();
	addAnimatedTiles();
	
	_body->setContentSize(Size(contentSize.width, _logo->getContentSize().height + 100 + _text->getContentSize().height + _button->getContentSize().height));
	_body->forceDoLayout();
}

void WelcomeScene::addAnimatedTiles()
{
	const Size& contentSize = this->getContentSize();
	const bool isPortrait = contentSize.width < contentSize.height;
	const float scrollDuration = 120.0f;
	const int numTiles = 20;
	std::vector<std::string> filenames;
	for(int i = 0; i < numTiles; i++)
	{
		filenames.push_back(StringUtils::format("%d.jpg",i));
	}

	Size tileSize = Size(683,510);
	const Vec2& padding = Vec2(30,30);
	const Vec2& gridSize = isPortrait ? Vec2(3,6) : Vec2(5,4);
	
	if(tileSize.width * gridSize.x < contentSize.width * 1.2f || tileSize.height * gridSize.y < contentSize.height * 1.2f)
	{
		tileSize = tileSize * MAX(contentSize.width * 1.2f / (tileSize.width * gridSize.x), contentSize.height * 1.2f / (tileSize.height * gridSize.y));
	}
	
	const Vec2& origin = Vec2(contentSize.width * 1.0f, 0);
	
	Node* gridNode = Node::create();
	gridNode->setContentSize(Size((tileSize.width * gridSize.x) + (padding.x * (gridSize.x)), (tileSize.height * gridSize.y) + (padding.y * (gridSize.y))));
	gridNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	//gridNode->setPosition(contentSize / 2.0f);
	gridNode->setRotation(-8);
	Vec2 move = Vec2(0 ,gridNode->getContentSize().width);
	move.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(-gridNode->getRotation() - 90));
	gridNode->setPosition(origin - move * 0.2f);
	gridNode->runAction(RepeatForever::create(Sequence::create(MoveTo::create(scrollDuration / 2.0f,origin + move * 0.8f), MoveTo::create(0,origin - move * 1.2f), MoveTo::create(scrollDuration / 2.0f, origin - move * 0.2f), NULL)));
	_tilesNode->addChild(gridNode);
	
	//LayerColor* overlay = LayerColor::create(Color4B(0,0,0,100));
	//_tilesNode->addChild(overlay);
	
	std::random_shuffle(filenames.begin(), filenames.end());
	for(int i = 0; i < MIN(gridSize.x * gridSize.y, filenames.size()); i++)
	{
		int row = i / gridSize.x;
		int col = i % (int)gridSize.x;
		
		const Vec2& pos = Vec2((tileSize.width * (col + 0.5f)) + (col * padding.x), (tileSize.height * (row + 0.5f)) + (row * padding.x) );
		
		Sprite* tile = Sprite::create("res/introAssets/tiles/" + filenames.at(i));
		tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		tile->setPosition(pos);
		tile->setContentSize(tileSize);
		tile->setOpacity(100);
		gridNode->addChild(tile);
	}
	
	Node* gridNode2 = Node::create();
	gridNode2->setContentSize(gridNode->getContentSize());
	gridNode2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	gridNode2->setPosition(origin - move * 1.2f);
	gridNode2->setRotation(gridNode->getRotation());
	//Vec2 move = Vec2(0 ,gridNode2->getContentSize().height);
	//move.rotate(Vec2(0,0), CC_DEGREES_TO_RADIANS(10));
	gridNode2->runAction(RepeatForever::create(Sequence::create(MoveTo::create(scrollDuration, origin + move * 0.8f), MoveTo::create(0,origin - move * 1.2f), NULL)));
	_tilesNode->addChild(gridNode2);
	
	//LayerColor* overlay = LayerColor::create(Color4B(0,0,0,100));
	//_tilesNode->addChild(overlay);
	
	std::random_shuffle(filenames.begin(), filenames.end());
	for(int i = 0; i < MIN(gridSize.x * gridSize.y, filenames.size()); i++)
	{
		int row = i / gridSize.x;
		int col = i % (int)gridSize.x;
		
		const Vec2& pos = Vec2((tileSize.width * (col + 0.5f)) + (col * padding.x), (tileSize.height * (row + 0.5f)) + (row * padding.x) );
		
		Sprite* tile = Sprite::create("res/introAssets/tiles/" + filenames.at(i));
		tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		tile->setPosition(pos);
		tile->setContentSize(tileSize);
		tile->setOpacity(100);
		gridNode2->addChild(tile);
	}
	
}

NS_AZOOMEE_END
