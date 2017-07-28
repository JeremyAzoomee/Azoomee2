#include "MessageBoxOnlineSafetySlidesLayer.h"
#include "../../Strings.h"
#include "../../Analytics/AnalyticsSingleton.h"
#include "../ElectricDreamsTextStyles.h"
#include "../ElectricDreamsDecoration.h"

using namespace cocos2d;

#define MESSAGE_BOX_PADDING 100

namespace Azoomee
{
    
Layer* MessageBoxOnlineSafetySlidesLayer::create(const std::map<std::string, std::string>& propertiesMap,Layer* parentLayer)
{
    auto layer = MessageBoxOnlineSafetySlidesLayer::create();
    
    layer->_parentLayer = parentLayer;
    layer->_propertiesMap =propertiesMap;
    
    std::string replaceChild1 = stringReplace(StringMgr::getInstance()->getStringForKey(CHAT_MESSAGE_BOXES_RESET_CHAT_TITLE), "%s1", layer->_propertiesMap["Child1"]);
    
    layer->_messageBoxTitle = stringReplace(replaceChild1, "%s2", layer->_propertiesMap["Child2"]);
    layer->_messageBoxBody = StringMgr::getInstance()->getStringForKey(CHAT_MESSAGE_BOXES_RESET_CHAT_BODY);
    
    layer->onSizeChanged();
    
    return layer;
}

bool MessageBoxOnlineSafetySlidesLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    setCurrentRunningSceneSize();
    setOrientation();
    
    return true;
}
    
void MessageBoxOnlineSafetySlidesLayer::setOrientation()
{
    if(currentRunningSceneSize.width < currentRunningSceneSize.height)
    {
        isLandscape = false;
        percentageOfScreenForBox = 0.9;
    }
    else
    {
        isLandscape = true;
        percentageOfScreenForBox = 0.7;
    }
}

//---------------------- Message Box Functions------------------------
  
void MessageBoxOnlineSafetySlidesLayer::createTitle()
{
    messageTitleLabel = createLabelMessageBoxTitle(_messageBoxTitle);
    messageTitleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    messageTitleLabel->setWidth(textMaxWidth);
}

void MessageBoxOnlineSafetySlidesLayer::createBody()
{
    messageBodyLabel = createLabelWith(_messageBoxBody, Style::Font::Regular, Style::Color::white, 64);
    messageBodyLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    messageBodyLabel->setWidth(textMaxWidth);
}
    
void MessageBoxOnlineSafetySlidesLayer::createSprite()
{
    oomeeSprite = Sprite::create("res/chat/ui/messageBoxes/thoughtfullOomee.png");
}

void MessageBoxOnlineSafetySlidesLayer::createButtons()
{
    onlineSafetyTipsButton = ElectricDreamsButton::createTextAsButtonAqua(StringMgr::getInstance()->getStringForKey(BUTTON_ONLINE_SAFETY_TIPS), 64, true);
    onlineSafetyTipsButton->setMixPanelButtonName("MessageBox-OnlineSafetyTips");
    onlineSafetyTipsButton->setDelegate(this);
    
    resetButton = ElectricDreamsButton::createButtonWithWidth(StringMgr::getInstance()->getStringForKey(BUTTON_RESET), onlineSafetyTipsButton->getContentSize().width);
    resetButton->setMixPanelButtonName("MessageBox-Reset");
    resetButton->setDelegate(this);
}

void MessageBoxOnlineSafetySlidesLayer::createCancelButton()
{
    cancelButton = ElectricDreamsButton::createWindowCloselButton();
    cancelButton->setMixPanelButtonName("messageBoxCancelButton");
    cancelButton->setDelegate(this);
}
    
//------------- LANDSCAPE SPECIFIC CREATION-------------

void MessageBoxOnlineSafetySlidesLayer::createMessageWindowLandscape()
{
    float windowHeight = cancelButton->getContentSize().height + messageTitleLabel->getContentSize().height + oomeeSprite->getContentSize().height + (4*MESSAGE_BOX_PADDING);
    
    windowLayer = createWindowLayer(currentRunningSceneSize.width * percentageOfScreenForBox, windowHeight);
    windowLayer->setPosition(currentRunningSceneSize.width/2- windowLayer->getContentSize().width/2,(currentRunningSceneSize.height - windowLayer->getContentSize().height) * 0.66);
    this->addChild(windowLayer);
}

