//
//  OomeeCarousel.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 27/02/2018.
//

#include "OomeeCarousel.h"
#include "../DataObjects/OomeeMakerDataHandler.h"
#include "OomeeSelectScene.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Device.h>

NS_AZOOMEE_OM_BEGIN

using namespace cocos2d;

const std::string OomeeCarousel::kNewOomeeTag = "new";

bool OomeeCarousel::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _contentNode = ClippingRectangleNode::create(Rect(0,0,this->getContentSize().width, this->getContentSize().height));
    this->addChild(_contentNode);
    return true;
}

void OomeeCarousel::onEnter()
{
    this->scheduleUpdate();
    Super::onEnter();
    this->getScheduler()->schedule([this](float deltaT){
        this->centerButtons();
        setTouchListener();
    }, this, 0, 0, 1.0, 0, "centerButtons");
}

void OomeeCarousel::setContentSize(const Size& contentSize)
{
    Super::setContentSize(contentSize);
    _contentNode->setClippingRegion(Rect(0,0,contentSize.width, contentSize.height));
    _spacing = this->getContentSize().width / (_visibleRange + 1);
}

void OomeeCarousel::setTouchListener()
{
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        for(LazyLoadingButton* button : _carouselButtons)
        {
            button->stopAllActions();
        }
        return true;
    };
    
    listener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        if(touch->getLocation().distance(touch->getStartLocation()) > 50)
        {
            if(_centerButton)
            {
                OomeeCarouselButton* carouselButton = dynamic_cast<OomeeCarouselButton*>(_centerButton);
                if(carouselButton && carouselButton->getName() != kNewOomeeTag)
                {
                    carouselButton->setInFocus(false);
                }
            }
        }
        
        for(LazyLoadingButton* button : _carouselButtons)
        {
            button->setPosition(button->getPosition() + Vec2(touch->getDelta().x,0));
        }

        if(touch->getDelta().x < 0)
        {
            this->rotateButtonsLeft();
        }
        else
        {
            this->rotateButtonsRight();
        }
        
        
    };
    
    listener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        centerButtons();
    };
    
    listener->onTouchCancelled = listener->onTouchEnded;
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void OomeeCarousel::setOomeeData(const std::vector<std::string>& oomeeFilenames)
{
    _contentNode->removeAllChildren();
    _carouselButtons.clear();
    _centerButton = nullptr;
    
    _oomeeData = oomeeFilenames;
    
    int j = 0;
    if(_oomeeData.size() < 3)
    {
        for(; j < (3 - _oomeeData.size()); j++)
        {
            OomeeCarouselButton* button = OomeeCarouselButton::create();
            button->setPlaceholderImage("res/oomeeMaker/new_oomee_button_3.png");
            button->loadPlaceholderImage();
            button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            button->setPosition(Vec2(_spacing * j, this->getContentSize().height / 2.0f));
            button->setSwallowTouches(false);
            button->setName(kNewOomeeTag);
            button->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType){
                if(pSender == _centerButton)
                {
                    if(eType == ui::Widget::TouchEventType::ENDED)
                    {
                        OomeeSelectScene::newOomee();
                    }
                }
            });
            _contentNode->addChild(button);
            _carouselButtons.push_back(button);
        }
    }
    
    for(int i = 0; i < oomeeFilenames.size(); i++)
    {
        const std::string assetName = OomeeMakerDataHandler::getInstance()->getFullSaveDir() + oomeeFilenames.at(i) + ".png";
        OomeeCarouselButton* button = OomeeCarouselButton::create();
        button->setDelegate(_buttonDelegate);
        button->setOomeeData(oomeeFilenames.at(i));
        button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        button->setPosition(Vec2(_spacing * (i + j), this->getContentSize().height / 2.0f));
        button->setSwallowTouches(false);
        button->setCascadeOpacityEnabled(true);
        button->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType){
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                if(pSender == _centerButton)
                {
                    auto oomeeCenterButton = dynamic_cast<OomeeCarouselButton*>(_centerButton);
                    if(oomeeCenterButton && oomeeCenterButton->getInFocus())
                    {
                        if(_buttonDelegate)
                        {
                            _buttonDelegate->editOomee(_oomeeData.at(i));
                        }
                    }
                }
            }
        });
        _contentNode->addChild(button);
        _carouselButtons.push_back(button);
    }
    
    _leftMostButton = _carouselButtons.at(0);

    _rightMostButton = _carouselButtons.at(_carouselButtons.size() - 1);
    
    rotateButtonsLeft();
    
}

