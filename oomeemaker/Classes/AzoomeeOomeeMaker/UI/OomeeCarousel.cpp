//
//  OomeeCarousel.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 27/02/2018.
//

#include "OomeeCarousel.h"
#include "../DataObjects/OomeeMakerDataHandler.h"
#include "OomeeSelectScene.h"

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
    setTouchListener();
    this->scheduleUpdate();
    centerButtons();
    Super::onEnter();
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
    
    _oomeeData = oomeeFilenames;
    
    for(int i = 0; i < oomeeFilenames.size(); i++)
    {
        const std::string assetName = OomeeMakerDataHandler::getInstance()->getFullSaveDir() + oomeeFilenames.at(i) + ".png";
        LazyLoadingButton* button = LazyLoadingButton::create();
        button->setMainImage(assetName);
        button->setPlaceholderImage("res/oomeeMaker/body_00.png");
        button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        button->setPosition(Vec2(_spacing * i, this->getContentSize().height / 2.0f));
        button->setSwallowTouches(false);
        button->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType){
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                OomeeSelectScene::editOomee(OomeeMakerDataHandler::getInstance()->getLocalSaveDir() + oomeeFilenames.at(i));
            }
        });
        _contentNode->addChild(button);
        _carouselButtons.push_back(button);
    }
    
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
    for(LazyLoadingButton* button : _carouselButtons)
    {
        //Vec2 buttonCenterPos = button->getPosition();// + button->getContentSize() / 2.0f;
        float dist = abs(button->getPosition().x - this->getPosition().x);//buttonCenterPos.distance(this->getPosition());
        if(dist < minDist)
        {
            minDist = dist;
            moveVector.x = this->getPosition().x - button->getPosition().x;
        }
    }
    
    Action* action = EaseBackOut::create(MoveBy::create(moveVector.length()/(this->getContentSize().width / 2), moveVector));
    
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
        button->setOpacity(MAX(0,255 - (255 * relativeDist)));
        button->setPositionY(this->getContentSize().height/2 + this->getContentSize().height/5 * (relativeDist * relativeDist));
        
    }
    
    Super::update(deltaT);
}

NS_AZOOMEE_OM_END