void MessageBoxOnlineSafetySlidesLayer::addObjectsToWindowLandscape()
{
    float nextItemHeight = windowLayer->getContentSize().height-cancelButton->getContentSize().height*.75;
    
    cancelButton->setCenterPosition(Vec2(windowLayer->getContentSize().width-cancelButton->getContentSize().width*0.75, nextItemHeight));
    windowLayer->addChild(cancelButton);
    
    // Add Title
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height/2 - messageTitleLabel->getContentSize().height/2;
    messageTitleLabel->setPosition(windowLayer->getContentSize().width/2, nextItemHeight);
    windowLayer->addChild(messageTitleLabel);

    // Add OomeeSprite
    nextItemHeight = nextItemHeight- MESSAGE_BOX_PADDING-messageTitleLabel->getContentSize().height/2 - oomeeSprite->getContentSize().height/2;
    oomeeSprite->setPosition(windowLayer->getContentSize().width/4,nextItemHeight);
    windowLayer->addChild(oomeeSprite);
    
    // Add Body
    messageBodyLabel->setWidth(textMaxWidth/2);
    nextItemHeight = oomeeSprite->getPositionY()+oomeeSprite->getContentSize().height/2-messageBodyLabel->getContentSize().height/2;
    messageBodyLabel->setPosition(windowLayer->getContentSize().width*.75, nextItemHeight);
    windowLayer->addChild(messageBodyLabel);
    
    // Add online Safety Tips Button
    nextItemHeight = nextItemHeight- MESSAGE_BOX_PADDING/4- messageBodyLabel->getContentSize().height/2 - onlineSafetyTipsButton->getContentSize().height/2;
    onlineSafetyTipsButton->setCenterPosition(Vec2(windowLayer->getContentSize().width*.75, nextItemHeight));
    windowLayer->addChild(onlineSafetyTipsButton);
    
    // Add Reset Button
    nextItemHeight = oomeeSprite->getPositionY()-oomeeSprite->getContentSize().height/2;
    resetButton->setCenterPosition(Vec2(windowLayer->getContentSize().width*.75, nextItemHeight));
    windowLayer->addChild(resetButton);
}
    
//------------- PORTRAIT SPECIFIC CREATION-------------
    
void MessageBoxOnlineSafetySlidesLayer::createMessageWindowPortrait()
{
    float windowHeight = cancelButton->getContentSize().height + messageTitleLabel->getContentSize().height  + messageBodyLabel->getContentSize().height + onlineSafetyTipsButton->getContentSize().height + resetButton->getContentSize().height+oomeeSprite->getContentSize().height + (5*MESSAGE_BOX_PADDING);
    
    windowLayer = createWindowLayer(currentRunningSceneSize.width * percentageOfScreenForBox, windowHeight);
    windowLayer->setPosition(currentRunningSceneSize.width/2- windowLayer->getContentSize().width/2,(currentRunningSceneSize.height - windowLayer->getContentSize().height) * 0.66);
    this->addChild(windowLayer);
}
    
void MessageBoxOnlineSafetySlidesLayer::addObjectsToWindowPortrait()
{
    float nextItemHeight = windowLayer->getContentSize().height-cancelButton->getContentSize().height*.75;
    
    cancelButton->setCenterPosition(Vec2(windowLayer->getContentSize().width-cancelButton->getContentSize().width*0.75, nextItemHeight));
    windowLayer->addChild(cancelButton);
    
    // Add OomeeSprite
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height/2 - oomeeSprite->getContentSize().height/2;
    oomeeSprite->setPosition(windowLayer->getContentSize().width/2,nextItemHeight);
    windowLayer->addChild(oomeeSprite);
    
    // Add Title
    nextItemHeight = nextItemHeight- MESSAGE_BOX_PADDING-oomeeSprite->getContentSize().height/2 - messageTitleLabel->getContentSize().height/2;
    messageTitleLabel->setPosition(windowLayer->getContentSize().width/2, nextItemHeight);
    windowLayer->addChild(messageTitleLabel);
    
    // Add Body
    nextItemHeight = nextItemHeight- MESSAGE_BOX_PADDING-messageTitleLabel->getContentSize().height/2 - messageBodyLabel->getContentSize().height/2;
    messageBodyLabel->setPosition(windowLayer->getContentSize().width/2, nextItemHeight);
    windowLayer->addChild(messageBodyLabel);
    
    // Add online Safety Tips Button
    nextItemHeight = nextItemHeight- MESSAGE_BOX_PADDING/4- messageBodyLabel->getContentSize().height/2 - onlineSafetyTipsButton->getContentSize().height/2;
    onlineSafetyTipsButton->setCenterPosition(Vec2(windowLayer->getContentSize().width/2, nextItemHeight));
    windowLayer->addChild(onlineSafetyTipsButton);
    
    // Add Reset Button
    nextItemHeight = nextItemHeight- MESSAGE_BOX_PADDING - onlineSafetyTipsButton->getContentSize().height/2 - resetButton->getContentSize().height/2;
    resetButton->setCenterPosition(Vec2(windowLayer->getContentSize().width/2, nextItemHeight));
    windowLayer->addChild(resetButton);
}

//---------------------- Actions -----------------
    
void MessageBoxOnlineSafetySlidesLayer::onSizeChanged()
{
    setCurrentRunningSceneSize();
    setOrientation();
    
    if(windowLayer)
        windowLayer->removeAllChildren();
    
    textMaxWidth = currentRunningSceneSize.width*percentageOfScreenForBox - MESSAGE_BOX_PADDING*2;

    createTitle();
    createBody();
    createButtons();
    createCancelButton();
    createSprite();
    
    if(isLandscape)
    {
        createMessageWindowLandscape();
        addObjectsToWindowLandscape();
    }
    else
    {
        createMessageWindowPortrait();
        addObjectsToWindowPortrait();
    }

}

//----------------------- Delegate Functions ----------------------------

void MessageBoxOnlineSafetySlidesLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == cancelButton)
        dynamic_cast<MessageBox*>(_parentLayer)->sendDelegateMessageBoxButtonPressed(_messageBoxTitle, "Cancel");
    else if(button == onlineSafetyTipsButton)
    {
        
    }
    else if(button == resetButton)
        dynamic_cast<MessageBox*>(_parentLayer)->sendDelegateMessageBoxButtonPressed(_messageBoxTitle, "Reset");
}
    
}
