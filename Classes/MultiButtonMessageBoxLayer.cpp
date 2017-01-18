#include "MultiButtonMessageBoxLayer.h"

#define MESSAGE_BOX_PADDING 100
#define MESSAGE_BOX_MINIMUM_WIDTH 1366
#define MESSAGE_BOX_MAXIMUM_WIDTH 2049

Layer* MultiButtonMessageBoxLayer::createMessageBox(std::string Title, std::string Body, std::string buttontext)
{
    auto layer = MultiButtonMessageBoxLayer::create();
    
    layer->createMessageWithSingleButton(Title, Body, buttontext);
    
    return layer;
}

bool MultiButtonMessageBoxLayer::init()
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

void MultiButtonMessageBoxLayer::createAndFadeInLayer()
{
    backgroundLayer = LayerColor::create(Color4B(0,0,0,255),origin.x+ visibleSize.width, origin.y + visibleSize.height);
    
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    addListenerToBackgroundLayer();
}

void MultiButtonMessageBoxLayer::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) 
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

//---------------------- Message Box Functions------------------------

void MultiButtonMessageBoxLayer::createMessageWithSingleButton(std::string messageTitle, std::string messageBody, std::string buttonText)
{
    createAndFadeInLayer();
    createTitle(messageTitle);
    createBody(messageBody);
    createButton(buttonText);
    createMessageBackground();
}

void MultiButtonMessageBoxLayer::createTitle(std::string messageTitle)
{
    auto titleLabel = Label::createWithTTF(messageTitle, "fonts/azoomee.ttf", 120);
    
    if(titleLabel->getContentSize().width < (MESSAGE_BOX_MINIMUM_WIDTH - MESSAGE_BOX_PADDING * 2))
    {
        messageBoxWidth = MESSAGE_BOX_MINIMUM_WIDTH;
        underlineTitle(titleLabel);
    }
    else if(titleLabel->getContentSize().width < (MESSAGE_BOX_MAXIMUM_WIDTH - MESSAGE_BOX_PADDING * 2))
    {
        messageBoxWidth = titleLabel->getContentSize().width + (MESSAGE_BOX_PADDING * 2);
        underlineTitle(titleLabel);
    }
    else
    {
        //Due to Title being more than 2 lines, currently not set to be underlined.
        messageBoxWidth = MESSAGE_BOX_MAXIMUM_WIDTH;
        titleLabel->setWidth(MESSAGE_BOX_MAXIMUM_WIDTH - MESSAGE_BOX_PADDING * 2);
    }
    
    titleLabel->setPosition(backgroundLayer->getContentSize().width * 0.5, backgroundLayer->getContentSize().height * 0.75);
    titleLabel->setColor(Color3B(28, 244, 244));
    titleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    titleLabel->setName("messageTitle");
    titleLabel->setOpacity(0);
    backgroundLayer->addChild(titleLabel,2);
    
    titleLabel->runAction(FadeTo::create(1, 255));
}

void MultiButtonMessageBoxLayer::underlineTitle(Label* titleLabel)
{
    DrawNode* newDrawNode = DrawNode::create();
    newDrawNode->drawRect(Vec2(0, 10), Vec2(titleLabel->getContentSize().width, 14), Color4F((28 * 255), (244 * 255), (244 * 255), 1));
    titleLabel->addChild(newDrawNode);
}

void MultiButtonMessageBoxLayer::createBody(std::string messageBody)
{
    auto titleLabel = (Label*)backgroundLayer->getChildByName("messageTitle");
    
    auto bodyLabel = Label::createWithTTF(messageBody, "fonts/azoomee.ttf", 90);
    bodyLabel->setWidth(messageBoxWidth - MESSAGE_BOX_PADDING * 2);
    bodyLabel->setPosition(backgroundLayer->getContentSize().width * 0.5, titleLabel->getPositionY() - (titleLabel->getContentSize().height/2) - MESSAGE_BOX_PADDING - (bodyLabel->getContentSize().height/2));
    bodyLabel->setColor(Color3B::WHITE);
    bodyLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    bodyLabel->setName("messageBody");
    bodyLabel->setOpacity(0);
    backgroundLayer->addChild(bodyLabel,2);
    
    bodyLabel->runAction(FadeTo::create(1, 255));
}

void MultiButtonMessageBoxLayer::createButton(std::string buttonText)
{
    auto bodyLabel = (Label*)backgroundLayer->getChildByName("messageBody");
    
    auto _button = ElectricDreamsButton::createButtonWithText(buttonText);
    _button->setCenterPosition(Vec2(backgroundLayer->getContentSize().width * 0.5, bodyLabel->getPositionY() - (bodyLabel->getContentSize().height/2) - MESSAGE_BOX_PADDING - (_button->getContentSize().height/2)));
    _button->setDelegate(this);
    _button->setOpacity(0);
    _button->setName("messageButton");
    backgroundLayer->addChild(_button, 2);
    
    _button->runAction(FadeTo::create(1, 255));
}

void MultiButtonMessageBoxLayer::createMessageBackground()
{
    auto titleLabel = (Label*)backgroundLayer->getChildByName("messageTitle");
    auto bodyLabel = (Label*)backgroundLayer->getChildByName("messageBody");
    auto _button = (ElectricDreamsButton*)backgroundLayer->getChildByName("messageButton");
    
    float messageBoxHeight = titleLabel->getContentSize().height + bodyLabel->getContentSize().height+_button->getContentSize().height + (4 * MESSAGE_BOX_PADDING);
    
    float messageBoxY = _button->getCenterPosition().y - (_button->getContentSize().height/2) - MESSAGE_BOX_PADDING;
    
    auto messageBoxLayer = LayerColor::create(Color4B::BLACK, messageBoxWidth, messageBoxHeight);
    messageBoxLayer->setPosition((visibleSize.width - messageBoxLayer->getContentSize().width)/2, messageBoxY);
    messageBoxLayer->setOpacity(0);
    backgroundLayer->addChild(messageBoxLayer,1);
    
    DrawNode* newDrawNode = DrawNode::create();
    messageBoxLayer->addChild(newDrawNode);
    newDrawNode->drawRect(Vec2(0, 0), Vec2(messageBoxLayer->getContentSize().width, messageBoxLayer->getContentSize().height), Color4F((28 * 255), (244 * 255), (244 * 255), 1));
    
    newDrawNode->setLineWidth(4);
    
    messageBoxLayer->runAction(FadeTo::create(0.5, 255));
}
//---------------------- Actions -----------------

void MultiButtonMessageBoxLayer::removeSelf(float dt)
{
    if(this)
    {
        this->removeChild(backgroundLayer);
        this->removeFromParent();
    }
}

//----------------------- Delegate Functions ----------------------------

void MultiButtonMessageBoxLayer::buttonPressed(ElectricDreamsButton* button)
{
    
}
