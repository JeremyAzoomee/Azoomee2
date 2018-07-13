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

NS_AZOOMEE_OM_BEGIN

using namespace cocos2d;

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
        if(_centerButton)
        {
            OomeeCarouselButton* carouselButton = dynamic_cast<OomeeCarouselButton*>(_centerButton);
            if(carouselButton)
            {
                carouselButton->setInFocus(false);
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
    
    if(_oomeeData.size() == 0)
    {
        LazyLoadingButton* button = LazyLoadingButton::create();
        button->setMainImage("res/oomeeMaker/newoomee_cta.png");
        button->setPlaceholderImage("res/oomeeMaker/body_00.png");
        button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        button->setPosition(Vec2(_spacing * _carouselButtons.size(), this->getContentSize().height / 2.0f));
        button->setSwallowTouches(false);
        button->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType){
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                OomeeSelectScene::newOomee();
            }
        });
        _contentNode->addChild(button);
        _carouselButtons.push_back(button);
    }
    
    for(int i = 0; i < oomeeFilenames.size(); i++)
    {
        const std::string assetName = OomeeMakerDataHandler::getInstance()->getFullSaveDir() + oomeeFilenames.at(i) + ".png";
        OomeeCarouselButton* button = OomeeCarouselButton::create();
        button->setDelegate(_buttonDelegate);
        button->setOomeeData(oomeeFilenames.at(i));
        button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        button->setPosition(Vec2(_spacing * i, this->getContentSize().height / 2.0f));
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
    if(carouselButton)
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

void OomeeCarousel::update(float deltaT)
{
    for(LazyLoadingButton* button : _carouselButtons)
    {
        Vec2 buttonCenterPos = button->getPosition();// + button->getContentSize() / 2.0f;
        float dist = abs(button->getPosition().x - this->getPosition().x);
        float relativeDist = dist / (this->getContentSize().width / 2.0f);
        button->setScale(MAX(0.1,1.2 - relativeDist));
        if(ConfigStorage::getInstance()->isDevice18x9())
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

NS_AZOOMEE_OM_END
