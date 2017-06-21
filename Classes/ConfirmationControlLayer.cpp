#include "ConfirmationControlLayer.h"
#include "SettingsConfirmationLayer.h"

#define MARGIN 69

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
    
    //REMOVE WHEN WE HAVE REAL DATA
    if(confirmationNumber == 0)
        confirmationFrameLayer->addDetailsToLayer("Emmaline", "Mary", "66OG09K8");
    else if(confirmationNumber == 1)
        confirmationFrameLayer->addDetailsToLayer("Montgomery", "Leroy", "49KW03B3");
    else
        confirmationFrameLayer->addDetailsToLayer("Emmaline", "Alexandra", "59DF01L7");
    
    this->addChild(confirmationFrameLayer);
}

void ConfirmationControlLayer::addButtons()
{
    confirmButton = ElectricDreamsButton::createInviteMainButton("Confirm", 400);
    confirmButton->setCenterPosition(Vec2(this->getContentSize().width-confirmButton->getContentSize().width/2-MARGIN,this->getContentSize().height/2));
    confirmButton->setDelegate(this);
    confirmationFrameLayer->addChild(confirmButton);
    
    yesButton = ElectricDreamsButton::createRedFilledButton("Yes", 400);
    yesButton->setCenterPosition(confirmButton->getCenterPosition());
    yesButton->setDelegate(this);
    yesButton->setVisible(false);
    confirmationFrameLayer->addChild(yesButton);
    
    noButton = ElectricDreamsButton::createRedOutlineButton("No", 400);
    noButton->setCenterPosition(Vec2(confirmButton->getPositionX() - noButton->getContentSize().width/2 - MARGIN,this->getContentSize().height/2));
    noButton->setVisible(false);
    noButton->setDelegate(this);
    confirmationFrameLayer->addChild(noButton);
    
    rejectButton = ElectricDreamsButton::createTextAsButtonAqua("Reject", 48,true);
    rejectButton->setCenterPosition(noButton->getCenterPosition());
    rejectButton->setDelegate(this);
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

void ConfirmationControlLayer::setToConfirmed()
{
    clearAllButtons();
    this->setLocalZOrder(CONFIRMATION_CONFIRMED_Z);
    confirmationFrameLayer->setToConfirm();
}

void ConfirmationControlLayer::setToReject()
{
    clearAllButtons();
    this->setLocalZOrder(CONFIRMATION_REJECT_Z);
    confirmationFrameLayer->setToReject();
    yesButton->setVisible(true);
    noButton->setVisible(true);
}

void ConfirmationControlLayer::setToRejected()
{
    clearAllButtons();
    confirmationFrameLayer->setToRejected();
}

void ConfirmationControlLayer::setToIdle()
{
    clearAllButtons();
    this->setLocalZOrder(CONFIRMATION_IDLE_Z);
    confirmationFrameLayer->setIdle();
    rejectButton->setVisible(true);
    confirmButton->setVisible(true);
}

//----------------------- Delegate Functions ----------------------------

void ConfirmationControlLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button==confirmButton)
        setToConfirmed();
    else if(button==rejectButton)
        setToReject();
    else if(button==noButton)
        setToIdle();
    else if(button==yesButton)
        setToRejected();
}
