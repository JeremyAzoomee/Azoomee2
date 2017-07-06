#include "ConfirmationControlLayer.h"
#include "SettingsConfirmationLayer.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/MessageBox.h>

#define MARGIN 69

NS_AZOOMEE_BEGIN

Layer* ConfirmationControlLayer::createController(Size layerSize, int setConfirmationNumber)
{
    auto layer = ConfirmationControlLayer::create();
    layer->setContentSize(layerSize);
    layer->confirmationNumber = setConfirmationNumber;
    layer->addConfirmationFrame();
    layer->addButtons();
    
    return layer;
}


bool ConfirmationControlLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

//----------------Add UI Objects-------------

void ConfirmationControlLayer::addConfirmationFrame()
{
    confirmationFrameLayer = ConfirmationLayer::create();
    confirmationFrameLayer->setContentSize(this->getContentSize());

    confirmationFrameLayer->addDetailsToLayer(ParentDataProvider::getInstance()->getPendingFriendRequestFriendName(confirmationNumber), ParentDataProvider::getInstance()->getPendingFriendRequestSenderName(confirmationNumber), ParentDataProvider::getInstance()->getPendingFriendRequestInviteCode(confirmationNumber));
    
    this->addChild(confirmationFrameLayer);
}

void ConfirmationControlLayer::addButtons()
{
    confirmButton = ElectricDreamsButton::createInviteMainButton("Confirm", 400);
    confirmButton->setCenterPosition(Vec2(this->getContentSize().width-confirmButton->getContentSize().width/2-MARGIN,this->getContentSize().height/2));
    confirmButton->setDelegate(this);
    confirmButton->setMixPanelButtonName("Settings-Confirmation-Confirm");
    confirmationFrameLayer->addChild(confirmButton);
    
    yesButton = ElectricDreamsButton::createRedFilledButton("Yes", 400);
    yesButton->setCenterPosition(confirmButton->getCenterPosition());
    yesButton->setDelegate(this);
    yesButton->setVisible(false);
    yesButton->setMixPanelButtonName("Settings-Confirmation-Reject-Yes");
    confirmationFrameLayer->addChild(yesButton);
    
    noButton = ElectricDreamsButton::createRedOutlineButton("No", 400);
    noButton->setCenterPosition(Vec2(confirmButton->getPositionX() - noButton->getContentSize().width/2 - MARGIN,this->getContentSize().height/2));
    noButton->setVisible(false);
    noButton->setDelegate(this);
    noButton->setMixPanelButtonName("Settings-Confirmation-Reject-No");
    confirmationFrameLayer->addChild(noButton);
    
    rejectButton = ElectricDreamsButton::createTextAsButtonAqua("Reject", 48,true);
    rejectButton->setCenterPosition(noButton->getCenterPosition());
    rejectButton->setDelegate(this);
    rejectButton->setMixPanelButtonName("Settings-Confirmation-Reject");
    confirmationFrameLayer->addChild(rejectButton);

}

//--------------BUTTON ACTIONS------------------

void ConfirmationControlLayer::clearAllButtons()
{
    confirmButton->setVisible(false);
    yesButton->setVisible(false);
    noButton->setVisible(false);
    rejectButton->setVisible(false);
}

void ConfirmationControlLayer::setToReject()
{
    clearAllButtons();
    this->setLocalZOrder(CONFIRMATION_REJECT_Z);
    confirmationFrameLayer->setToReject();
    yesButton->setVisible(true);
    noButton->setVisible(true);
}

void ConfirmationControlLayer::setToIdle()
{
    clearAllButtons();
    this->setLocalZOrder(CONFIRMATION_IDLE_Z);
    confirmationFrameLayer->setIdle();
    rejectButton->setVisible(true);
    confirmButton->setVisible(true);
}

//----------------------BACKENDCALLER RETURN FUNCTIONS-------------------

void ConfirmationControlLayer::setToConfirmed()
{
     clearAllButtons();
     this->setLocalZOrder(CONFIRMATION_CONFIRMED_Z);
     confirmationFrameLayer->setToConfirm();
}

void ConfirmationControlLayer::setToRejected()
{
    
    clearAllButtons();
    confirmationFrameLayer->setToRejected();
}

//-------------------- CALL BACKEND------------------

void ConfirmationControlLayer::friendRequestReaction(bool confirmed)
{
    HttpRequestCreator *request = API::friendRequestReaction(confirmed, ParentDataProvider::getInstance()->getPendingFriendRequestRespondentID(confirmationNumber), ParentDataProvider::getInstance()->getPendingFriendRequestRequestID(confirmationNumber), ParentDataProvider::getInstance()->getPendingFriendRequestSenderName(confirmationNumber), this);
    request->execute();
}



//----------------------- Delegate Functions ----------------------------

void ConfirmationControlLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button==confirmButton)
        friendRequestReaction(true);
    else if(button==rejectButton)
        setToReject();
    else if(button==noButton)
        setToIdle();
    else if(button==yesButton)
        friendRequestReaction(false);
}

void ConfirmationControlLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(body.find("approval\":\"REJECTED") != std::string::npos)
    {
        AnalyticsSingleton::getInstance()->settingsConfirmationRejectedSuccess();
        setToRejected();
    } else if(body.find("approval\":\"APPROVED")!= std::string::npos)
    {
        AnalyticsSingleton::getInstance()->settingsConfirmationApprovedSuccess();
        setToConfirmed();
    }
}

void ConfirmationControlLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    AnalyticsSingleton::getInstance()->settingsConfirmationError(errorCode);
    MessageBox::createWith(ERROR_CODE_SOMETHING_WENT_WRONG, nullptr);
}

NS_AZOOMEE_END
