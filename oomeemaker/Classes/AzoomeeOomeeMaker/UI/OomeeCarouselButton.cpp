//
//  OomeeCarouselButton.cpp
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 16/03/2018.
//

#include "OomeeCarouselButton.h"
#include "SimpleAudioEngine.h"
#include "../DataObjects/OomeeMakerDataHandler.h"
#include "OomeeSelectScene.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

bool OomeeCarouselButton::init()
{
    if(!Super::init())
    {
        return false;
    }
	
	setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	setPlaceholderImage("res/oomeeMaker/1_Oomee_Reference.png");
	loadPlaceholderImage();
	ignoreContentAdaptWithSize(false);
	
	_innerCircle = Sprite::create("res/oomeeMaker/circle_0.png");
	_innerCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	BlendFunc blendFunc = BlendFunc();
	blendFunc.src = GL_ZERO;
	blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
	_innerCircle->setBlendFunc(blendFunc);
	//bgCircle1->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	//float targetScale = (Vec2(this->getContentSize()).length() * 0.49f) / bgCircle1->getContentSize().height;
	//bgCircle1->setPosition(Vec2(this->getContentSize().width / 2,  (bgCircle1->getContentSize().height * targetScale) / 2));
	_innerCircle->setScale(0);
	_innerCircle->setRotation(RandomHelper::random_real(0.0,M_PI));
	this->addChild(_innerCircle, -1);
	
	auto popIn1 = EaseBackOut::create(ScaleTo::create(0.5, (Vec2(this->getContentSize()).length() * 0.55f) / _innerCircle->getContentSize().height));
	auto rotate1 = RepeatForever::create(RotateBy::create(30 + CCRANDOM_0_1() * 30, 360));
	
	_innerCircle->runAction(popIn1);
	_innerCircle->runAction(rotate1);
	
	_outerCircle = Sprite::create("res/oomeeMaker/circle_1.png");
	_outerCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_outerCircle->setBlendFunc(blendFunc);
	//bgCircle2->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	//bgCircle2->setPosition(Vec2(this->getContentSize().width / 2,  (bgCircle2->getContentSize().height * targetScale) / 2));
	_outerCircle->setScale(0);
	_outerCircle->setRotation(RandomHelper::random_real(0.0,M_PI));
	this->addChild(_outerCircle, -1);
	
	auto popIn2 = EaseBackOut::create(ScaleTo::create(0.5, (Vec2(this->getContentSize()).length() * 0.75f) / _outerCircle->getContentSize().height));
	auto rotate2 = RepeatForever::create(RotateBy::create(30 +  CCRANDOM_0_1() * 30, -360));
	
	_outerCircle->runAction(popIn2);
	_outerCircle->runAction(rotate2);
	
	float targetScale = (Vec2(this->getContentSize()).length() * 0.75f) / _outerCircle->getContentSize().height;
	_innerCircle->setNormalizedPosition(Vec2(0.5, ((_outerCircle->getContentSize().height * targetScale) / 2.5) / this->getContentSize().height));
	_outerCircle->setNormalizedPosition(Vec2(0.5, ((_outerCircle->getContentSize().height * targetScale) / 2.5) / this->getContentSize().height));
	
	_outerCircle->setColor(Style::Color::darkTeal);
	_innerCircle->setColor(Style::Color::darkTeal);
	
	_tutInnerCircle = Sprite::create("res/oomeeMaker/inner_circle_tut.png");
	_tutInnerCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_tutInnerCircle->setRotation(_innerCircle->getRotation());
	_tutInnerCircle->runAction(rotate1->clone());
	_tutInnerCircle->setOpacity(0);
	_tutInnerCircle->setNormalizedPosition(_innerCircle->getNormalizedPosition());
	this->addChild(_tutInnerCircle, -1);
	
	_tutOuterCircle = Sprite::create("res/oomeeMaker/outer_circle_tut.png");
	_tutOuterCircle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_tutOuterCircle->setRotation(_outerCircle->getRotation());
	_tutOuterCircle->runAction(rotate2->clone());
	_tutOuterCircle->setOpacity(0);
	_tutOuterCircle->setNormalizedPosition(_outerCircle->getNormalizedPosition());
	this->addChild(_tutOuterCircle, -1);
	
	_mainCircleColour = LayerGradient::create(Color4B(Style::Color::darkTeal), Color4B(Style::Color::greenishTeal), Vec2(-1,1));
	BlendFunc blendFunc1 = BlendFunc();
	blendFunc1.src = GL_ONE_MINUS_DST_ALPHA;
	blendFunc1.dst = GL_DST_ALPHA;
	_mainCircleColour->setBlendFunc(blendFunc1);
	_mainCircleColour->setContentSize(this->getContentSize() * 1.3f);
	_mainCircleColour->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_mainCircleColour->setPosition(this->getContentSize() * -0.15f);
	this->addChild(_mainCircleColour,-1);
	
	_highlightCircleColour = LayerGradient::create(Color4B(Style::Color::greenishTeal), Color4B(Style::Color::greenishTeal), Vec2(-1,1));
	_highlightCircleColour->setBlendFunc(blendFunc1);
	_highlightCircleColour->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_highlightCircleColour->setPosition(this->getContentSize() * -0.15f);
	_highlightCircleColour->setContentSize(this->getContentSize() * 1.3f);
	_highlightCircleColour->setOpacity(0);
	_highlightCircleColour->setVisible(false);
	this->addChild(_highlightCircleColour,-1);
	
	_deleteButton = ui::Button::create("res/oomeeMaker/bin_button.png");
	_deleteButton->setContentSize(_deleteButton->getContentSize() * 0.8f);
	_deleteButton->ignoreContentAdaptWithSize(false);
	_deleteButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_deleteButton->setPosition(this->getContentSize()/2);
	_deleteButton->setScale(0);
	_deleteButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->deleteOomee(_oomeeFileName);
			}
		}
	});
	this->addChild(_deleteButton);
	
	_makeAvatarButton = ui::Button::create("res/oomeeMaker/make_oomee_button.png");
	_makeAvatarButton->setContentSize(_makeAvatarButton->getContentSize() * 0.8f);
	_makeAvatarButton->ignoreContentAdaptWithSize(false);
	_makeAvatarButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_makeAvatarButton->setPosition(this->getContentSize()/2);
	_makeAvatarButton->setScale(0);
	_makeAvatarButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->makeAvatar(_oomeeFileName);
			}
		}
	});
	this->addChild(_makeAvatarButton);
	
	_shareButton = ui::Button::create("res/oomeeMaker/message_button.png");
	_shareButton->setContentSize(_shareButton->getContentSize() * 0.8f);
	_shareButton->ignoreContentAdaptWithSize(false);
	_shareButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_shareButton->setPosition(this->getContentSize()/2);
	_shareButton->setScale(0);
	_shareButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->shareOomee(_oomeeFileName);
			}
		}
	});
	this->addChild(_shareButton);
	
    return true;
}

