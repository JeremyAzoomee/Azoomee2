#include "MessageBox.h"
#include "../Strings.h"
#include "../Analytics/AnalyticsSingleton.h"
#include "ElectricDreamsTextStyles.h"
#include "ElectricDreamsDecoration.h"
#include "Style.h"

#include "MessageBoxLayers/MessageBoxTextLayer.h"
#include "MessageBoxLayers/ChatMessageLayers/MessageBoxChatResetModerationLayer.h"
#include "MessageBoxLayers/ChatMessageLayers/MessageBoxChatReportChatLayer.h"
#include "MessageBoxLayers/MessageBoxOnlineSafetySlidesLayer.h"

using namespace cocos2d;

#define MESSAGE_BOX_PADDING 100

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

MessageBox* MessageBox::createWith(const std::string& Title, const std::string& Body, const std::vector<std::string>& buttonTitleList, MessageBoxDelegate* _delegate)
{
    auto layer = MessageBox::create();
    
    if(_delegate)
        layer->setDelegate(_delegate);

    layer->windowLayer = MessageBoxTextLayer::createMessageBoxTextLayer(Title,Body,buttonTitleList,layer);
    layer->addChild(layer->windowLayer);
    
    return layer;
}

MessageBox* MessageBox::createWith(const std::string& Title, const std::string& Body, const std::string& Button, MessageBoxDelegate* _delegate)
{
    auto layer = MessageBox::create();
    
    if(_delegate)
        layer->setDelegate(_delegate);
    
    std::vector<std::string> buttonsTitleList;
    buttonsTitleList.push_back(Button);
    
    layer->windowLayer = MessageBoxTextLayer::createMessageBoxTextLayer(Title,Body,buttonsTitleList,layer);
    layer->addChild(layer->windowLayer);
    
    return layer;
}

MessageBox* MessageBox::createWith(long errorCode, MessageBoxDelegate* _delegate)
{
    auto layer = MessageBox::create();
    
    if(_delegate)
        layer->setDelegate(_delegate);
    
    layer->windowLayer = MessageBoxTextLayer::createMessageBoxTextLayer(errorCode,layer);
    layer->addChild(layer->windowLayer);
                    
    return layer;
}

MessageBox* MessageBox::createWith(long errorCode, TextInputLayer* textInputToHide, MessageBoxDelegate* _delegate)
{
    auto layer = MessageBox::create();
    
    layer->hideTextInput(textInputToHide);
    
    if(_delegate)
        layer->setDelegate(_delegate);
    
    layer->windowLayer = MessageBoxTextLayer::createMessageBoxTextLayer(errorCode,layer);
    layer->addChild(layer->windowLayer);
                    
    return layer;
}
    
MessageBox* MessageBox::createWithLayer(MessageBoxLayerEnum messageBoxLayer, MessageBoxDelegate* _delegate)
{
    auto layer = MessageBox::create();
    
    if(_delegate)
        layer->setDelegate(_delegate);
    
    layer->windowLayer = layer->initLayer(messageBoxLayer,{},layer);
    
    layer->addChild(layer->windowLayer);
    
    return layer;
}
    
MessageBox* MessageBox::createWithLayer(MessageBoxLayerEnum messageBoxLayer, const std::map<std::string, std::string>& propertiesMap, MessageBoxDelegate* _delegate)
{
    auto layer = MessageBox::create();
    
    if(_delegate)
        layer->setDelegate(_delegate);
    
    layer->windowLayer = layer->initLayer(messageBoxLayer,propertiesMap,layer);
    
    layer->addChild(layer->windowLayer);
    
    return layer;
}
    
Layer* MessageBox::initLayer(MessageBoxLayerEnum messageBoxLayer, const std::map<std::string, std::string>& propertiesMap, Layer* newLayer)
{
    switch (messageBoxLayer) {
        case ChatReportForModeration:
            return MessageBoxChatReportChatLayer::create(newLayer);
            break;
        case OnlineSafetySlidesAdult:
            return MessageBoxOnlineSafetySlidesLayer::createForParent(newLayer);
            break;
        case OnlineSafetySlidesChild:
            return MessageBoxOnlineSafetySlidesLayer::createForChild(newLayer);
            break;
        case ChatResetModeration:
            return MessageBoxChatResetModerationLayer::create(propertiesMap,newLayer);
            break;
            
        default:
            break;
    }
    return nullptr;
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
    
    Director::getInstance()->getRunningScene()->addChild(this);
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
        dynamic_cast<MessageBoxLayer*>(windowLayer)->onSizeChanged();
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
        savedTextInputToHide->setEditboxVisibility(true);
}
    
void MessageBox::sendDelegateMessageBoxButtonPressed(const std::string& messageBoxTitle,const std::string& buttonTitle)
{
    this->scheduleOnce(schedule_selector(MessageBox::removeSelf), 0.1);
    UnHideTextInput();
    if(_delegate)
        this->getDelegate()->MessageBoxButtonPressed(messageBoxTitle, buttonTitle);
}
                    
}
