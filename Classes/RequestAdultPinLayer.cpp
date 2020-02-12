#include "RequestAdultPinLayer.h"
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include "ModalMessages.h"
#include "ErrorCodes.h"
#include <AzoomeeCommon/Utils/BiometricAuthenticationHandler.h>
#include <AzoomeeCommon/UI/Scene.h>
#include "PopupMessageBox.h"
#include "Style.h"

using namespace cocos2d;

#define PIN_Z_ORDER 3000

NS_AZOOMEE_BEGIN

const std::string RequestAdultPinLayer::kAndroidMsgBoxName = "androidBiometricMessageBox";

bool RequestAdultPinLayer::init()
{
    if ( !Super::init() )
    {
        return false;
    }
    
    this->setName("RequestPinLayer");
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0,1.0f));
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    
    AudioMixer::getInstance()->pauseBackgroundMusic();
    
    createBackgroundLayer();
    addListenerToBackgroundLayer();
    addListenerToBiometricValidationSuccess();
    addListenerToBiometricValidationFailure();
    
    if(BiometricAuthenticationHandler::getInstance()->biometricAuthenticationAvailable() && BiometricAuthenticationHandler::getInstance()->biometricAuthenticationSet())
    {
        BiometricAuthenticationHandler::getInstance()->startBiometricAuthentication();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        PopupMessageBox* messageBox = PopupMessageBox::create();
        messageBox->setName(kAndroidMsgBoxName);
        messageBox->setTitle(_("Waiting for authentication"));
        messageBox->setBody(_("Please authenticate yourself to access parents area"));
        messageBox->setButtonText(_("Cancel"));
        messageBox->setButtonColour(Colours::Color_3B::strongPink);
        messageBox->setButtonPressedCallback([](PopupMessageBox* pSender){
            pSender->removeFromParent();
            BiometricAuthenticationHandler::getInstance()->stopBiometricAuthentication();
        });
        addChild(messageBox, PIN_Z_ORDER);
#endif
    }
    setPercentageofScreenForBox();
    addUIObjects();
    
    return true;
}

//---------------------- Create Layer -----------------------------

void RequestAdultPinLayer::createBackgroundLayer()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    
    backgroundLayer = LayerColor::create(Color4B(155,155,155,204),currentRunningScene->getContentSize().width, currentRunningScene->getContentSize().height);
    this->addChild(backgroundLayer);
    
    currentRunningScene->addChild(this, PIN_Z_ORDER);
}

