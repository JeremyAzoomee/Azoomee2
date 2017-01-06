#include "ElectricDreamsButton.h"

bool ElectricDreamsButton::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//---------------------- Create Button Types -----------------------------

ElectricDreamsButton* ElectricDreamsButton::createTextAsButton(std::string buttonText)
{
    auto layer = ElectricDreamsButton::create();
    
    Label* textButton = Label::createWithTTF(buttonText, "fonts/azoomee.ttf", 90);
    textButton->setColor(Color3B(28, 244, 244));
    textButton->setPosition(Vec2(textButton->getContentSize().width/2, textButton->getContentSize().height/2));

    layer->setContentSize(textButton->getContentSize());
    
    layer->addChild(textButton);
    
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createButtonWithText(std::string buttonText)
{
    auto layer = ElectricDreamsButton::create();
    
    ui::Scale9Sprite* newButton = layer->createButtonBackground(buttonText);

    layer->setContentSize(newButton->getContentSize());
    
    layer->addChild(newButton);
    
    layer->addListener();
    
    return layer;
}

ui::Scale9Sprite* ElectricDreamsButton::createButtonBackground(std::string buttonText)
{
    Label* buttonLabel = Label::createWithTTF(buttonText, "fonts/azoomee.ttf", 90);
    buttonLabel->setColor(Color3B::BLACK);
    
    Rect spriteRect = Rect(0, 0, 196, 197);
    Rect capInsents = Rect(98, 98, 1, 1);
    
    ui::Scale9Sprite* newButton = ui::Scale9Sprite::create("res/modal/generic_button_slice_ready.png", spriteRect, capInsents);
    newButton->setContentSize(Size(buttonLabel->getContentSize().width+196, 197));
    newButton->setPosition(Vec2(newButton->getContentSize().width/2, newButton->getContentSize().height/2));
    
    buttonLabel->setPosition(newButton->getContentSize().width/2, newButton->getContentSize().height/2);
    
    newButton->addChild(buttonLabel);
    
    return newButton;
}

ElectricDreamsButton* ElectricDreamsButton::createNextButton()
{
    auto layer = ElectricDreamsButton::create();
    
    Sprite* nextButton = Sprite::create("res/login/next_btn.png");
    nextButton->setPosition(nextButton->getContentSize().width/2, nextButton->getContentSize().height/2);
    
    layer->setContentSize(nextButton->getContentSize());
    
    layer->addChild(nextButton);
    
    layer->addListener();
    
    return layer;
}

ElectricDreamsButton* ElectricDreamsButton::createBackButton()
{
    auto layer = ElectricDreamsButton::create();
    
    Sprite* backButton = Sprite::create("res/login/back_btn.png");
    backButton->setPosition(backButton->getContentSize().width/2, backButton->getContentSize().height/2);
    
    layer->setContentSize(backButton->getContentSize());
    
    layer->addChild(backButton);
    
    layer->addListener();
    
    return layer;
}

//---------------------- Listener Function -----------------------------

void ElectricDreamsButton::addListener()
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
            this->getDelegate()->buttonPressed(this);
        }
        
        return false;
    };

    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

//---------------------- public Functions After Setup -----------------------------

void ElectricDreamsButton::setCenterPosition(Vec2 position)
{
    this->setPosition(Vec2(position.x - this->getContentSize().width/2, position.y - this->getContentSize().height/2));
}
