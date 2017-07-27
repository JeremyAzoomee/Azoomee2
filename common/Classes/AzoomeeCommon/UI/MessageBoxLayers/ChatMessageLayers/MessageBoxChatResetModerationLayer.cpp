#include "MessageBoxChatResetModerationLayer.h"
#include "../../../Strings.h"
#include "../../../Analytics/AnalyticsSingleton.h"
#include "../../ElectricDreamsTextStyles.h"
#include "../../ElectricDreamsDecoration.h"

using namespace cocos2d;

#define MESSAGE_BOX_PADDING 100

namespace Azoomee
{
    
Layer* MessageBoxChatResetModerationLayer::create(Layer* parentLayer)
{
    auto layer = MessageBoxChatResetModerationLayer::create();
    
    layer->_parentLayer = parentLayer;
    
    return layer;
}

bool MessageBoxChatResetModerationLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    setCurrentRunningSceneSize();
    setOrientation();
    
    return true;
}
    
void MessageBoxChatResetModerationLayer::setOrientation()
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
  
void MessageBoxChatResetModerationLayer::createTitle()
{
    messageTitleLabel = createLabelMessageBoxTitle(_messageBoxTitle);
    messageTitleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    messageTitleLabel->setWidth(textMaxWidth);
}

void MessageBoxChatResetModerationLayer::createBody()
{
    messageBodyLabel = createLabelMessageBoxBody(_messageBoxBody);
    messageBodyLabel->setWidth(textMaxWidth);
    
}
    
void MessageBoxChatResetModerationLayer::createSprite()
{
    oomeeSprite = Sprite::create("res/chat/ui/messageBoxes/thoughtfullOomee.png");
}

void MessageBoxChatResetModerationLayer::createButtons()
{
    onlineSafetyTipsButton = ElectricDreamsButton::createTextAsButtonAqua("online safety tips", 64, true);
    onlineSafetyTipsButton->setMixPanelButtonName("MessageBox-OnlineSafetyTips");
    resetButton = ElectricDreamsButton::createButtonWithWidth("Reset", onlineSafetyTipsButton->getContentSize().width);
    resetButton->setMixPanelButtonName("MessageBox-Reset");
}

void MessageBoxChatResetModerationLayer::createCancelButton()
{
    cancelButton = ElectricDreamsButton::createWindowCloselButton();
    cancelButton->setMixPanelButtonName("messageBoxCancelButton");
    cancelButton->setDelegate(this);
}
    
//------------- LANDSCAPE SPECIFIC CREATION-------------

void MessageBoxChatResetModerationLayer::createMessageWindowLandscape()
{
    float windowHeight = cancelButton->getContentSize().height + messageTitleLabel->getContentSize().height *3 + oomeeSprite->getContentSize().height;
    
    windowLayer = createWindowLayer(currentRunningSceneSize.width * percentageOfScreenForBox, windowHeight);
    windowLayer->setPosition(currentRunningSceneSize.width/2- windowLayer->getContentSize().width/2,(currentRunningSceneSize.height - windowLayer->getContentSize().height) * 0.66);
    this->addChild(windowLayer);
}

void MessageBoxChatResetModerationLayer::addObjectsToWindowLandscape()
{
    float nextItemHeight = windowLayer->getContentSize().height-cancelButton->getContentSize().height*.75;
    
    cancelButton->setCenterPosition(Vec2(windowLayer->getContentSize().width-cancelButton->getContentSize().width*0.75, nextItemHeight));
    windowLayer->addChild(cancelButton);
    
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height*.25 - messageTitleLabel->getContentSize().height/2;
    
    /*messageTitleLabel->setPosition(windowLayer->getContentSize().width/2, nextItemHeight);
    windowLayer->addChild(messageTitleLabel);
    
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height - messageTitleLabel->getContentSize().height/2 - scrollView->getContentSize().height/2;
    
    scrollView->setPosition(Vec2(windowLayer->getContentSize().width/2, nextItemHeight));
    windowLayer->addChild(scrollView);
    
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height - scrollView->getContentSize().height/2 - buttonsList.at(0)->getContentSize().height/2;
    
    positionButtonsBasedOnWidth(nextItemHeight);*/
}
    
//------------- PORTRAIT SPECIFIC CREATION-------------
    
void MessageBoxChatResetModerationLayer::createMessageWindowPortrait()
{
    float windowHeight = cancelButton->getContentSize().height + messageTitleLabel->getContentSize().height +  + messageBodyLabel->getContentSize().height + onlineSafetyTipsButton->getContentSize().height + resetButton->getContentSize().height;
    
    windowLayer = createWindowLayer(currentRunningSceneSize.width * percentageOfScreenForBox, windowHeight);
    windowLayer->setPosition(currentRunningSceneSize.width/2- windowLayer->getContentSize().width/2,(currentRunningSceneSize.height - windowLayer->getContentSize().height) * 0.66);
    this->addChild(windowLayer);
}
    
void MessageBoxChatResetModerationLayer::addObjectsToWindowPortrait()
{
    float nextItemHeight = windowLayer->getContentSize().height-cancelButton->getContentSize().height*.75;
    
    cancelButton->setCenterPosition(Vec2(windowLayer->getContentSize().width-cancelButton->getContentSize().width*0.75, nextItemHeight));
    windowLayer->addChild(cancelButton);
    
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height*.25 - messageTitleLabel->getContentSize().height/2;
    
    messageTitleLabel->setPosition(windowLayer->getContentSize().width/2, nextItemHeight);
    windowLayer->addChild(messageTitleLabel);
    
    /*nextItemHeight = nextItemHeight-cancelButton->getContentSize().height - messageTitleLabel->getContentSize().height/2 - scrollView->getContentSize().height/2;
    
    scrollView->setPosition(Vec2(windowLayer->getContentSize().width/2, nextItemHeight));
    windowLayer->addChild(scrollView);
    
    nextItemHeight = nextItemHeight-cancelButton->getContentSize().height - scrollView->getContentSize().height/2 - buttonsList.at(0)->getContentSize().height/2;
    
    positionButtonsBasedOnWidth(nextItemHeight);*/
}

//---------------------- Actions -----------------
    
void MessageBoxChatResetModerationLayer::onSizeChanged()
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

void MessageBoxChatResetModerationLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == cancelButton)
    {
        onCancelPressed();
    }
    else if(button == onlineSafetyTipsButton)
    {
        
    }
    else if(button == resetButton)
    {
        
    }
}
  
void MessageBoxChatResetModerationLayer::onCancelPressed()
{
     dynamic_cast<MessageBox*>(_parentLayer)->sendDelegateMessageBoxButtonPressed(_messageBoxTitle, "Cancel");
}
    
}