void RequestAdultPinLayer::addListenerToBiometricValidationSuccess()
{
    _biometricValidationSuccessListener = cocos2d::EventListenerCustom::create(BiometricAuthenticationHandler::kBiometricValidationSuccess, [=](EventCustom* event) {
        auto funcCallAction = CallFunc::create([=](){
            removeChildByName(kAndroidMsgBoxName);
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
            removeChildByName(kAndroidMsgBoxName);
            PopupMessageBox* messageBox = PopupMessageBox::create();
            messageBox->setName(kAndroidMsgBoxName);
            messageBox->setTitle(_("Authentication Failed"));
            messageBox->setBody(_("Oops! Biometric authentication failed."));
            messageBox->setButtonText(_("Retry"));
            messageBox->setButtonColour(Colours::Color_3B::darkIndigo);
            messageBox->setButtonPressedCallback([](MessagePopupBase* pSender){
                pSender->removeFromParent();
                BiometricAuthenticationHandler::getInstance()->startBiometricAuthentication();
            });
            messageBox->setSecondButtonText(_("Cancel"));
            messageBox->setSecondButtonColour(Colours::Color_3B::strongPink);
            messageBox->setSecondButtonPressedCallback([this](MessagePopupBase* pSender){
                pSender->removeFromParent();
                editBox_pin->focusAndShowKeyboard();
            });
            addChild(messageBox, PIN_Z_ORDER);
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
    });
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_biometricValidationFailureListener, 1);
    
    _biometricValidationErrorListener = cocos2d::EventListenerCustom::create(BiometricAuthenticationHandler::kBiometricValidationError, [=](EventCustom* event) {
        auto funcCallAction = CallFunc::create([=](){
            removeChildByName(kAndroidMsgBoxName);
            editBox_pin->focusAndShowKeyboard();
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
    });
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_biometricValidationErrorListener, 1);
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
    {
        percentageOfScreenForBox = 0.85;
    }
    else
    {
        percentageOfScreenForBox = 0.66;
    }
}

void RequestAdultPinLayer::addUIObjects()
{
	const Size& visibleSize = Director::getInstance()->getVisibleSize();
	const bool isPortrait = visibleSize.width < visibleSize.height;
    //-----------CREATE WINDOW LAYER-----------
    
	windowLayer = ui::Scale9Sprite::create("res/settings/rounded_rect.png");
	windowLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	windowLayer->setPosition(Vec2(visibleSize.width * 0.5,visibleSize.height * 0.9));
	windowLayer->setContentSize(Size(visibleSize.width * 0.875,visibleSize.height * 0.4f));
    this->addChild(windowLayer);
	
	//------- CREATE ACCEPT BUTTON -----------
	
	_accept = ui::Button::create("res/settings/confirm_edit_button.png");
	_accept->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	_accept->setAnchorPoint(Vec2(0.1,0.5));
	_accept->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			this->requestUpdatedPin();
		}
	});
	
    //-------- PIN EDITBOX -------------
    
    editBox_pin = TextInputLayer::createSettingsBoxTextInput((windowLayer->getContentSize().width * 0.5f) - _accept->getContentSize().width, INPUT_IS_PIN);
	editBox_pin->setCenterPosition(Vec2((windowLayer->getContentSize().width * 0.5f) - (_accept->getContentSize().width * 0.5f), editBox_pin->getContentSize().height * 1.5f));
    editBox_pin->setDelegate(this);
	editBox_pin->setText(currentTypedPinNo);
    windowLayer->addChild(editBox_pin);
    
    if(!BiometricAuthenticationHandler::getInstance()->biometricAuthenticationSet())
    {
        editBox_pin->focusAndShowKeyboard();
    }
	
	_accept->setEnabled(editBox_pin->inputIsValid());
	editBox_pin->addChild(_accept,1);
    
    //---------- MODAL LABEL ------------
    
	enterYourPinTitle = Label::createWithTTF(_("You are now entering the Parents' Zone. Enter your Parent PIN to continue."), Style::Font::Regular(), 65);
	enterYourPinTitle->setTextColor(Color4B::BLACK);
	enterYourPinTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	enterYourPinTitle->setOverflow(Label::Overflow::SHRINK);
	enterYourPinTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	enterYourPinTitle->setVerticalAlignment(TextVAlignment::CENTER);
	if(isPortrait)
	{
		enterYourPinTitle->setPosition(windowLayer->getContentSize().width * 0.5f, windowLayer->getContentSize().height * 0.45f);
		enterYourPinTitle->setDimensions(windowLayer->getContentSize().width * 0.8f, windowLayer->getContentSize().height * 0.17f);
	}
	else
	{
		enterYourPinTitle->setPosition(windowLayer->getContentSize().width * 0.5f, windowLayer->getContentSize().height * 0.85f);
		enterYourPinTitle->setDimensions(windowLayer->getContentSize().width * 0.75f, windowLayer->getContentSize().height * 0.4f);
	}
    windowLayer->addChild(enterYourPinTitle);
	
	//-------- IMAGE ------------
	
	if(isPortrait)
	{
		Sprite* image = Sprite::create("res/settings/oomee_and_child.png");
		image->setNormalizedPosition(Vec2(0.5,0.65));
		windowLayer->addChild(image);
	}
	
    //-------- CLOSE BUTTON ----------
	
	ui::Button* cancel = ui::Button::create("res/settings/exit_button_red.png");
	cancel->setAnchorPoint(Vec2(-0.5,1.5));
	cancel->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	cancel->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			AudioMixer::getInstance()->resumeBackgroundMusic();
			//Schedule so it calls delegate before removing self. Avoiding crash
			this->scheduleOnce(schedule_selector(RequestAdultPinLayer::removeSelf), 0.1);
			if(this->getDelegate())
				this->getDelegate()->AdultPinCancelled(this);
		}
	});
	windowLayer->addChild(cancel);
	
}

