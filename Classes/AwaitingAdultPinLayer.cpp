#include "AwaitingAdultPinLayer.h"
#include <AzoomeeCommon/Strings.h>
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include <AzoomeeCommon/Utils/BiometricAuthenticationHandler.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
    backgroundLayer = LayerColor::create(Style::Color_4B::awaitingPinLayer,origin.x+ visibleSize.width, origin.y + visibleSize.height);
    
    this->addChild(backgroundLayer);
    Director::getInstance()->getRunningScene()->addChild(this);
    
    addListenerToBackgroundLayer();
    addListenerToBiometricValidationSuccess();
    addListenerToBiometricValidationFailure();
    
    if(BiometricAuthenticationHandler::getInstance()->biometricAuthenticationSet())
    {
        BiometricAuthenticationHandler::getInstance()->startBiometricAuthentication();
    }
}

void AwaitingAdultPinLayer::addListenerToBiometricValidationSuccess()
{
    _biometricValidationSuccessListener = cocos2d::EventListenerCustom::create(BiometricAuthenticationHandler::kBiometricValidationSuccess, [=](EventCustom* event) {
        auto funcCallAction = CallFunc::create([=](){
            this->scheduleOnce(schedule_selector(AwaitingAdultPinLayer::removeSelf), 0.1);
            
            if(this->getDelegate())
            {
                this->getDelegate()->AdultPinAccepted(this);
            }
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
    });
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_biometricValidationSuccessListener, 1);
}

void AwaitingAdultPinLayer::addListenerToBiometricValidationFailure()
{
    _biometricValidationFailureListener = cocos2d::EventListenerCustom::create(BiometricAuthenticationHandler::kBiometricValidationFailure, [=](EventCustom* event) {
        auto funcCallAction = CallFunc::create([=](){
            MessageBox::createWith(BiometricAuthenticationHandler::kAuthFailedDialogTitle, BiometricAuthenticationHandler::kAuthFailedDialogBody, BiometricAuthenticationHandler::kAuthFailedDialogButtons, this);
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
    });
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_biometricValidationFailureListener, 1);
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
    
    windowLayer = createWhiteWindowLayer(750);
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
    
    editBox_pin = TextInputLayer::createWithSize(Size(windowLayer->getContentSize().width/2,160), INPUT_IS_PIN);
    editBox_pin->setPosition(Vec2(windowLayer->getContentSize().width/2 - editBox_pin->getContentSize().width/2 - acceptButton->getContentSize().width*.66, windowLayer->getContentSize().height*.25));
    editBox_pin->setDelegate(this);
    windowLayer->addChild(editBox_pin);
    
    if(!BiometricAuthenticationHandler::getInstance()->biometricAuthenticationSet())
    {
        editBox_pin->focusAndShowKeyboard();
    }
    
    //--------- LOCATION FOR ACCEPT BUTTON---------
    
    placeHolderAcceptButton->setPosition(Vec2(editBox_pin->getPositionX() + editBox_pin->getContentSize().width + acceptButton->getContentSize().width/2,editBox_pin->getPositionY()+editBox_pin->getContentSize().height/2 - placeHolderAcceptButton->getContentSize().height/2));
    acceptButton->setPosition(placeHolderAcceptButton->getPosition());
    
    //---------- MODAL LABEL ------------
    
    auto enterYourPinTitle = createLabelButtonAdultPrimary(StringMgr::getInstance()->getStringForKey(PIN_REQUEST_LABEL));
    enterYourPinTitle->setPosition(editBox_pin->getPositionX() + enterYourPinTitle->getContentSize().width/2, windowLayer->getContentSize().height*.66+enterYourPinTitle->getContentSize().height/2);
    windowLayer->addChild(enterYourPinTitle);
    
    //-------- CLOSE BUTTON ----------
    
    cancelButton = ElectricDreamsButton::createWindowCloseButtonGreen();
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
        Director::getInstance()->getEventDispatcher()->removeEventListener(_biometricValidationSuccessListener);
        Director::getInstance()->getEventDispatcher()->removeEventListener(_biometricValidationFailureListener);
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
    if(messageBoxTitle == BiometricAuthenticationHandler::kStartBiometricDialogTitle)
    {
        if(buttonTitle == BiometricAuthenticationHandler::kStartBiometricDialogButtons.at(0))
        {
            BiometricAuthenticationHandler::getInstance()->startBiometricAuthentication();
        }
        
        if(buttonTitle == BiometricAuthenticationHandler::kStartBiometricDialogButtons.at(1))
        {
            BiometricAuthenticationHandler::getInstance()->biometricAuthenticationNotNeeded();
        }
        
        this->scheduleOnce(schedule_selector(AwaitingAdultPinLayer::removeSelf), 0.1);
        return;
    }
    
    if(messageBoxTitle == BiometricAuthenticationHandler::kAuthFailedDialogTitle)
    {
        if(buttonTitle == BiometricAuthenticationHandler::kAuthFailedDialogButtons.at(0))
        {
            BiometricAuthenticationHandler::getInstance()->startBiometricAuthentication();
            return;
        }
        
        if(buttonTitle == BiometricAuthenticationHandler::kAuthFailedDialogButtons.at(1))
        {
            editBox_pin->focusAndShowKeyboard();
            return;
        }
    }
    
    editBox_pin->focusAndShowKeyboard();
}

void AwaitingAdultPinLayer::secondCheckForPin()
{
    //Please implement your second check here. If first check is not okay, please call: BackEndCaller::getInstance->updateParentPin(this);
    
    if(editBox_pin->getText() == ParentDataProvider::getInstance()->getParentPin() || ("" == ParentDataProvider::getInstance()->getParentPin() && editBox_pin->getText() == "1234"))
    {
        if(this->getDelegate())
        {
            this->getDelegate()->AdultPinAccepted(this);
        }
        
        if((UserDefault::getInstance()->getIntegerForKey(BiometricAuthenticationHandler::kBiometricValidation) == 0) && (!_pinIsForPayment) && (BiometricAuthenticationHandler::getInstance()->biometricAuthenticationAvailable()))
        {
            MessageBox::createWith(BiometricAuthenticationHandler::kStartBiometricDialogTitle, BiometricAuthenticationHandler::kStartBiometricDialogBody, BiometricAuthenticationHandler::kStartBiometricDialogButtons, this);
        }
        else
        {
            //Schedule so it calls delegate before removing self. Avoiding crash
            this->scheduleOnce(schedule_selector(AwaitingAdultPinLayer::removeSelf), 0.1);
        }
    }
    else
    {
        editBox_pin->setText("");
        MessageBox::createWith(ERROR_CODE_INCORRECT_PIN, editBox_pin, this);
        acceptButton->setVisible(false);
    }
    
}

void AwaitingAdultPinLayer::setPinIsForPayment(bool pinIsForPayment)
{
    _pinIsForPayment = pinIsForPayment;
}

NS_AZOOMEE_END