void OomeeCarouselButton::onEnter()
{
    Super::onEnter();
}

void OomeeCarouselButton::setOomeeData(const std::string &filename)
{
    _oomeeFileName = filename;
    rapidjson::Document data;
    data.Parse(FileUtils::getInstance()->getStringFromFile(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + filename + OomeeMakerDataHandler::kOomeeFileExtension).c_str());
    if(!data.HasParseError())
    {
        _oomeeData = OomeeFigureData::createWithData(data);
        _deleteButton->setEnabled(!_oomeeData->isSelected());
        _deleteButton->setOpacity(_oomeeData->isSelected() ? 125 : 255);
    }
    setMainImage(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + filename + ".png");
    setPlaceholderImage("res/oomeeMaker/1_Oomee_Reference.png");
    loadPlaceholderImage();
}

std::string OomeeCarouselButton::getOomeeFilename() const
{
    return _oomeeFileName;
}

void OomeeCarouselButton::setInFocus(bool inFocus)
{
    if(inFocus == _inFocus)
    {
        //dont do anything if already in state
        return;
    }
    
    _inFocus = inFocus;
    if(inFocus)
    {
        animateButtonsIn(1);
    }
    else
    {
        animateButtonsOut(0.3);
    }
}

bool OomeeCarouselButton::getInFocus() const
{
    return _inFocus;
}

void OomeeCarouselButton::setDelegate(OomeeCarouselButtonDelegate *delegate)
{
    _delegate = delegate;
}

