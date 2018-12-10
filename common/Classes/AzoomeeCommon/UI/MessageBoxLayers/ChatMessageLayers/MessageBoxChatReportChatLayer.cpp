#include "MessageBoxChatReportChatLayer.h"
#include "../../../Strings.h"
#include "../../../Analytics/AnalyticsSingleton.h"
#include "../../ElectricDreamsTextStyles.h"
#include "../../ElectricDreamsDecoration.h"


using namespace cocos2d;

#define MESSAGE_BOX_PADDING 100

namespace Azoomee
{
    
Layer* MessageBoxChatReportChatLayer::create(Layer* parentLayer)
{
    auto layer = MessageBoxChatReportChatLayer::create();
    
    layer->_parentLayer = parentLayer;
    layer->_messageBoxTitle = _("Do you really want to report this chat to your parents?");
    
    layer->onSizeChanged();
    
    return layer;
}

bool MessageBoxChatReportChatLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    setCurrentRunningSceneSize();
    setOrientation();
    
    return true;
}
    
void MessageBoxChatReportChatLayer::setOrientation()
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
  
void MessageBoxChatReportChatLayer::createTitle()
{
    messageTitleLabel = createLabelMessageBoxTitle(_messageBoxTitle);
    messageTitleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    messageTitleLabel->setWidth(textMaxWidth);
}
    
void MessageBoxChatReportChatLayer::createSprite()
{
    oomeeSprite = Sprite::create("res/chat/ui/messageBoxes/Oomee_Report_Chat.png");
}

void MessageBoxChatReportChatLayer::createButtons()
{
    onlineSafetyTipsButton = ElectricDreamsButton::createTextAsButtonAqua(_("online safety tips"), 64, true);
    onlineSafetyTipsButton->setMixPanelButtonName("MessageBox-OnlineSafetyTips");
    onlineSafetyTipsButton->setDelegate(this);
    
    reportButton = ElectricDreamsButton::createButtonWithWidth(_("Report"), onlineSafetyTipsButton->getContentSize().width);
    reportButton->setMixPanelButtonName("MessageBox-Report");
    reportButton->setDelegate(this);
}

void MessageBoxChatReportChatLayer::createCancelButton()
{
    cancelButton = ElectricDreamsButton::createWindowCloseButton();
    cancelButton->setMixPanelButtonName("messageBoxCancelButton");
    cancelButton->setDelegate(this);
}
    
//------------- LANDSCAPE SPECIFIC CREATION-------------

void MessageBoxChatReportChatLayer::createMessageWindowLandscape()
{
    float windowHeight = cancelButton->getContentSize().height + messageTitleLabel->getContentSize().height + oomeeSprite->getContentSize().height + reportButton->getContentSize().height + (4*MESSAGE_BOX_PADDING);
    if(currentRunningSceneSize.width / currentRunningSceneSize.height >= 2)
    {
        windowHeight -= (2*MESSAGE_BOX_PADDING);
    }
    windowLayer = createWindowLayer(currentRunningSceneSize.width * percentageOfScreenForBox, windowHeight);
    windowLayer->setPosition(currentRunningSceneSize.width/2- windowLayer->getContentSize().width/2,(currentRunningSceneSize.height - windowLayer->getContentSize().height) * 0.66);
    this->addChild(windowLayer);
}

