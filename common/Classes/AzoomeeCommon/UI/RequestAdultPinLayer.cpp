#include "RequestAdultPinLayer.h"
#include "../Strings.h"
#include "../Data/Parent/ParentDataProvider.h"
#include "../Audio/AudioMixer.h"
#include "../UI/ElectricDreamsTextStyles.h"
#include "../UI/ElectricDreamsDecoration.h"
#include "../API/API.h"
#include "../Data/Parent/ParentDataParser.h"
#include "ModalMessages.h"
#include "../ErrorCodes.h"
#include "../Utils/BiometricAuthenticationHandler.h"
#include "Scene.h"

using namespace cocos2d;

#define PIN_Z_ORDER 3000

NS_AZOOMEE_BEGIN

bool RequestAdultPinLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setName("RequestPinLayer");
    
    this->setPosition(dynamic_cast<Azoomee::Scene*>(Director::getInstance()->getRunningScene()) ? Vec2(0,0) : -Director::getInstance()->getVisibleOrigin());
    
    AudioMixer::getInstance()->pauseBackgroundMusic();
    
    createBackgroundLayer();
    addListenerToBackgroundLayer();
    addListenerToBiometricValidationSuccess();
    addListenerToBiometricValidationFailure();
    
    if(BiometricAuthenticationHandler::getInstance()->biometricAuthenticationSet())
    {
        BiometricAuthenticationHandler::getInstance()->startBiometricAuthentication();
    }
    setPercentageofScreenForBox();
    addUIObjects();
    addOrRemoveWires();
    
    return true;
}

//---------------------- Create Layer -----------------------------

void RequestAdultPinLayer::createBackgroundLayer()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    
    backgroundLayer = LayerColor::create(Color4B(15,14,7,255),currentRunningScene->getContentSize().width, currentRunningScene->getContentSize().height);
    this->addChild(backgroundLayer);
    
    currentRunningScene->addChild(this, PIN_Z_ORDER);
}

void RequestAdultPinLayer::addListenerToBiometricValidationSuccess()
{
    _biometricValidationSuccessListener = cocos2d::EventListenerCustom::create(BiometricAuthenticationHandler::kBiometricValidationSuccess, [=](EventCustom* event) {
        auto funcCallAction = CallFunc::create([=](){
            this->scheduleOnce(schedule_selector(RequestAdultPinLayer::removeSelf), 0.1);
            
            if(this->getDelegate())
            {
                this->getDelegate()->AdultPinAccepted(this);
            }
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
    });
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_biometricValidationSuccessListener, 1);
}

void RequestAdultPinLayer::addListenerToBiometricValidationFailure()
{
    _biometricValidationFailureListener = cocos2d::EventListenerCustom::create(BiometricAuthenticationHandler::kBiometricValidationFailure, [=](EventCustom* event) {
        auto funcCallAction = CallFunc::create([=](){
            MessageBox::createWith(BiometricAuthenticationHandler::kAuthFailedDialogTitle, BiometricAuthenticationHandler::kAuthFailedDialogBody, BiometricAuthenticationHandler::kAuthFailedDialogButtons, this);
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
    });
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_biometricValidationFailureListener, 1);
}

void RequestAdultPinLayer::addListenerToBackgroundLayer()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event) 
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), backgroundLayer);
}

void RequestAdultPinLayer::setPercentageofScreenForBox()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    
    if(currentRunningScene->getContentSize().width < currentRunningScene->getContentSize().height)
        percentageOfScreenForBox = 0.85;
    else
        percentageOfScreenForBox = 0.66;
}

void RequestAdultPinLayer::addUIObjects()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    
    //-----------CREATE WINDOW LAYER-----------
    Rect spriteRect = Rect(0, 0, 467, 230);
    Rect capInsents = Rect(100, 100, 255, 1);
    
    windowLayer = ui::Scale9Sprite::create("res/decoration/whiteWindowScale9.png", spriteRect, capInsents);
    windowLayer->setContentSize(Size(currentRunningScene->getContentSize().width * percentageOfScreenForBox, 750));
    windowLayer->setPosition(currentRunningScene->getContentSize().width/2,currentRunningScene->getContentSize().height * 0.72f);
    this->addChild(windowLayer);
    
    //-------ACCEPT PLACEHOLDER BUTTON-------
    
    placeHolderAcceptButton= ElectricDreamsButton::createPlaceHolderButton();
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
    
    editBox_pin->focusAndShowKeyboard();
    
    //--------- LOCATION FOR ACCEPT BUTTON---------
    
    placeHolderAcceptButton->setPosition(Vec2(editBox_pin->getPositionX() + editBox_pin->getContentSize().width + acceptButton->getContentSize().width/2,editBox_pin->getPositionY()+editBox_pin->getContentSize().height/2 - placeHolderAcceptButton->getContentSize().height/2));
    acceptButton->setPosition(placeHolderAcceptButton->getPosition());
    
    //---------- MODAL LABEL ------------
    
    enterYourPinTitle = createLabelButtonAdultPrimary(StringMgr::getInstance()->getStringForKey(PIN_REQUEST_LABEL));
    enterYourPinTitle->setPosition(editBox_pin->getPositionX() + enterYourPinTitle->getContentSize().width/2, windowLayer->getContentSize().height*.66+enterYourPinTitle->getContentSize().height/2);
    windowLayer->addChild(enterYourPinTitle);
    
    //-------- CLOSE BUTTON ----------
    
    cancelButton = ElectricDreamsButton::createWindowCloseButtonGreen();
    cancelButton->setCenterPosition(Vec2(windowLayer->getContentSize().width-cancelButton->getContentSize().width*0.75, windowLayer->getContentSize().height-cancelButton->getContentSize().height*.75));
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("PinCancelButton");
    windowLayer->addChild(cancelButton);
}

