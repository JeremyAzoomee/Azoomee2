#include "AwaitingAdultPinLayer.h"
#include <AzoomeeCommon/Strings.h>
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <AzoomeeCommon/Utils/IosNativeFunctionsSingleton.h>
#endif

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
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(UserDefault::getInstance()->getIntegerForKey(IosNativeFunctionsSingleton::kBiometricValidation) == 1)
    {
        IosNativeFunctionsSingleton::getInstance()->doBiometricValidation(false);
    }
#endif
}

void AwaitingAdultPinLayer::addListenerToBiometricValidationSuccess()
{
    _biometricValidationSuccessListener = cocos2d::EventListenerCustom::create(IosNativeFunctionsSingleton::kBiometricValidationSuccess, [=](EventCustom* event) {
        auto funcCallAction = CallFunc::create([=](){
            this->scheduleOnce(schedule_selector(AwaitingAdultPinLayer::removeSelf), 0.1);
            this->getDelegate()->AdultPinAccepted(this);
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
    });
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_biometricValidationSuccessListener, 1);
}

void AwaitingAdultPinLayer::addListenerToBiometricValidationFailure()
{
    _biometricValidationFailureListener = cocos2d::EventListenerCustom::create(IosNativeFunctionsSingleton::kBiometricValidationFailure, [=](EventCustom* event) {
        auto funcCallAction = CallFunc::create([=](){
            const std::vector<std::string>& buttonTitleList = {"Retry", "Cancel"};
            MessageBox::createWith("Authentication Failed", "Ooops! Biometric authentication failed.", buttonTitleList, this);
            editBox_pin->hideKeyboard();
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
    
    if(UserDefault::getInstance()->getIntegerForKey(IosNativeFunctionsSingleton::kBiometricValidation) != 1)
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
    if(messageBoxTitle == "Biometric Authentication")
    {
        if(buttonTitle == "Yes")
        {
            IosNativeFunctionsSingleton::getInstance()->doBiometricValidation(false);
            return;
        }
        
        if(buttonTitle == "No")
        {
            UserDefault::getInstance()->setIntegerForKey(IosNativeFunctionsSingleton::kBiometricValidation, -1);
            return;
        }
    }
    
    if(messageBoxTitle == "Authentication Failed")
    {
        if(buttonTitle == "Retry")
        {
            IosNativeFunctionsSingleton::getInstance()->doBiometricValidation(false);
            return;
        }
        
        if(buttonTitle == "Cancel")
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
        //Schedule so it calls delegate before removing self. Avoiding crash
        this->scheduleOnce(schedule_selector(AwaitingAdultPinLayer::removeSelf), 0.1);
        this->getDelegate()->AdultPinAccepted(this);
        
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if(UserDefault::getInstance()->getIntegerForKey(IosNativeFunctionsSingleton::kBiometricValidation) == 0)
        {
            if(IosNativeFunctionsSingleton::getInstance()->doBiometricValidation(true))
            {
                const std::vector<std::string>& buttonTitleList = {"Yes", "No"};
                MessageBox::createWith("Biometric Authentication", "Do you want to use biometric authentication to enter your PIN in the future?", buttonTitleList, this);
            }
        }
#endif
    }
    else
    {
        editBox_pin->setText("");
        MessageBox::createWith(ERROR_CODE_INCORRECT_PIN, editBox_pin, this);
        acceptButton->setVisible(false);
    }
    
}

NS_AZOOMEE_END