void OomeeCarousel::setVisibleRange(int visibleRange)
{
    _visibleRange = visibleRange;
    _spacing = this->getContentSize().width / (_visibleRange + 1);
}

void OomeeCarousel::centerButtons()
{
    float minDist = FLT_MAX;
    Vec2 moveVector = Vec2(0,0);
    LazyLoadingButton* centerButton = nullptr;
    for(LazyLoadingButton* button : _carouselButtons)
    {
        float dist = abs(button->getPosition().x - this->getPosition().x);
        if(dist < minDist)
        {
            minDist = dist;
            moveVector.x = this->getPosition().x - button->getPosition().x;
            centerButton = button;
        }
    }
    
    if(!centerButton)
    {
        return;
    }
    
    OomeeCarouselButton* carouselButton = dynamic_cast<OomeeCarouselButton*>(centerButton);
    if(carouselButton && carouselButton->getName() != kNewOomeeTag)
    {
        carouselButton->setInFocus(true);
    }
    _centerButton = centerButton;
    
    Action* action = EaseBackOut::create(MoveBy::create(moveVector.length()/(this->getContentSize().width / 4), moveVector));
    
    for(LazyLoadingButton* button : _carouselButtons)
    {
        button->runAction(action->clone());
    }
    
}

void OomeeCarousel::moveCarouselLeft()
{
    for(LazyLoadingButton* button : _carouselButtons)
    {
        button->stopAllActions();
    }
    
    if(_centerButton)
    {
        OomeeCarouselButton* carouselButton = dynamic_cast<OomeeCarouselButton*>(_centerButton);
        if(carouselButton && carouselButton->getName() != kNewOomeeTag)
        {
            carouselButton->setInFocus(false);
        }
    }
    
    Vec2 moveVec = Vec2(-_spacing * 0.7f,0);
    FiniteTimeAction* moveLeft = MoveBy::create(moveVec.length()/(this->getContentSize().width / 2), moveVec);
    rotateButtonsLeft();
    for(LazyLoadingButton* button : _carouselButtons)
    {
        if(button == _centerButton)
        {
            button->runAction(Sequence::create(moveLeft->clone(), CallFunc::create([&](){
                rotateButtonsLeft();
                centerButtons();
            }) , NULL));
        }
        else
        {
            button->runAction(moveLeft->clone());
        }
        
    }
    
}

void OomeeCarousel::moveCarouselRight()
{
    for(LazyLoadingButton* button : _carouselButtons)
    {
        button->stopAllActions();
    }
    
    if(_centerButton)
    {
        OomeeCarouselButton* carouselButton = dynamic_cast<OomeeCarouselButton*>(_centerButton);
        if(carouselButton && carouselButton->getName() != kNewOomeeTag)
        {
            carouselButton->setInFocus(false);
        }
    }
    
    Vec2 moveVec = Vec2(_spacing * 0.7f,0);
    FiniteTimeAction* moveRight = MoveBy::create(moveVec.length()/(this->getContentSize().width / 2), moveVec);
    rotateButtonsRight();
    for(LazyLoadingButton* button : _carouselButtons)
    {
        if(button == _centerButton)
        {
            button->runAction(Sequence::create(moveRight->clone(), CallFunc::create([&](){
                rotateButtonsRight();
                centerButtons();
            }) , NULL));
        }
        else
        {
            button->runAction(moveRight->clone());
        }
        
    }
}

