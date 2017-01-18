#include "MultiButtonMessageBoxLayer.h"

#define MESSAGE_BOX_PADDING 100
#define MESSAGE_BOX_MINIMUM_WIDTH 1366
#define MESSAGE_BOX_MAXIMUM_WIDTH 2049

Layer* MultiButtonMessageBoxLayer::createMessageBox(std::string Title, std::string Body, std::vector<std::string> buttonTitleList, MultiButtonMessageBoxLayerDelegate* _delegate)
{
    auto layer = MultiButtonMessageBoxLayer::create();
    
    layer->setDelegate(_delegate);
    
    layer->createAndFadeInLayer();
    layer->createTitle(Title);
    layer->createBody(Body);
    layer->createButtons(buttonTitleList);
    layer->createMessageBackground();
    
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

void MultiButtonMessageBoxLayer::createTitle(std::string messageTitle)
{
    messageTitleLabel = Label::createWithTTF(messageTitle, "fonts/azoomee.ttf", 120);
    
    if(messageTitleLabel->getContentSize().width < (MESSAGE_BOX_MINIMUM_WIDTH - MESSAGE_BOX_PADDING * 2))
    {
        messageBoxWidth = MESSAGE_BOX_MINIMUM_WIDTH;
        underlineTitle();
    }
    else if(messageTitleLabel->getContentSize().width < (MESSAGE_BOX_MAXIMUM_WIDTH - MESSAGE_BOX_PADDING * 2))
    {
        messageBoxWidth = messageTitleLabel->getContentSize().width + (MESSAGE_BOX_PADDING * 2);
        underlineTitle();
    }
    else
    {
        //Due to Title being more than 2 lines, currently not set to be underlined.
        messageBoxWidth = MESSAGE_BOX_MAXIMUM_WIDTH;
        messageTitleLabel->setWidth(MESSAGE_BOX_MAXIMUM_WIDTH - MESSAGE_BOX_PADDING * 2);
    }
    
    messageTitleLabel->setPosition(backgroundLayer->getContentSize().width * 0.5, backgroundLayer->getContentSize().height * 0.75);
    messageTitleLabel->setColor(Color3B(28, 244, 244));
    messageTitleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    backgroundLayer->addChild(messageTitleLabel,2);
}

void MultiButtonMessageBoxLayer::underlineTitle()
{
    DrawNode* newDrawNode = DrawNode::create();
    newDrawNode->drawRect(Vec2(0, 10), Vec2(messageTitleLabel->getContentSize().width, 14), Color4F((28 * 255), (244 * 255), (244 * 255), 1));
    messageTitleLabel->addChild(newDrawNode);
}

void MultiButtonMessageBoxLayer::createBody(std::string messageBody)
{
    messageBodyLabel = Label::createWithTTF(messageBody, "fonts/azoomee.ttf", 90);
    messageBodyLabel->setWidth(messageBoxWidth - MESSAGE_BOX_PADDING * 2);
    messageBodyLabel->setPosition(backgroundLayer->getContentSize().width * 0.5, messageTitleLabel->getPositionY() - (messageTitleLabel->getContentSize().height/2) - MESSAGE_BOX_PADDING - (messageBodyLabel->getContentSize().height/2));
    messageBodyLabel->setColor(Color3B::WHITE);
    messageBodyLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    backgroundLayer->addChild(messageBodyLabel,2);
}

void MultiButtonMessageBoxLayer::createButtons(std::vector<std::string> buttonTitleList)
{
    float MessageBoxButtonSpace = messageBoxWidth/buttonTitleList.size();
    
    for(int i=0;i < buttonTitleList.size(); i++)
    {
        float buttonXValue = (backgroundLayer->getContentSize().width * 0.5) - (messageBoxWidth/2) + (MessageBoxButtonSpace * i) + (MessageBoxButtonSpace/2);
    
        auto _button = ElectricDreamsButton::createButtonWithText(buttonTitleList.at(i));
        _button->setCenterPosition(Vec2(buttonXValue, messageBodyLabel->getPositionY() - (messageBodyLabel->getContentSize().height/2) - MESSAGE_BOX_PADDING - (_button->getContentSize().height/2)));
        _button->setDelegate(this);
        backgroundLayer->addChild(_button, 2);
        
        buttonsList.push_back(_button);
    }
}

void MultiButtonMessageBoxLayer::createMessageBackground()
{
    float messageBoxHeight = messageTitleLabel->getContentSize().height + messageBodyLabel->getContentSize().height+buttonsList.at(0)->getContentSize().height + (4 * MESSAGE_BOX_PADDING);
    
    float messageBoxY = buttonsList.at(0)->getCenterPosition().y - (buttonsList.at(0)->getContentSize().height/2) - MESSAGE_BOX_PADDING;
    
    auto messageBoxLayer = LayerColor::create(Color4B::BLACK, messageBoxWidth, messageBoxHeight);
    messageBoxLayer->setPosition((visibleSize.width - messageBoxLayer->getContentSize().width)/2, messageBoxY);
    backgroundLayer->addChild(messageBoxLayer,1);
    
    DrawNode* newDrawNode = DrawNode::create();
    messageBoxLayer->addChild(newDrawNode);
    newDrawNode->drawRect(Vec2(0, 0), Vec2(messageBoxLayer->getContentSize().width, messageBoxLayer->getContentSize().height), Color4F((28 * 255), (244 * 255), (244 * 255), 1));
    
    newDrawNode->setLineWidth(4);
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
    for(int i=0;i < buttonsList.size(); i++)
    {
        if(buttonsList.at(i) == button)
        {
            //To enable call to delegate and avoid crash, schedule remove for after delegate call.
            this->scheduleOnce(schedule_selector(MultiButtonMessageBoxLayer::removeSelf), 0.1);
            this->getDelegate()->MultiButtonMessageBoxPressed(i);
        }
    }
}
