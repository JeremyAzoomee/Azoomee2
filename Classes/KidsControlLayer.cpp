#include "KidsControlLayer.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/NativeShare/NativeShare.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>

NS_AZOOMEE_BEGIN

Layer* KidsControlLayer::createController(SettingsKidsLayer* setParent, int setChildNumber)
{
    auto layer = KidsControlLayer::create();
    layer->parentLayer = setParent;
    layer->childNumber = setChildNumber;
    layer->addChildFrame();
    layer->createTextInput();
    layer->addButtons();
    
    return layer;
}

bool KidsControlLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void KidsControlLayer::addChildFrame()
{
    childFrameLayer = KidsLayer::createWithChildDetails(childNumber);
    this->setContentSize(childFrameLayer->getContentSize());
    this->addChild(childFrameLayer);
}

//----------------Add CONTROL Objects-------------

void KidsControlLayer::createTextInput()
{
    kidCodeTextInput = TextInputLayer::createSettingsChatTextInput(this->getContentSize().width*.8);
    kidCodeTextInput->setDelegate(this);
    kidCodeTextInput->setEditboxVisibility(false);
    kidCodeTextInput->setCenterPosition(Vec2(this->getContentSize().width/2,kidCodeTextInput->getContentSize().height*2.5));
    childFrameLayer->addChild(kidCodeTextInput);
}

void KidsControlLayer::addButtons()
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
    closeButton->setScale(0.68);
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

//------------------CONTROL FUNCTIONS----------------------

void KidsControlLayer::moveToShowCode()
{
    childFrameLayer->setToShowingCode();
    
    this->setLocalZOrder(SELECTED_KID_LAYER_Z_ORDER);
    parentLayer->selectChild(childNumber);
    closeButton->setVisible(true);
    
    clearAllButCloseButton();
    shareButton->setVisible(true);
}

void KidsControlLayer::moveToStartAddFriend()
{
    kidCodeTextInput->setText("");
    childFrameLayer->setToAddAFriend();
    
    this->setLocalZOrder(SELECTED_KID_LAYER_Z_ORDER);
    parentLayer->selectChild(childNumber);
    closeButton->setVisible(true);
    
    clearAllButCloseButton();
    textInputButton->setVisible(true);
}

void KidsControlLayer::moveToAddFriendTextBox()
{
    kidCodeTextInput->setText("");
    childFrameLayer->setToAddAFriendTextBox();
    
    clearAllButCloseButton();
    kidCodeTextInput->setEditboxVisibility(true);
    sendCodeButton->setEnabled(false);
    sendCodeButton->setVisible(true);
    kidCodeTextInput->focusAndShowKeyboard();
}

void KidsControlLayer::closeKidController()
{
    sendCodeButton->setEnabled(false);
    kidCodeTextInput->setText("");
    childFrameLayer->removeObjects(true);
    
    clearAllButCloseButton();
    getCodeButton->setVisible(true);
    addFriendButton->setVisible(true);
    
    this->setLocalZOrder(IDLE_KID_LAYER_Z_ORDER);
    closeButton->setVisible(false);
    parentLayer->scrollReset();
}

void KidsControlLayer::sendInviteCode()
{
    ModalMessages::getInstance()->startLoading();
    
    HttpRequestCreator *request = API::friendRequest(ParentDataProvider::getInstance()->getIDForAvailableChildren(childNumber),ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(childNumber),stringToUpper(kidCodeTextInput->getText()), this);
    request->execute();
    
    
}

void KidsControlLayer::inviteCodeResponse(bool codeIsOK)
{
    ModalMessages::getInstance()->stopLoading();
    clearAllButCloseButton();
    
    if(codeIsOK)
    {
        childFrameLayer->setToCodeSuccess(kidCodeTextInput->getText());
        addAnotherButton->setVisible(true);
    }
    else
    {
        childFrameLayer->setToCodeError(kidCodeTextInput->getText());
        tryAgainButton->setVisible(true);
    }
}

void KidsControlLayer::shareKidCode()
{
    nativeShareScreenString("Bob has shared this code to you to connect via Azoomee chat, ABCD09SE");
}

void KidsControlLayer::clearAllButCloseButton()
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

void KidsControlLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button ==getCodeButton)
        moveToShowCode();
    else if(button == addFriendButton)
        moveToStartAddFriend();
    else if(button == closeButton)
        closeKidController();
    else if(button == textInputButton || button == tryAgainButton || button == addAnotherButton)
        moveToAddFriendTextBox();
    else if(button == sendCodeButton)
        sendInviteCode();
    else if(button ==shareButton)
        shareKidCode();
}

void KidsControlLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    sendCodeButton->setEnabled( isValid);
}

void KidsControlLayer::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(kidCodeTextInput->inputIsValid())
       sendInviteCode();
}

void KidsControlLayer::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    auto parent = this->getParent();
    
    originalYposition = parent->getParent()->getPositionY();
    
    parent->getParent()->setPosition(parent->getParent()->getPositionX(),Director::getInstance()->getVisibleOrigin().y + Director::getInstance()->getVisibleSize().height*0.75);
}

void KidsControlLayer::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    auto parent = this->getParent();
    
    parent->getParent()->setPosition(parent->getParent()->getPositionX(),originalYposition);
}

void KidsControlLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    ModalMessages::getInstance()->startLoading();
    inviteCodeResponse(true);
}

void KidsControlLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
    inviteCodeResponse(false);
}

NS_AZOOMEE_END

