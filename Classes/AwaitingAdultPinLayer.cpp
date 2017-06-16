#include "AwaitingAdultPinLayer.h"
#include <AzoomeeCommon/Strings.h>
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>

using namespace Azoomee;


bool AwaitingAdultPinLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    AudioMixer::getInstance()->pauseBackgroundMusic();
    
    createAndFadeInLayer();
    addUIObjects();
    
    return true;
}

//---------------------- Create Layer -----------------------------

void AwaitingAdultPinLayer::createAndFadeInLayer()
{
    backgroundLayer = LayerColor::create(Color4B(15,14,7,255),origin.x+ visibleSize.width, origin.y + visibleSize.height);
    
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    addListenerToBackgroundLayer();
}

void AwaitingAdultPinLayer::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) 
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

void AwaitingAdultPinLayer::addUIObjects()
{
    addSideWiresToScreen(this, 0, 2);
    
    windowLayer = createWindowLayer(750);
    windowLayer->setPosition(visibleSize.width/2- windowLayer->getContentSize().width/2,origin.y + visibleSize.height*.72 - windowLayer->getContentSize().height/2);
    this->addChild(windowLayer);
    
    //-------ACCEPT PLACEHOLDER BUTTON-------
    
    auto placeHolderAcceptButton= ElectricDreamsButton::createPlaceHolderButton();
    windowLayer->addChild(placeHolderAcceptButton);

    
    //------- CREATE ACCEPT BUTTON -----------
    
    acceptButton = ElectricDreamsButton::createAcceptButton();
    acceptButton->setDelegate(this);
    acceptButton->setVisible(false);
    acceptButton->setMixPanelButtonName("PinAcceptButton");
    windowLayer->addChild(acceptButton);
    
    //-------- PIN EDITBOX -------------
    
    editBox_pin = TextInputLayer::createWithSize(Size(windowLayer->getContentSize().width/2,acceptButton->getContentSize().height), INPUT_IS_PIN);
    editBox_pin->setPosition(Vec2(windowLayer->getContentSize().width/2 - editBox_pin->getContentSize().width/2 - acceptButton->getContentSize().width*.66, windowLayer->getContentSize().height*.25));
    editBox_pin->setDelegate(this);
    windowLayer->addChild(editBox_pin);
    
    editBox_pin->focusAndShowKeyboard();
    
    //--------- LOCATION FOR ACCEPT BUTTON---------
    
    placeHolderAcceptButton->setPosition(Vec2(editBox_pin->getPositionX() + editBox_pin->getContentSize().width + acceptButton->getContentSize().width/2,editBox_pin->getPositionY()));
    acceptButton->setPosition(placeHolderAcceptButton->getPosition());
    
    //---------- MODAL LABEL ------------
    
    auto enterYourPinTitle = createLabelHeaderWhite(StringMgr::getInstance()->getStringForKey(PIN_REQUEST_LABEL));
    enterYourPinTitle->setPosition(editBox_pin->getPositionX() + enterYourPinTitle->getContentSize().width/2, windowLayer->getContentSize().height*.66+enterYourPinTitle->getContentSize().height/2);
    windowLayer->addChild(enterYourPinTitle);
    
    //-------- CLOSE BUTTON ----------
    
    cancelButton = ElectricDreamsButton::createWindowCloselButton();
    cancelButton->setCenterPosition(Vec2(windowLayer->getContentSize().width-cancelButton->getContentSize().width*0.75, windowLayer->getContentSize().height-cancelButton->getContentSize().height*.75));
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("PinCancelButton");
    windowLayer->addChild(cancelButton);
}

//---------------------- Actions -----------------

void AwaitingAdultPinLayer::removeSelf(float dt)
{
    if(this)
    {
        this->removeChild(backgroundLayer);
        this->removeFromParent();
    }
}

//----------------------- Delegate Functions ----------------------------

void AwaitingAdultPinLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    acceptButton->setVisible(isValid);
}

void AwaitingAdultPinLayer::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(editBox_pin->inputIsValid())
        BackEndCaller::getInstance()->updateParentPin(this);
}

void AwaitingAdultPinLayer::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    
}

void AwaitingAdultPinLayer::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    
}

void AwaitingAdultPinLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == cancelButton)
    {
        AudioMixer::getInstance()->resumeBackgroundMusic();
        //Schedule so it calls delegate before removing self. Avoiding crash
        this->scheduleOnce(schedule_selector(AwaitingAdultPinLayer::removeSelf), 0.1);
        this->getDelegate()->AdultPinCancelled(this);
    }
    else if(button == acceptButton)
        BackEndCaller::getInstance()->updateParentPin(this);
}

void AwaitingAdultPinLayer::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    editBox_pin->focusAndShowKeyboard();
}

void AwaitingAdultPinLayer::secondCheckForPin()
{
    //Please implement your second check here. If first check is not okay, please call: BackEndCaller::getInstance->updateParentPin(this);
    CCLOG("Second check for pin callback was called");
    
    if(editBox_pin->getText() == ParentDataProvider::getInstance()->getParentPin() || ("" == ParentDataProvider::getInstance()->getParentPin() && editBox_pin->getText() == "1234"))
    {
        //Schedule so it calls delegate before removing self. Avoiding crash
        this->scheduleOnce(schedule_selector(AwaitingAdultPinLayer::removeSelf), 0.1);
        this->getDelegate()->AdultPinAccepted(this);
    }
    else
    {
        editBox_pin->setText("");
        MessageBox::createWith(ERROR_CODE_INCORRECT_PIN, editBox_pin, this);
        acceptButton->setVisible(false);
    }
    
}
