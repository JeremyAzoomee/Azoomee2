#include "SettingsKidsChildControlLayer.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>

using namespace Azoomee;

Layer* SettingsKidsChildControlLayer::createController(SettingsKidsLayer* setParent, int setChildNumber)
{
    auto layer = SettingsKidsChildControlLayer::create();
    layer->parentLayer = setParent;
    layer->childNumber = setChildNumber;
    layer->addChildFrame();
    layer->createTextInput();
    layer->addButtons();
    
    return layer;
}

bool SettingsKidsChildControlLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//----------------Add UI Objects-------------

void SettingsKidsChildControlLayer::addChildFrame()
{
    std::string oomeeUrl = ParentDataProvider::getInstance()->getAvatarForAnAvailableChildren(childNumber);
    int oomeeNr = ConfigStorage::getInstance()->getOomeeNumberForUrl(oomeeUrl);
    
    childFrameLayer = ChildOomeeLayer::createWithChildDetails(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(childNumber), oomeeNr);
    this->setContentSize(childFrameLayer->getContentSize());
    this->addChild(childFrameLayer);
    
}

void SettingsKidsChildControlLayer::addButtons()
{
    getCodeButton = ElectricDreamsButton::createInviteMainButton("Kid Code", this->getContentSize().width*.66);
    getCodeButton->setCenterPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height*.25));
    getCodeButton->setDelegate(this);
    childFrameLayer->addChild(getCodeButton);
    
    addFriendButton = ElectricDreamsButton::createInviteMainButton("Add A Friend", this->getContentSize().width*.66);
    addFriendButton->setCenterPosition(Vec2(this->getContentSize().width/2,getCodeButton->getPositionY() - addFriendButton->getContentSize().height));
    addFriendButton->setDelegate(this);
    childFrameLayer->addChild(addFriendButton);
    
    closeButton = ElectricDreamsButton::createWindowCloselButton();
    closeButton->setVisible(false);
    closeButton->setScale(0.75);
    closeButton->setPosition(this->getContentSize().width-closeButton->getContentSize().width,this->getContentSize().height-closeButton->getContentSize().height);
    closeButton->setDelegate(this);
    childFrameLayer->addChild(closeButton);
    
    shareButton = ElectricDreamsButton::createInviteMainButton("Share", this->getContentSize().width*.66);
    shareButton->setCenterPosition(Vec2(this->getContentSize().width/2,getCodeButton->getPositionY() - shareButton->getContentSize().height));
    shareButton->setVisible(false);
    shareButton->setDelegate(this);
    childFrameLayer->addChild(shareButton);
    
    tryAgainButton = ElectricDreamsButton::createInviteMainButton("Try Again", this->getContentSize().width*.66);
    tryAgainButton->setCenterPosition(Vec2(this->getContentSize().width/2,getCodeButton->getPositionY() - tryAgainButton->getContentSize().height));
    tryAgainButton->setVisible(false);
    tryAgainButton->setDelegate(this);
    childFrameLayer->addChild(tryAgainButton);
    
    addAnotherButton = ElectricDreamsButton::createInviteMainButton("Add Another", this->getContentSize().width*.66);
    addAnotherButton->setCenterPosition(Vec2(this->getContentSize().width/2,getCodeButton->getPositionY() - addAnotherButton->getContentSize().height));
    addAnotherButton->setVisible(false);
    addAnotherButton->setDelegate(this);
    childFrameLayer->addChild(addAnotherButton);
    
    textInputButton = ElectricDreamsButton::createTextInputAsButton("Enter their Kid Code here", this->getContentSize().width*.8);
    textInputButton->setCenterPosition(Vec2(this->getContentSize().width/2,textInputButton->getContentSize().height*2.5));
    textInputButton->setVisible(false);
    textInputButton->setDelegate(this);
    childFrameLayer->addChild(textInputButton);
    
    textInputButton = ElectricDreamsButton::createTextInputAsButton("Enter their Kid Code here", this->getContentSize().width*.8);
    textInputButton->setPosition(kidCodeTextInput->getPosition());
    textInputButton->setVisible(false);
    textInputButton->setDelegate(this);
    childFrameLayer->addChild(textInputButton);
    
    sendCodeButton = ElectricDreamsButton::createSendButton();
    sendCodeButton->setCenterPosition(Vec2(textInputButton->getCenterPosition().x+textInputButton->getContentSize().width/2 - sendCodeButton->getContentSize().width*.75,textInputButton->getCenterPosition().y));
    sendCodeButton->setVisible(false);
    sendCodeButton->setDelegate(this);
    childFrameLayer->addChild(sendCodeButton);

}

void SettingsKidsChildControlLayer::createTextInput()
{
    kidCodeTextInput = TextInputLayer::createSettingsChatTextInput(this->getContentSize().width*.8);
    kidCodeTextInput->setDelegate(this);
    kidCodeTextInput->setEditboxVisibility(false);
    kidCodeTextInput->setCenterPosition(Vec2(this->getContentSize().width/2,kidCodeTextInput->getContentSize().height*2.5));
    childFrameLayer->addChild(kidCodeTextInput);
}

void SettingsKidsChildControlLayer::clearAllButCloseButton()
{
    getCodeButton->setVisible(false);
    addFriendButton->setVisible(false);
    shareButton->setVisible(false);
    tryAgainButton->setVisible(false);
    addAnotherButton->setVisible(false);
    textInputButton->setVisible(false);
    sendCodeButton->setVisible(false);
    kidCodeTextInput->setEditboxVisibility(false);
}

//----------------------- Delegate Functions ----------------------------

void SettingsKidsChildControlLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button ==getCodeButton)
    {
        childFrameLayer->setToShowingCode();
        
        this->setLocalZOrder(220);
        parentLayer->scrollToPosition(childNumber);
        closeButton->setVisible(true);
        
        clearAllButCloseButton();
        shareButton->setVisible(true);
    }
    else if(button == addFriendButton || button == addAnotherButton)
    {
        kidCodeTextInput->setText("");
        childFrameLayer->setToAddAFriend();
        
        this->setLocalZOrder(220);
        parentLayer->scrollToPosition(childNumber);
        closeButton->setVisible(true);
        
        clearAllButCloseButton();
        textInputButton->setVisible(true);
        
    }
    else if(button == closeButton)
    {
        sendCodeButton->setEnabled(false);
        kidCodeTextInput->setText("");
        childFrameLayer->resetToIdle();
        
        clearAllButCloseButton();
        getCodeButton->setVisible(true);
        addFriendButton->setVisible(true);
        
        this->setLocalZOrder(200);
        closeButton->setVisible(false);
        parentLayer->scrollReset();
    }
    else if(button == textInputButton || button == tryAgainButton)
    {
        childFrameLayer->setToAddAFriendTextBox();
        
        clearAllButCloseButton();
        kidCodeTextInput->setEditboxVisibility(true);
        sendCodeButton->setVisible(true);
    }
    else if(button == sendCodeButton)
    {
        clearAllButCloseButton();
        
        if(kidCodeTextInput->getText() == "YES")
        {
            childFrameLayer->setToCodeSuccess("YES");
            addAnotherButton->setVisible(true);
        }
        else
        {
            childFrameLayer->setToCodeError("NO");
            tryAgainButton->setVisible(true);
        }
    }
}

void SettingsKidsChildControlLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    if(inputLayer->getText() == "YES" || inputLayer->getText() == "NO")
        sendCodeButton->setEnabled(true);
    else
        sendCodeButton->setEnabled(false);
}

void SettingsKidsChildControlLayer::textInputReturnPressed(TextInputLayer* inputLayer)
{
    
}

