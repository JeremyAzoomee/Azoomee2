#include "KidsControlLayer.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/NativeShare/NativeShare.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

NS_AZOOMEE_BEGIN

Layer* KidsControlLayer::createController(SettingsKidsLayer* setParent, int setChildNumber)
{
    auto layer = KidsControlLayer::create();
    layer->parentLayer = setParent;
    layer->childNumber = setChildNumber;
    layer->addChildFrame();
    layer->addButtonsAndInput();
    
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

void KidsControlLayer::addButtonsAndInput()
{
    closeButton = ElectricDreamsButton::createWindowCloselButton();
    closeButton->setVisible(false);
    closeButton->setScale(0.68);
    closeButton->setPosition(this->getContentSize().width-closeButton->getContentSize().width,this->getContentSize().height-closeButton->getContentSize().height);
    closeButton->setDelegate(this);
    closeButton->setMixPanelButtonName("Settings-YourKids-Close");
    childFrameLayer->addChild(closeButton);
    
    shareButton = ElectricDreamsButton::createKidCodeShareButton(ParentDataProvider::getInstance()->getInviteCodeForAvailableChildren(childNumber), this->getContentSize().width*.66);
    shareButton->setCenterPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height - shareButton->getContentSize().height*2.5));
    shareButton->setDelegate(this);
    shareButton->setMixPanelButtonName("Settings-YourKids-ShareCode");
    childFrameLayer->addChild(shareButton);
    
    tryAgainButton = ElectricDreamsButton::createInviteMainButton("Try Again", this->getContentSize().width*.66);
    tryAgainButton->setCenterPosition(Vec2(this->getContentSize().width/2,this->getContentSize().height*.25 - tryAgainButton->getContentSize().height));
    tryAgainButton->setVisible(false);
    tryAgainButton->setDelegate(this);
    tryAgainButton->setMixPanelButtonName("Settings-YourKids-TryCodeAgain");
    childFrameLayer->addChild(tryAgainButton);
    
    addAnotherButton = ElectricDreamsButton::createInviteMainButton("Add Another", this->getContentSize().width*.66);
    addAnotherButton->setCenterPosition(Vec2(this->getContentSize().width/2,tryAgainButton->getPositionY()));
    addAnotherButton->setVisible(false);
    addAnotherButton->setDelegate(this);
    addAnotherButton->setMixPanelButtonName("Settings-YourKids-AddAnotherCode");
    childFrameLayer->addChild(addAnotherButton);

    textInputButton = ElectricDreamsButton::createTextInputAsButton("Enter their Kid Code here", this->getContentSize().width*.8);
    textInputButton->setCenterPosition(Vec2(this->getContentSize().width/2,textInputButton->getContentSize().height*1.3));
    textInputButton->setDelegate(this);
    textInputButton->setMixPanelButtonName("Settings-YourKids-EnterKidCodeHere");
    childFrameLayer->addChild(textInputButton);
    
    kidCodeTextInput = TextInputLayer::createSettingsChatTextInput(this->getContentSize().width*.8);
    kidCodeTextInput->setDelegate(this);
    kidCodeTextInput->setEditboxVisibility(false);
    kidCodeTextInput->setCenterPosition(Vec2(this->getContentSize().width/2,textInputButton->getPositionY()+textInputButton->getContentSize().height/2));
    childFrameLayer->addChild(kidCodeTextInput);
    
    sendCodeButton = ElectricDreamsButton::createSendButton();
    sendCodeButton->setCenterPosition(Vec2(kidCodeTextInput->getPositionX()+kidCodeTextInput->getContentSize().width - sendCodeButton->getContentSize().width*.75,kidCodeTextInput->getPositionY()+kidCodeTextInput->getContentSize().height/2));
    sendCodeButton->setVisible(false);
    sendCodeButton->setDelegate(this);
    sendCodeButton->setMixPanelButtonName("Settings-YourKids-SendKidCodeInviteRequest");
    childFrameLayer->addChild(sendCodeButton);
}

//------------------CONTROL FUNCTIONS----------------------

void KidsControlLayer::moveToAddFriendTextBox()
{
    kidCodeTextInput->setText("");
    childFrameLayer->setToAddAFriendTextBox();
    
    this->setLocalZOrder(SELECTED_KID_LAYER_Z_ORDER);
    parentLayer->selectChildForTextInput(childNumber);
    closeButton->setVisible(true);
    
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
    childFrameLayer->setToStartLayout();
    
    clearAllButCloseButton();
    
    this->setLocalZOrder(IDLE_KID_LAYER_Z_ORDER);
    closeButton->setVisible(false);
    parentLayer->scrollReset();
    shareButton->setVisible(true);
    textInputButton->setVisible(true);
}

void KidsControlLayer::sendInviteCode()
{
    ModalMessages::getInstance()->startLoading();
    
    HttpRequestCreator *request = API::FriendRequest(ParentDataProvider::getInstance()->getIDForAvailableChildren(childNumber),ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(childNumber),stringToUpper(kidCodeTextInput->getText()), this);
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
    parentLayer->selectChildForSharing(childNumber);
    
    std::string childsName = ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(childNumber);
    
    nativeShareScreenString(StringUtils::format("%s  is using Azoomee to safely chat with friends! Enter %s's Kid Code %s in Azoomee.\nDownload Azoomee here: http://bit.ly/azoomeekids",childsName.c_str(),childsName.c_str(),ParentDataProvider::getInstance()->getInviteCodeForAvailableChildren(childNumber).c_str()));
}

void KidsControlLayer::clearAllButCloseButton()
{
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
    if(button == closeButton)
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
    
    parent->getParent()->setPosition(parent->getParent()->getPositionX(),Director::getInstance()->getVisibleOrigin().y + Director::getInstance()->getVisibleSize().height*0.85);
}

void KidsControlLayer::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    auto parent = this->getParent();
    
    parent->getParent()->setPosition(parent->getParent()->getPositionX(),originalYposition);
}

void KidsControlLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    AnalyticsSingleton::getInstance()->settingsKidInviteRequestSuccess();
    ModalMessages::getInstance()->startLoading();
    inviteCodeResponse(true);
}

void KidsControlLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    AnalyticsSingleton::getInstance()->settingsKidInviteRequestError(errorCode);
    ModalMessages::getInstance()->stopLoading();
    inviteCodeResponse(false);
}

NS_AZOOMEE_END