void OomeeCarouselButton::animateButtonsIn(float duration)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Homescreen_Buttons_Appear.mp3");
    const float centerWidth = this->getContentSize().width / 2.0f;
    const float centerHeight = this->getContentSize().height / 2.0f;
    const float durationMod = 1 - _deleteButton->getScale();
    duration *= durationMod;
    Action* scaleAction = ScaleTo::create(duration, 1);
    
    Vec2 basePos = Vec2(centerWidth, centerHeight + this->getContentSize().height*0.65f);
    
    Action* moveActionAvatar = EaseBackOut::create(MoveTo::create(duration, basePos));
    _makeAvatarButton->stopAllActions();
    _makeAvatarButton->runAction(scaleAction->clone());
    _makeAvatarButton->runAction(moveActionAvatar);
    
    basePos.rotate(Vec2(centerWidth,centerHeight),CC_DEGREES_TO_RADIANS(-30));
    Action* moveActionShare = EaseBackOut::create(MoveTo::create(duration, basePos));
    _shareButton->stopAllActions();
    _shareButton->runAction(scaleAction->clone());
    _shareButton->runAction(moveActionShare);
    
    
    basePos.rotate(Vec2(centerWidth,centerHeight),CC_DEGREES_TO_RADIANS(60));
    Action* moveActionDelete = EaseBackOut::create(MoveTo::create(duration, basePos));
    _deleteButton->stopAllActions();
    _deleteButton->runAction(scaleAction->clone());
    _deleteButton->runAction(moveActionDelete);
    
}

void OomeeCarouselButton::animateButtonsOut(float duration)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
    const float durationMod = _deleteButton->getScale();
    duration *= durationMod;
    Action* moveAction = EaseBackIn::create(MoveTo::create(duration, Vec2(this->getContentSize()/2)));
    Action* scaleAction = ScaleTo::create(duration, 0);
    
    _deleteButton->stopAllActions();
    _deleteButton->runAction(scaleAction->clone());
    _deleteButton->runAction(moveAction->clone());
    
    _shareButton->stopAllActions();
    _shareButton->runAction(scaleAction->clone());
    _shareButton->runAction(moveAction->clone());
    
    _makeAvatarButton->stopAllActions();
    _makeAvatarButton->runAction(scaleAction->clone());
    _makeAvatarButton->runAction(moveAction->clone());
}

void OomeeCarouselButton::setHighlightCircleColour(const Color4B& startGradColour, const Color4B& endGradColour, const Vec2& gradDir)
{
	if(_highlightCircleColour)
	{
		_highlightCircleColour->setVector(gradDir);
		_highlightCircleColour->setStartColor(Color3B(startGradColour));
		_highlightCircleColour->setStartOpacity(startGradColour.a);
		_highlightCircleColour->setEndColor(Color3B(endGradColour));
		_highlightCircleColour->setEndOpacity(endGradColour.a);
	}
}

void OomeeCarouselButton::enableHighlight(bool enable)
{
    if(enable)
    {
		_highlightCircleColour->runAction(FadeTo::create(0.5f,255));
		_mainCircleColour->runAction(FadeTo::create(0.5f,0));
		_highlightCircleColour->setVisible(true);
    }
    else
    {
		_highlightCircleColour->runAction(Sequence::create(FadeTo::create(0.5f, 0), CallFunc::create([&](){_highlightCircleColour->setVisible(false);}), NULL));
		_mainCircleColour->runAction(FadeTo::create(0.5f,255));
    }
}

void OomeeCarouselButton::enableTutorialHighlight(bool enable)
{
	if(enable)
	{
		auto pulse = RepeatForever::create(Sequence::create(DelayTime::create(0.25f), ScaleTo::create(1.5f, 1.05f), ScaleTo::create(1.5f, 1.0f), NULL));
		pulse->setTag(1000);
		auto fade = FadeTo::create(1.0f, 255);
		auto tint = RepeatForever::create(Sequence::create(DelayTime::create(0.25f), TintTo::create(1.5f, Color3B::WHITE), TintTo::create(1.5f, Color3B(29,117,99)), NULL));
		tint->setTag(1001);
		
		_tutInnerCircle->runAction(pulse->clone());
		_tutOuterCircle->runAction(pulse->clone());
		_mainCircleColour->setVisible(false);
		_tutInnerCircle->runAction(tint->clone());
		_tutOuterCircle->runAction(tint->clone());
		_tutInnerCircle->runAction(fade->clone());
		_tutOuterCircle->runAction(fade->clone());
	}
	else
	{
		_mainCircleColour->setVisible(true);
		_tutInnerCircle->stopActionByTag(1000);
		_tutOuterCircle->stopActionByTag(1000);

		_tutInnerCircle->stopActionByTag(1001);
		_tutOuterCircle->stopActionByTag(1001);
		
		_tutInnerCircle->runAction(FadeTo::create(0.5f,0));
		_tutOuterCircle->runAction(FadeTo::create(0.5f,0));
	}
}


NS_AZOOMEE_OM_END