void MessageBoxChatReportChatLayer::addObjectsToWindowLandscape()
{
    bool removePadding = currentRunningSceneSize.width / currentRunningSceneSize.height >= 2 ? true : false;
    
    float nextItemHeight = windowLayer->getContentSize().height-cancelButton->getContentSize().height*.75;
    
    cancelButton->setCenterPosition(Vec2(windowLayer->getContentSize().width-cancelButton->getContentSize().width*0.75, nextItemHeight));
    windowLayer->addChild(cancelButton);
    
    // Add OomeeSprite
    nextItemHeight = nextItemHeight - oomeeSprite->getContentSize().height/2 - (removePadding ? 0 : cancelButton->getContentSize().height/2);
    oomeeSprite->setPosition(windowLayer->getContentSize().width/2,nextItemHeight);
    windowLayer->addChild(oomeeSprite);
    
    // Add Title
    nextItemHeight = nextItemHeight- MESSAGE_BOX_PADDING-oomeeSprite->getContentSize().height/2 - messageTitleLabel->getContentSize().height/2;
    messageTitleLabel->setPosition(windowLayer->getContentSize().width/2, nextItemHeight);
    windowLayer->addChild(messageTitleLabel);
    
    // Add Reset Button
    nextItemHeight = nextItemHeight- MESSAGE_BOX_PADDING* (removePadding ? 0.75 : 1.5) - messageTitleLabel->getContentSize().height/2 - reportButton->getContentSize().height/2;
    reportButton->setCenterPosition(Vec2(windowLayer->getContentSize().width*.7, nextItemHeight));
    windowLayer->addChild(reportButton);
    
    // Add online Safety Tips Button
    onlineSafetyTipsButton->setCenterPosition(Vec2(windowLayer->getContentSize().width*.3, reportButton->getPositionY()+reportButton->getContentSize().height/2));
    windowLayer->addChild(onlineSafetyTipsButton);

}
    
//------------- PORTRAIT SPECIFIC CREATION-------------
    
void MessageBoxChatReportChatLayer::createMessageWindowPortrait()
{
    float windowHeight = cancelButton->getContentSize().height + messageTitleLabel->getContentSize().height  + onlineSafetyTipsButton->getContentSize().height + reportButton->getContentSize().height+oomeeSprite->getContentSize().height + (5*MESSAGE_BOX_PADDING);
    
    windowLayer = createWindowLayer(currentRunningSceneSize.width * percentageOfScreenForBox, windowHeight);
    windowLayer->setPosition(currentRunningSceneSize.width/2- windowLayer->getContentSize().width/2,(currentRunningSceneSize.height - windowLayer->getContentSize().height) * 0.66);
    this->addChild(windowLayer);
}
    
void MessageBoxChatReportChatLayer::addObjectsToWindowPortrait()
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
    
    // Add Reset Button
    nextItemHeight = nextItemHeight- MESSAGE_BOX_PADDING - messageTitleLabel->getContentSize().height/2 - reportButton->getContentSize().height/2;
    reportButton->setCenterPosition(Vec2(windowLayer->getContentSize().width/2, nextItemHeight));
    windowLayer->addChild(reportButton);
    
    // Add online Safety Tips Button
    nextItemHeight = nextItemHeight- MESSAGE_BOX_PADDING- reportButton->getContentSize().height/2 - onlineSafetyTipsButton->getContentSize().height/2;
    onlineSafetyTipsButton->setCenterPosition(Vec2(windowLayer->getContentSize().width/2, nextItemHeight));
    windowLayer->addChild(onlineSafetyTipsButton);
}

//---------------------- Actions -----------------
    
void MessageBoxChatReportChatLayer::onSizeChanged()
{
    setCurrentRunningSceneSize();
    setOrientation();
    
    if(windowLayer)
        windowLayer->removeAllChildren();
    
    textMaxWidth = currentRunningSceneSize.width*percentageOfScreenForBox - MESSAGE_BOX_PADDING*2;

    createTitle();    createButtons();
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

void MessageBoxChatReportChatLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == cancelButton)
        dynamic_cast<MessageBox*>(_parentLayer)->sendDelegateMessageBoxButtonPressed(_messageBoxTitle, MessageBox::kCancel);
    else if(button == onlineSafetyTipsButton)
    {
        MessageBox::createWithLayer(OnlineSafetySlidesChild, nullptr);
    }
    else if(button == reportButton)
        dynamic_cast<MessageBox*>(_parentLayer)->sendDelegateMessageBoxButtonPressed(_messageBoxTitle, MessageBox::kReport);
}
    
}
