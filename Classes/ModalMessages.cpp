#include "ModalMessages.h"
#include "ui/UIEditBox/UIEditBox.h"

USING_NS_CC;

#define MESSAGE_BOX_PADDING 100
#define MESSAGE_BOX_WIDTH 1366

static ModalMessages *_sharedModalMessages = NULL;

ModalMessages* ModalMessages::getInstance()
{
    if (! _sharedModalMessages)
    {
        _sharedModalMessages = new ModalMessages();
        _sharedModalMessages->init();
    }
    
    return _sharedModalMessages;
}

ModalMessages::~ModalMessages(void)
{
}

bool ModalMessages::init(void)
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

void ModalMessages::createAndFadeInLayer()
{
    loadingLayer = LayerColor::create(Color4B(0,0,0,150), visibleSize.width, visibleSize.height);
    loadingLayer->setPosition(origin.x, origin.y);
    loadingLayer->setOpacity(0);
    Director::getInstance()->getRunningScene()->addChild(loadingLayer);
    
    addListenerToBackgroundLayer();
    
    loadingLayer->runAction(FadeTo::create(0.5, 200));
}

void ModalMessages::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), loadingLayer);
}

void ModalMessages::removeLayer()
{
    if(loadingLayer) //This might be called when loading is not active, so better to check first
    {
        Director::getInstance()->getRunningScene()->removeChild(loadingLayer);
    }
}

void ModalMessages::startLoading()
{
    createAndFadeInLayer();
    
    //ADDING SOME TEMPORARY CIRCLES AS LOADING, NOT FINAL YET
    
    for(int i = 0; i < 3; i++)
    {
        auto loadingCircle = Sprite::create("res/modal/loading.png");
        loadingCircle->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        loadingCircle->setOpacity(0);
        loadingCircle->setRotation(RandomHelper::random_int(0, 360));
        loadingCircle->setScale(0.6 + i * 0.2);
        
        loadingLayer->addChild(loadingCircle);
        
        int direction = 1;
        if(CCRANDOM_0_1() < 0.5) direction = -1;
        
        loadingCircle->runAction(RepeatForever::create(RotateBy::create(CCRANDOM_0_1() + 1, 360 * direction)));
        loadingCircle->runAction(FadeTo::create(0.5, 255));
    }
}

void ModalMessages::stopLoading()
{
    this->removeLayer();
}

void ModalMessages::createMessageWithSingleButton(std::string messageTitle, std::string messageBody, std::string buttonText)
{
    //ALL MESSAGEBOX CODE NEEDS FINAL WAY OF WORKING
    //ALSO NEEDS TO TAKE COLOURS ETC FROM CONFIG CLASS WHEN BRANCHED
    createAndFadeInLayer();
    createAndFadeInTitle(messageTitle);
    createAndFadeInBody(messageBody);
    createAndFadeInButton(buttonText);
    createAndFadeInMessageBackground();
}

void ModalMessages::createAndFadeInTitle(std::string messageTitle)
{
    auto titleLabel = Label::createWithTTF(messageTitle, "fonts/azoomee.ttf", 130);
    titleLabel->setWidth(MESSAGE_BOX_WIDTH - MESSAGE_BOX_PADDING * 2);
    titleLabel->setPosition(loadingLayer->getContentSize().width * 0.5, loadingLayer->getContentSize().height * 0.75);
    titleLabel->setColor(Color3B::BLACK);
    titleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    titleLabel->setName("messageTitle");
    titleLabel->setOpacity(0);
    loadingLayer->addChild(titleLabel,2);
    
    titleLabel->runAction(FadeTo::create(1, 255));
}

void ModalMessages::createAndFadeInBody(std::string messageBody)
{
    auto titleLabel = (Label*)loadingLayer->getChildByName("messageTitle");
    
    auto bodyLabel = Label::createWithTTF(messageBody, "fonts/azoomee.ttf", 90);
    bodyLabel->setWidth(MESSAGE_BOX_WIDTH - MESSAGE_BOX_PADDING * 2);
    bodyLabel->setPosition(loadingLayer->getContentSize().width * 0.5, titleLabel->getPositionY() - (titleLabel->getContentSize().height/2) - MESSAGE_BOX_PADDING - (bodyLabel->getContentSize().height/2));
    bodyLabel->setColor(Color3B::WHITE);
    bodyLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    bodyLabel->setName("messageBody");
    bodyLabel->setOpacity(0);
    loadingLayer->addChild(bodyLabel,2);
    
    bodyLabel->runAction(FadeTo::create(1, 255));
}

void ModalMessages::createAndFadeInButton(std::string buttonText)
{
    auto bodyLabel = (Label*)loadingLayer->getChildByName("messageBody");
    
    auto _button = ui::Button::create("res/modal/tempButtonBackground.png");
    _button->setTitleFontName("fonts/azoomee.ttf");
    _button->setTitleFontSize(90);
    _button->setColor(Color3B::WHITE);
    _button->setTitleText(buttonText);
    _button->setPosition(Vec2(loadingLayer->getContentSize().width * 0.5, bodyLabel->getPositionY() - (bodyLabel->getContentSize().height/2) - MESSAGE_BOX_PADDING - (_button->getContentSize().height/2)));
    _button->setOpacity(0);
    _button->setName("messageButton");
    
    _button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                this->removeLayer();
                break;
            default:
                break;
        }
    });
    
    loadingLayer->addChild(_button,2);
    
    _button->runAction(FadeTo::create(1, 255));
}

void ModalMessages::createAndFadeInMessageBackground()
{
    auto titleLabel = (Label*)loadingLayer->getChildByName("messageTitle");
    auto bodyLabel = (Label*)loadingLayer->getChildByName("messageBody");
    auto _button = (Sprite*)loadingLayer->getChildByName("messageButton");
    
    float messageBoxHeight = titleLabel->getContentSize().height + bodyLabel->getContentSize().height+_button->getContentSize().height + (4 * MESSAGE_BOX_PADDING);
    
    float messageBoxY = _button->getPositionY() - (_button->getContentSize().height/2) - MESSAGE_BOX_PADDING;
    
    auto messageBoxLayer = LayerColor::create(Color4B(28, 200, 200,255), MESSAGE_BOX_WIDTH, visibleSize.height / 2);
    messageBoxLayer->setContentSize(Size(MESSAGE_BOX_WIDTH, messageBoxHeight));
    messageBoxLayer->setPosition(visibleSize.width / 4, messageBoxY);
    messageBoxLayer->setOpacity(0);
    loadingLayer->addChild(messageBoxLayer,1);
    
    messageBoxLayer->runAction(FadeTo::create(0.5, 255));
    
}
