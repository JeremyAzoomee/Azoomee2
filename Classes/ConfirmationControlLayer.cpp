#include "ConfirmationControlLayer.h"

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
    
}

void ConfirmationControlLayer::addButtons()
{
    /*getCodeButton = ElectricDreamsButton::createInviteMainButton("Kid Code", this->getContentSize().width*.66);
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
    childFrameLayer->addChild(sendCodeButton);*/

}

void ConfirmationControlLayer::clearAllButtons()
{

}

//----------------------- Delegate Functions ----------------------------

void ConfirmationControlLayer::buttonPressed(ElectricDreamsButton* button)
{
    
}
