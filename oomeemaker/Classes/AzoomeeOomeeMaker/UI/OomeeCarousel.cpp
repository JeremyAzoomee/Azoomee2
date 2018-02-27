//
//  OomeeCarousel.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 27/02/2018.
//

#include "OomeeCarousel.h"

NS_AZOOMEE_OM_BEGIN

using namespace cocos2d;

bool OomeeCarousel::init()
{
    if(!Super::init())
    {
        return false;
    }
    _contentNode = ClippingRectangleNode::create(Rect(0,0,this->getContentSize().width, this->getContentSize().height));
    this->addChild(_contentNode);
    return true;
}

void OomeeCarousel::onEnter()
{
    setTouchListener();
    
    Super::onEnter();
}

void OomeeCarousel::setContentSize(const Size& contentSize)
{
    Super::setContentSize(contentSize);
    _contentNode->setClippingRegion(Rect(0,0,contentSize.width, contentSize.height));
}

void OomeeCarousel::setTouchListener()
{
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        return true;
    };
    
    listener->onTouchMoved = [&](Touch* touch, Event* event)
    {
        for(ui::Button* button : _carouselButtons)
        {
            button->setPosition(button->getPosition() + Vec2(touch->getDelta().x,0));
        }
        float spacing = this->getContentSize().width / _visibleRange;
        for(int i = 0; i < _carouselButtons.size(); i++)
        {
            ui::Button* button = _carouselButtons.at(i);
            if(touch->getDelta().x < 0)
            {
                if(button->getPosition().x + (button->getContentSize().width) < this->getPosition().x - (this->getContentSize().width / 2))
                {
                    if(button == _leftMostButton)
                    {
                        button->setPosition(_rightMostButton->getPosition() + Vec2(spacing, 0));
                        _rightMostButton = button;
                        _leftMostButton = _carouselButtons.at((i+1)%_carouselButtons.size());
                    }
                }
            }
            else
            {
                if(button->getPosition().x > this->getPosition().x + (this->getContentSize().width / 2))
                {
                    if(button == _rightMostButton)
                    {
                        button->setPosition(_leftMostButton->getPosition() - Vec2(spacing, 0));
                        _leftMostButton = button;
                        _rightMostButton = _carouselButtons.at((i + _carouselButtons.size() - 1) % _carouselButtons.size());
                    }
                }
            }
            Vec2 buttonCenterPos = button->getPosition() + button->getContentSize() / 2.0f;
            float dist = buttonCenterPos.distance(this->getPosition());
            float relativeDist = dist / (this->getContentSize().width / 2.0f);
            button->setScale(MAX(0.1,1.2 - relativeDist));
            button->setOpacity(MAX(0,255 - (255 * relativeDist)));
            
        }
        
    };
    
    listener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        
    };
    
    listener->onTouchCancelled = listener->onTouchEnded;
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void OomeeCarousel::setOomeeData(const std::vector<std::string>& oomeeFilenames)
{
    _oomeeData = oomeeFilenames;
    float spacing = this->getContentSize().width / _visibleRange;
    for(int i = 0; i < oomeeFilenames.size(); i++)
    {
        ui::Button* button = ui::Button::create(oomeeFilenames.at(i));
        button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        button->setPosition(Vec2(spacing * i, this->getContentSize().height / 2.0f));
        _contentNode->addChild(button);
        _carouselButtons.push_back(button);
        if(i == 0)
        {
            _leftMostButton = button;
        }
        if(i == oomeeFilenames.size() - 1)
        {
            _rightMostButton = button;
        }
    }
}

void OomeeCarousel::setVisibleRange(int visibleRange)
{
    _visibleRange = visibleRange;
}

NS_AZOOMEE_OM_END
