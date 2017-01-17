#include "AwaitingAdultPinLayer.h"

bool AwaitingAdultPinLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

//---------------------- Create Layer -----------------------------

void AwaitingAdultPinLayer::createAndFadeInLayer()
{
    backgroundLayer = LayerColor::create(Color4B(0,0,0,150), visibleSize.width, visibleSize.height);
    backgroundLayer->setPosition(origin.x, origin.y);
    backgroundLayer->setOpacity(0);
    this->addChild(backgroundLayer);
    
    addListenerToBackgroundLayer();
    
    backgroundLayer->runAction(FadeTo::create(0.5, 200));
}

void AwaitingAdultPinLayer::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

/*ElectricDreamsButton* ElectricDreamsButton::createTextAsButton(std::string buttonText)
{
    auto layer = ElectricDreamsButton::create();
    
    Label* textButton = Label::createWithTTF(buttonText, "fonts/azoomee.ttf", 90);
    textButton->setColor(Color3B(28, 244, 244));
    textButton->setPosition(Vec2(textButton->getContentSize().width/2, textButton->getContentSize().height/2));

    layer->setContentSize(textButton->getContentSize());
    
    layer->addChild(textButton);
    
    layer->addListener();
    
    return layer;
}*/

//---------------------- Listener Function -----------------------------

/*void AwaitingAdultPinLayer::addListener()
{

    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Rect rect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);
        
        if(rect.containsPoint(locationInNode) && this->isVisible())
        {
            this->scheduleOnce(schedule_selector(AwaitingAdultPinLayer::callDelegateFunction), 0.1);
            
            return true;
        }
        
        return false;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}*/

void AwaitingAdultPinLayer::callDelegateFunction(float dt)
{
    this->getDelegate()->AdultPinAccepted(this);
}

//---------------------- public Functions After Setup -----------------------------

void AwaitingAdultPinLayer::setCenterPosition(Vec2 position)
{
    this->setPosition(Vec2(position.x - this->getContentSize().width/2, position.y - this->getContentSize().height/2));
}

Vec2 AwaitingAdultPinLayer::getCenterPosition()
{
    return Vec2(this->getPositionX() + this->getContentSize().width/2, this->getPositionY() + this->getContentSize().height/2);
}

//----------------------- Delegate Functions ----------------------------

void AwaitingAdultPinLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{

    
}

void AwaitingAdultPinLayer::buttonPressed(ElectricDreamsButton* button)
{

    
}