void RequestAdultPinLayer::resizeWindowAndObjects()
{
	currentTypedPinNo = editBox_pin->getText();
	windowLayer->removeFromParent();
	addUIObjects();
}

void RequestAdultPinLayer::onSizeChanged()
{
    Super::onSizeChanged();
    
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    backgroundLayer->setContentSize(currentRunningScene->getContentSize());
    
    resizeWindowAndObjects();
}


//---------------------- Actions -----------------

void RequestAdultPinLayer::removeSelf(float dt)
{
    if(this)
    {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_biometricValidationSuccessListener);
        Director::getInstance()->getEventDispatcher()->removeEventListener(_biometricValidationFailureListener);
        Director::getInstance()->getEventDispatcher()->removeEventListener(_biometricValidationErrorListener);
        this->removeChild(backgroundLayer);
        this->removeFromParent();
    }
}

void RequestAdultPinLayer::onExit()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(_biometricValidationSuccessListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(_biometricValidationFailureListener);
    Director::getInstance()->getEventDispatcher()->removeEventListener(_biometricValidationErrorListener);
    
    Super::onExit();
}

void RequestAdultPinLayer::requestUpdatedPin()
{
    ModalMessages::getInstance()->startLoading();
    HttpRequestCreator* request = API::UpdateParentPinRequest(UserAccountManager::getInstance()->getLoggedInParentId(), this);
    request->execute();
}

void RequestAdultPinLayer::checkPinAgainstStoredPin()
{
    ModalMessages::getInstance()->stopLoading();
    
    if(editBox_pin->getText() == UserAccountManager::getInstance()->getParentPin() || ("" == UserAccountManager::getInstance()->getParentPin() && editBox_pin->getText() == "1234"))
    {
        //Schedule so it calls delegate before removing self. Avoiding crash
        this->scheduleOnce(schedule_selector(RequestAdultPinLayer::removeSelf), 0.1);
        
        if(this->getDelegate())
        {
            this->getDelegate()->AdultPinAccepted(this);
        }
    }
    else
    {
        editBox_pin->setText("");
        editBox_pin->setEditboxVisibility(false);
        const auto& errorMessageText = LocaleManager::getInstance()->getErrorMessageWithCode(ERROR_CODE_INCORRECT_PIN);
               
        PopupMessageBox* messageBox = PopupMessageBox::create();
        messageBox->setTitle(errorMessageText.at(ERROR_TITLE));
        messageBox->setBody(errorMessageText.at(ERROR_BODY));
        messageBox->setButtonText(_("Back"));
        messageBox->setButtonColour(Colours::Color_3B::darkIndigo);
        messageBox->setPatternColour(Colours::Color_3B::azure);
        messageBox->setButtonPressedCallback([this](MessagePopupBase* pSender){
            pSender->removeFromParent();
            editBox_pin->setEditboxVisibility(true);
            editBox_pin->focusAndShowKeyboard();
        });
        addChild(messageBox, 1);
        _accept->setEnabled(false);
    }
}

//----------------------- Delegate Functions ----------------------------

void RequestAdultPinLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    _accept->setEnabled(isValid);
}

void RequestAdultPinLayer::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(editBox_pin->inputIsValid())
    {
        requestUpdatedPin();
    }
}

void RequestAdultPinLayer::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    
}

void RequestAdultPinLayer::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    
}

void RequestAdultPinLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{

    UserAccountManager::getInstance()->parseUpdateParentData(body);

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