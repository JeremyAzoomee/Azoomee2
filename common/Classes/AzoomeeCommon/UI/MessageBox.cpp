#include "MessageBox.h"
#include "../Strings.h"
#include "../Analytics/AnalyticsSingleton.h"
#include "ElectricDreamsTextStyles.h"
#include "ElectricDreamsDecoration.h"
#include "Style.h"

#include "MessageBoxLayers/MessageBoxTextLayer.h"

using namespace cocos2d;

#define MESSAGE_BOX_PADDING 100

#define MESSAGE_BOX_Z_ORDER 4000

namespace Azoomee
{
    
const char* const MessageBox::kOK = "OK";
const char* const MessageBox::kCancel = "Cancel";
const char* const MessageBox::kLogin = "Log in";
const char* const MessageBox::kSignUp = "Sign up";
const char* const MessageBox::kResetPassword = "Reset Password";
const char* const MessageBox::kReport ="Report";
const char* const MessageBox::kReset ="Reset";
const char* const MessageBox::kDelete ="Delete";

MessageBox* MessageBox::createWith(const std::string& Title, const std::string& Body, const std::vector<std::string>& buttonTitleList, MessageBoxDelegate* delegate)
{
    auto layer = MessageBox::create();
    
    if(delegate)
    {
        layer->setDelegate(delegate);
    }

    layer->windowLayer = MessageBoxTextLayer::createMessageBoxTextLayer(Title,Body,buttonTitleList,layer);
    layer->addChild(layer->windowLayer);
    
    return layer;
}

MessageBox* MessageBox::createWith(const std::string& Title, const std::string& Body, const std::string& Button, MessageBoxDelegate* delegate)
{
    auto layer = MessageBox::create();
    
    if(delegate)
    {
        layer->setDelegate(delegate);
    }
    
    std::vector<std::string> buttonsTitleList;
    buttonsTitleList.push_back(Button);
    
    layer->windowLayer = MessageBoxTextLayer::createMessageBoxTextLayer(Title,Body,buttonsTitleList,layer);
    layer->addChild(layer->windowLayer);
    
    return layer;
}
    
    MessageBox* MessageBox::createWith(const std::string& Title, const std::string& imagePath, const std::string& Body, const std::string& Button, MessageBoxDelegate* delegate)
{
    auto layer = MessageBox::create();
    
    if(delegate)
    {
        layer->setDelegate(delegate);
    }
    
    std::vector<std::string> buttonsTitleList;
    buttonsTitleList.push_back(Button);
    
    layer->windowLayer = MessageBoxTextLayer::createMessageBoxTextLayer(Title,Body,buttonsTitleList,layer);
    
    Sprite* image = Sprite::create(imagePath);
    image->setPosition(layer->windowLayer->getContentSize().width / 2, Director::getInstance()->getVisibleOrigin().y + 200);
    image->setScale(160 / image->getContentSize().width, 160 / image->getContentSize().height);
    layer->windowLayer->addChild(image);
    
    layer->addChild(layer->windowLayer);
    
    return layer;
}

MessageBox* MessageBox::createWith(long errorCode, MessageBoxDelegate* delegate)
{
    auto layer = MessageBox::create();
    
    if(delegate)
    {
        layer->setDelegate(delegate);
    }
    
    layer->windowLayer = MessageBoxTextLayer::createMessageBoxTextLayer(errorCode,layer);
    layer->addChild(layer->windowLayer);
                    
    return layer;
}

MessageBox* MessageBox::createWith(long errorCode, TextInputLayer* textInputToHide, MessageBoxDelegate* delegate)
{
    auto layer = MessageBox::create();
    
    layer->hideTextInput(textInputToHide);
    
    if(delegate)
    {
        layer->setDelegate(delegate);
    }
    
    layer->windowLayer = MessageBoxTextLayer::createMessageBoxTextLayer(errorCode,layer);
    layer->addChild(layer->windowLayer);
                    
    return layer;
}

void MessageBox::removeMessageBox()
{
    this->scheduleOnce(schedule_selector(MessageBox::removeSelf), 0.1);
}

bool MessageBox::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setName("MessageBoxLayer");
    createBackgroundLayer();
    addListenerToBackgroundLayer();
    
    return true;
}

//---------------------- Create Layer -----------------------------

void MessageBox::createBackgroundLayer()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    
    backgroundLayer = LayerColor::create(Style::Color_4B::awaitingPinLayer,currentRunningScene->getContentSize().width, currentRunningScene->getContentSize().height);
    this->addChild(backgroundLayer);
    
    Director::getInstance()->getRunningScene()->addChild(this, MESSAGE_BOX_Z_ORDER);
}

void MessageBox::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) 
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

//---------------------- Actions -----------------

void MessageBox::removeSelf(float dt)
{
    
    if(this)
    {
        this->removeChild(backgroundLayer);
        this->removeFromParent();
    }
}
    
void MessageBox::onSizeChanged()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    backgroundLayer->setContentSize(currentRunningScene->getContentSize());
    
    if(windowLayer)
    {
        dynamic_cast<MessageBoxLayer*>(windowLayer)->onSizeChanged();
    }
}

//-------------------- Object To Hide Functions ----------------------

void MessageBox::hideTextInput(TextInputLayer* textInputToHide)
{
    if(textInputToHide)
    {
        textInputToHide->setEditboxVisibility(false);
        savedTextInputToHide = textInputToHide;
    }
    else
        savedTextInputToHide = nullptr;
}

void MessageBox::UnHideTextInput()
{
    if(savedTextInputToHide)
    {
        savedTextInputToHide->setEditboxVisibility(true);
    }
}
    
void MessageBox::sendDelegateMessageBoxButtonPressed(const std::string& messageBoxTitle,const std::string& buttonTitle)
{
    this->scheduleOnce(schedule_selector(MessageBox::removeSelf), 0.1);
    UnHideTextInput();
    if(_delegate)
    {
        this->getDelegate()->MessageBoxButtonPressed(messageBoxTitle, buttonTitle);
    }
}
                    
}