void RequestAdultPinLayer::resizeWindowAndObjects()
{
    setPercentageofScreenForBox();
    
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    
    windowLayer->setContentSize(Size(currentRunningScene->getContentSize().width * percentageOfScreenForBox, 750));
    windowLayer->setPosition(currentRunningScene->getContentSize().width/2,currentRunningScene->getContentSize().height * 0.72f);
    
    editBox_pin->setNewWidth(windowLayer->getContentSize().width/2);
    editBox_pin->setPosition(Vec2(windowLayer->getContentSize().width/2 - editBox_pin->getContentSize().width/2 - acceptButton->getContentSize().width * 0.66f, windowLayer->getContentSize().height * 0.25f));

    placeHolderAcceptButton->setPosition(Vec2(editBox_pin->getPositionX() + editBox_pin->getContentSize().width + acceptButton->getContentSize().width/2,editBox_pin->getPositionY()));
    
    acceptButton->setPosition(placeHolderAcceptButton->getPosition());
    
    enterYourPinTitle->setPosition(editBox_pin->getPositionX() + enterYourPinTitle->getContentSize().width/2, windowLayer->getContentSize().height*.66+enterYourPinTitle->getContentSize().height/2);
    
    cancelButton->setCenterPosition(Vec2(windowLayer->getContentSize().width-cancelButton->getContentSize().width * 0.75f, windowLayer->getContentSize().height-cancelButton->getContentSize().height * 0.75f));
}

void RequestAdultPinLayer::addOrRemoveWires()
{
    removeWiresFromScreen(backgroundLayer);
    
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    
    if(currentRunningScene->getContentSize().width > currentRunningScene->getContentSize().height)
        addSideWiresToScreen(backgroundLayer);
}

void RequestAdultPinLayer::onSizeChanged()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    backgroundLayer->setContentSize(currentRunningScene->getContentSize());
    
    resizeWindowAndObjects();
    addOrRemoveWires();
}


//---------------------- Actions -----------------

void RequestAdultPinLayer::removeSelf(float dt)
{
    if(this)
    {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_biometricValidationSuccessListener);
        Director::getInstance()->getEventDispatcher()->removeEventListener(_biometricValidationFailureListener);
        this->removeChild(backgroundLayer);
        this->removeFromParent();
    }
}

void RequestAdultPinLayer::onExit()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(_biometricValidationSuccessListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(_biometricValidationFailureListener);
    
    Node::onExit();
}

void RequestAdultPinLayer::requestUpdatedPin()
{
    ModalMessages::getInstance()->startLoading();
    HttpRequestCreator* request = API::UpdateParentPinRequest(this);
    request->execute();
}

void RequestAdultPinLayer::checkPinAgainstStoredPin()
{
    ModalMessages::getInstance()->stopLoading();
    
    if(editBox_pin->getText() == ParentDataProvider::getInstance()->getParentPin() || ("" == ParentDataProvider::getInstance()->getParentPin() && editBox_pin->getText() == "1234"))
    {
        //Schedule so it calls delegate before removing self. Avoiding crash
        this->scheduleOnce(schedule_selector(RequestAdultPinLayer::removeSelf), 0.1);
        
        if(this->getDelegate())
        this->getDelegate()->AdultPinAccepted(this);
    }
    else
    {
        editBox_pin->setText("");
        editBox_pin->setEditboxVisibility(false);
        MessageBox::createWith(ERROR_CODE_INCORRECT_PIN, this);
        acceptButton->setVisible(false);
    }
}

//----------------------- Delegate Functions ----------------------------

void RequestAdultPinLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    acceptButton->setVisible(isValid);
}

void RequestAdultPinLayer::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(editBox_pin->inputIsValid())
        requestUpdatedPin();
}

void RequestAdultPinLayer::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    
}

void RequestAdultPinLayer::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    
}

void RequestAdultPinLayer::buttonPressed(ElectricDreamsButton* button)
{
    if(button == cancelButton)
    {
        AudioMixer::getInstance()->resumeBackgroundMusic();
        //Schedule so it calls delegate before removing self. Avoiding crash
        this->scheduleOnce(schedule_selector(RequestAdultPinLayer::removeSelf), 0.1);
        if(this->getDelegate())
            this->getDelegate()->AdultPinCancelled(this);
    }
    else if(button == acceptButton)
        requestUpdatedPin();
}

void RequestAdultPinLayer::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
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
        
        this->scheduleOnce(schedule_selector(RequestAdultPinLayer::removeSelf), 0.1);
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
    editBox_pin->setEditboxVisibility(true);
    editBox_pin->focusAndShowKeyboard();
    
}

void RequestAdultPinLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{

    ParentDataParser::getInstance()->parseUpdateParentData(body);

    checkPinAgainstStoredPin();
}

void RequestAdultPinLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    checkPinAgainstStoredPin();
}

void RequestAdultPinLayer::setPinIsForPayment(bool pinIsForPayment)
{
    _pinIsForPayment = pinIsForPayment;
}

NS_AZOOMEE_END
