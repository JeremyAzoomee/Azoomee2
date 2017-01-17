#include "AwaitingAdultPinLayer.h"
#include "ModalMessages.h"

bool AwaitingAdultPinLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    
    createAndFadeInLayer();
    addUIObjects();
    
    return true;
}

//---------------------- Create Layer -----------------------------

void AwaitingAdultPinLayer::createAndFadeInLayer()
{
    backgroundLayer = LayerColor::create(Color4B(0,0,0,150), origin.x + visibleSize.width, origin.y +visibleSize.height);
    backgroundLayer->setPosition(origin.x, origin.y);
    backgroundLayer->setOpacity(0);
    this->addChild(backgroundLayer);
    
    addListenerToBackgroundLayer();
    
    backgroundLayer->runAction(FadeTo::create(0.5, 255));
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

void AwaitingAdultPinLayer::addUIObjects()
{
    auto enterYourPinTitle = Label::createWithTTF("Please enter your pin to proceed", "fonts/azoomee.ttf", 90);
    enterYourPinTitle->setPosition(origin.x+visibleSize.width /2, origin.y+visibleSize.height*0.8);
    enterYourPinTitle->setColor(Color3B::WHITE);
    enterYourPinTitle->setOpacity(0);
    backgroundLayer->addChild(enterYourPinTitle);
    
    enterYourPinTitle->runAction(FadeTo::create(0.5, 255));
    
    editBox_pin = TextInputLayer::createWithSize(Size(400,131), INPUT_IS_PIN);
    editBox_pin->setCenterPosition(Vec2(origin.x+visibleSize.width /2, origin.y+visibleSize.height*0.65));
    editBox_pin->setDelegate(this);
    backgroundLayer->addChild(editBox_pin);
    
    editBox_pin->focusAndShowKeyboard();
    
    cancelButton = ElectricDreamsButton::createCancelButton();
    cancelButton->setCenterPosition(Vec2(origin.x + visibleSize.width /2 - 400, origin.y + visibleSize.height * 0.65));
    cancelButton->setDelegate(this);
    backgroundLayer->addChild(cancelButton);
    
    acceptButton = ElectricDreamsButton::createAcceptButton();
    acceptButton->setCenterPosition(Vec2(origin.x + visibleSize.width /2 + 400, origin.y + visibleSize.height * 0.65));
    acceptButton->setDelegate(this);
    acceptButton->setVisible(false);
    backgroundLayer->addChild(acceptButton);
    
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

//---------------------- Actions -----------------

void AwaitingAdultPinLayer::removeSelf()
{
    this->removeChild(backgroundLayer);
    this->removeFromParent();
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
    acceptButton->setVisible(isValid);
    
}

void AwaitingAdultPinLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == cancelButton)
        removeSelf();
    else if(button == acceptButton)
    {
        if(editBox_pin->getText() == "2222")
        {
            this->getDelegate()->AdultPinAccepted(this);
            removeSelf();
        }
        else
        {
            ModalMessages::getInstance()->createMessageWithSingleButton("Error", "Pin is incorrect.", "Retry");
            editBox_pin->setText("");
        }
    }
    
}

