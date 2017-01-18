#include "ModalMessages.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "StringStorage.h"
#include "LoginScene.h"
#include "OnboardingScene.h"

USING_NS_CC;

#define MESSAGE_BOX_PADDING 100
#define MESSAGE_BOX_MINIMUM_WIDTH 1366
#define MESSAGE_BOX_MAXIMUM_WIDTH 2049

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

//---------------------- Message Box Functions------------------------

void ModalMessages::createMessageWithSingleButton(std::string messageTitle, std::string messageBody, std::string buttonText)
{
    createAndFadeInLayer();
    createAndFadeInTitle(messageTitle);
    createAndFadeInBody(messageBody);
    createAndFadeInButton(buttonText);
    createAndFadeInMessageBackground();
}

void ModalMessages::createAndFadeInTitle(std::string messageTitle)
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
    
    titleLabel->setPosition(loadingLayer->getContentSize().width * 0.5, loadingLayer->getContentSize().height * 0.75);
    titleLabel->setColor(Color3B(28, 244, 244));
    titleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    titleLabel->setName("messageTitle");
    titleLabel->setOpacity(0);
    loadingLayer->addChild(titleLabel,2);
    
    titleLabel->runAction(FadeTo::create(1, 255));
}

void ModalMessages::underlineTitle(Label* titleLabel)
{
    DrawNode* newDrawNode = DrawNode::create();
    newDrawNode->drawRect(Vec2(0, 10), Vec2(titleLabel->getContentSize().width, 14), Color4F((28 * 255), (244 * 255), (244 * 255), 1));
    titleLabel->addChild(newDrawNode);
}

void ModalMessages::createAndFadeInBody(std::string messageBody)
{
    auto titleLabel = (Label*)loadingLayer->getChildByName("messageTitle");
    
    auto bodyLabel = Label::createWithTTF(messageBody, "fonts/azoomee.ttf", 90);
    bodyLabel->setWidth(messageBoxWidth - MESSAGE_BOX_PADDING * 2);
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
    
    auto _button = ElectricDreamsButton::createButtonWithText(buttonText);
    _button->setCenterPosition(Vec2(loadingLayer->getContentSize().width * 0.5, bodyLabel->getPositionY() - (bodyLabel->getContentSize().height/2) - MESSAGE_BOX_PADDING - (_button->getContentSize().height/2)));
    _button->setDelegate(this);
    _button->setOpacity(0);
    _button->setName("messageButton");
    loadingLayer->addChild(_button, 2);
    
    _button->runAction(FadeTo::create(1, 255));
}

void ModalMessages::createAndFadeInMessageBackground()
{
    auto titleLabel = (Label*)loadingLayer->getChildByName("messageTitle");
    auto bodyLabel = (Label*)loadingLayer->getChildByName("messageBody");
    auto _button = (ElectricDreamsButton*)loadingLayer->getChildByName("messageButton");
    
    float messageBoxHeight = titleLabel->getContentSize().height + bodyLabel->getContentSize().height+_button->getContentSize().height + (4 * MESSAGE_BOX_PADDING);
    
    float messageBoxY = _button->getCenterPosition().y - (_button->getContentSize().height/2) - MESSAGE_BOX_PADDING;
    
    auto messageBoxLayer = LayerColor::create(Color4B::BLACK, messageBoxWidth, messageBoxHeight);
    messageBoxLayer->setPosition((visibleSize.width - messageBoxLayer->getContentSize().width)/2, messageBoxY);
    messageBoxLayer->setOpacity(0);
    loadingLayer->addChild(messageBoxLayer,1);
    
    DrawNode* newDrawNode = DrawNode::create();
    messageBoxLayer->addChild(newDrawNode);
    newDrawNode->drawRect(Vec2(0, 0), Vec2(messageBoxLayer->getContentSize().width, messageBoxLayer->getContentSize().height), Color4F((28 * 255), (244 * 255), (244 * 255), 1));
    
    newDrawNode->setLineWidth(4);

    messageBoxLayer->runAction(FadeTo::create(0.5, 255));
}

//------------ PREVIEW MODE MESSAGE BOX ----------------
void ModalMessages::createPreviewLoginSignupMessageBox()
{
    if(messageBoxButtonTitles.size() == 0)
    {
        messageBoxButtonTitles.push_back(LOGIN_BUTTON_TEXT);
        messageBoxButtonTitles.push_back(SIGNUP_BUTTON_TEXT);
        messageBoxButtonTitles.push_back(CANCEL_BUTTON_TEXT);
    }
    
    MultiButtonMessageBoxLayer::createMessageBox(PREVIEW_MESSAGEBOX_TITLE, PREVIEW_MESSAGEBOX_BODY, messageBoxButtonTitles, this);
}

// ------------- DELEGATE FUNCTIONS --------------------

void ModalMessages::buttonPressed(ElectricDreamsButton* button)
{
    auto messageButton = (ElectricDreamsButton*)loadingLayer->getChildByName("messageButton");
    
    if(button == messageButton)
        this->removeLayer();
}

void ModalMessages::MultiButtonMessageBoxPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(messageBoxTitle == PREVIEW_MESSAGEBOX_TITLE)
    {
        if(buttonTitle == LOGIN_BUTTON_TEXT)
        {
            Scene *loginScene = LoginScene::createScene(0);
            Director::getInstance()->replaceScene(loginScene);
        }
        else if(buttonTitle == SIGNUP_BUTTON_TEXT)
        {
            Scene *onboardingScene = OnboardingScene::createScene(0);
            Director::getInstance()->replaceScene(onboardingScene);
        }
    }
}
