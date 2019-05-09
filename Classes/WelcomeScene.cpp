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
	
	_wires = Sprite::create("res/rewards/big_wires.png");
	_wires->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_wires->setScale(MAX(contentSize.width, contentSize.height) / _wires->getContentSize().width);
	_wires->setRotation(isPortrait ? 90 : 0);
	_wires->setOpacity(65);
	this->addChild(_wires);
	
	_bottomGradient = Sprite::create("res/decoration/TopNavGrad.png");
	_bottomGradient->setContentSize(Size(contentSize.width, 400));
	_bottomGradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_bottomGradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_bottomGradient->setColor(Style::Color::skyBlue);
	_bottomGradient->setRotation(180);
	this->addChild(_bottomGradient);
	
	_tilesNode = Node::create();
	_tilesNode->setContentSize(contentSize);
	_tilesNode->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_tilesNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(_tilesNode);
	
	addAnimatedTiles();
	
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
	
	_text = Label::createWithTTF(_("introText"), Style::Font::Regular(), 90);
	_text->setWidth(contentSize.width * 0.6f);
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
	
	_button = ui::Button::create("res/buttons/blue_cta.png");
	_button->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_button->setScale9Enabled(true);
	_button->setCascadeOpacityEnabled(true);
	_button->setOpacity(0);
	_button->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5), FadeIn::create(1.5f)));
	_button->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			BackEndCaller::getInstance()->anonymousDeviceLogin();
		}
	});
	_body->addChild(_button);
	
	Label* buttonText = Label::createWithTTF(_("introButton"), Style::Font::Regular(), 80);
	buttonText->setTextColor(Color4B::WHITE);
	buttonText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	buttonText->setNormalizedPosition(Vec2(0.4f,0.5f));
	buttonText->setOverflow(Label::Overflow::SHRINK);
	buttonText->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	_button->addChild(buttonText);
	_button->setContentSize(Size(MIN(buttonText->getContentSize().width + 400, _button->getContentSize().width), _button->getContentSize().height));
	buttonText->setDimensions(_button->getContentSize().width * 0.66, buttonText->getContentSize().height);
	
	_body->setContentSize(Size(contentSize.width, _logo->getContentSize().height + 50 + _text->getContentSize().height + _button->getContentSize().height));
	
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
	bool isPortrait = contentSize.height > contentSize.width;
	
	_bgColour->setContentSize(contentSize);
	
	_wires->setScale(MAX(contentSize.width, contentSize.height) / _wires->getContentSize().width);
	_wires->setRotation(isPortrait ? 90 : 0);
	
	_text->setWidth(contentSize.width * 0.6f);
	_textHolder->setContentSize(_text->getContentSize());
	
	_bottomGradient->setContentSize(Size(contentSize.width, 400));
	
	_tilesNode->setContentSize(contentSize);
	_tilesNode->removeAllChildren();
	addAnimatedTiles();
	
	_body->setContentSize(Size(contentSize.width, _logo->getContentSize().height + 50 + _text->getContentSize().height + _button->getContentSize().height));
	_body->forceDoLayout();
}

void WelcomeScene::addAnimatedTiles()
{
	const Size& contentSize = this->getContentSize();
	
	const int numTiles = 10;
	const float travelDur = 120.0f;
	
	PointArray* points = PointArray::create(numTiles);
	Vec2 dir = Vec2(0.5f, 1.0f);
	
	//rotation intervals
	const std::vector<float>& angles = {
		CC_DEGREES_TO_RADIANS(-25.0f),//25
		CC_DEGREES_TO_RADIANS(-25.0f),//50
		CC_DEGREES_TO_RADIANS(-30.0f),//80
		CC_DEGREES_TO_RADIANS(-40.0f),//120
		CC_DEGREES_TO_RADIANS(-25.0f),//145
		CC_DEGREES_TO_RADIANS(-55.0f),//200
		CC_DEGREES_TO_RADIANS(-25.0f),//225
		CC_DEGREES_TO_RADIANS(-40.0f),//265
		CC_DEGREES_TO_RADIANS(-40.0f),//305
		CC_DEGREES_TO_RADIANS(-30.0f)//335
	};
	// tile scale at each path node
	const std::vector<float>& scales = {
		1.1f,
		1.1f,
		0.9f,
		1.1f,
		1.0f,
		1.2f,
		1.0f,
		1.2f,
		0.9f,
		1.0f
	};
	
	const Size& pathArea = Size(MAX(contentSize.width * 0.9f, 1710),MAX(contentSize.height * 0.9f, 1710));
	const Vec2& origin = Vec2((contentSize.width - pathArea.width) / 2.0f, (contentSize.height - pathArea.height) / 2.0f);
	for(int i = 0; i < numTiles; i++)
	{
		dir.rotate(Vec2(0.5f, 0.5f), angles.at(i));
		Vec2 pos = Vec2(origin.x + dir.x * pathArea.width, origin.y +  dir.y * pathArea.height);
		points->insertControlPoint(pos, i);
	}
	
	for(int i = 0; i < numTiles; i++)
	{
		PointArray* path = PointArray::create(numTiles + 1); // use numTiles + 1 so spline loops cleanly
		Vector<FiniteTimeAction*> scaleActions;
		for(int j = 0; j < (numTiles + 1); j++)
		{
			path->insertControlPoint(points->getControlPointAtIndex((i + 1 + j) % numTiles),j);
			scaleActions.pushBack(ScaleTo::create(travelDur/(numTiles + 1), scales.at((i + 1 + j) % numTiles)));
		}
		
		FiniteTimeAction* moveAction = RepeatForever::create(CardinalSplineTo::create(travelDur, path, 0.1f));
		FiniteTimeAction* scaleAction = RepeatForever::create(Sequence::create(scaleActions));
		
		Sprite* tile = Sprite::create("res/introAssets/content_tile_" + StringUtils::format("%d",i) + ".jpg");
		tile->setPosition(points->getControlPointAtIndex(i));
		tile->runAction(moveAction->clone());
		tile->runAction(scaleAction->clone());
		tile->runAction(Sequence::createWithTwoActions(DelayTime::create(RandomHelper::random_real(0.75, 1.25)), FadeTo::create(RandomHelper::random_real(1.0, 2.0), 175)));
		tile->setOpacity(0);
		tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		
		ui::Scale9Sprite* stencil = ui::Scale9Sprite::create("res/introAssets/clipping_asset.png");
		stencil->setPosition(points->getControlPointAtIndex(i));
		stencil->runAction(moveAction->clone());
		stencil->runAction(scaleAction->clone());
		stencil->setContentSize(tile->getContentSize());
		
		ClippingNode* clip = ClippingNode::create(stencil);
		clip->setContentSize(contentSize);
		clip->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		clip->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
		clip->setAlphaThreshold(0.5f);
		clip->addChild(tile);
		_tilesNode->addChild(clip);
		
	}
}

NS_AZOOMEE_END