void OomeeCarousel::rotateButtonsRight()
{
    for(int i = 0; i < _carouselButtons.size(); i++)
    {
        LazyLoadingButton* button = _carouselButtons.at(i);
        if(button->getPosition().x > this->getPosition().x + (_spacing * (_visibleRange/2.0f)))
        {
            if(button == _rightMostButton)
            {
                button->setPosition(_leftMostButton->getPosition() - Vec2(_spacing, 0));
                _leftMostButton = button;
                _rightMostButton = _carouselButtons.at((i + _carouselButtons.size() - 1) % _carouselButtons.size());
            }
        }
    }
}

void OomeeCarousel::rotateButtonsLeft()
{
    for(int i = 0; i < _carouselButtons.size(); i++)
    {
        LazyLoadingButton* button = _carouselButtons.at(i);
        if(button->getPosition().x < this->getPosition().x - (_spacing * (_visibleRange/2.0f)))
        {
            if(button == _leftMostButton)
            {
                button->setPosition(_rightMostButton->getPosition() + Vec2(_spacing, 0));
                _rightMostButton = button;
                _leftMostButton = _carouselButtons.at((i+1)%_carouselButtons.size());
            }
        }
    }
}

void OomeeCarousel::centerOnOomee(const std::string &targetOomee)
{
    auto target = std::find_if(_carouselButtons.begin(), _carouselButtons.end(), [=](LazyLoadingButton* button){
        OomeeCarouselButton* oomeeButton = dynamic_cast<OomeeCarouselButton*>(button);
        if(oomeeButton)
        {
            return oomeeButton->getOomeeFilename() == targetOomee;
        }
        return false;
    });
    
    if(target == _carouselButtons.end())
    {
        return;
    }
    
    OomeeCarouselButton* targetButton = dynamic_cast<OomeeCarouselButton*>(*target);
    if(!targetButton)
    {
        return;
    }
    
    for(LazyLoadingButton* button : _carouselButtons)
    {
        button->stopAllActions();
    }
    
    if(_centerButton)
    {
        OomeeCarouselButton* carouselButton = dynamic_cast<OomeeCarouselButton*>(_centerButton);
        if(carouselButton && carouselButton->getName() != kNewOomeeTag)
        {
            carouselButton->setInFocus(false);
        }
    }
    
    Vec2 moveVec = Vec2(this->getPosition().x - targetButton->getPosition().x,0);
    
    for(LazyLoadingButton* button : _carouselButtons)
    {
        button->setPosition(button->getPosition() + moveVec);
    }
    
    moveVec.x > 0 ? rotateButtonsRight() : rotateButtonsLeft();
    
}

void OomeeCarousel::update(float deltaT)
{
    for(LazyLoadingButton* button : _carouselButtons)
    {
        Vec2 buttonCenterPos = button->getPosition();// + button->getContentSize() / 2.0f;
        float dist = abs(button->getPosition().x - this->getPosition().x);
        float relativeDist = dist / (this->getContentSize().width / 2.0f);
        button->setScale(MAX(0.1,1.2 - relativeDist));
        if(Device::getInstance()->isDevice18x9())
        {
            button->setScale(button->getScale() * 0.8f);
        }
        button->setOpacity(MAX(0,255 - (255 * relativeDist)));
        button->setPositionY(this->getContentSize().height/2 + this->getContentSize().height/5 * (relativeDist * relativeDist));
        button->setLocalZOrder(button->getScale() * 10.0f);
    }
    
    Super::update(deltaT);
}

void OomeeCarousel::setButtonDelegate(OomeeCarouselButtonDelegate* delegate)
{
    _buttonDelegate = delegate;
    for(LazyLoadingButton* button : _carouselButtons)
    {
        OomeeCarouselButton* oomeeButton = dynamic_cast<OomeeCarouselButton*>(button);
        if(oomeeButton)
        {
            oomeeButton->setDelegate(delegate);
        }
    }
}

OomeeCarouselButton* OomeeCarousel::getCenterButton()
{
    return dynamic_cast<OomeeCarouselButton*>(_centerButton);
}

std::vector<std::string> OomeeCarousel::getOomeeData() const
{
	return _oomeeData;
}

NS_AZOOMEE_OM_END
