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
    
    if(BiometricAuthenticationHandler::getInstance()->biometricAuthenticationAvailable() && BiometricAuthenticationHandler::getInstance()->biometricAuthenticationSet())
    {
        BiometricAuthenticationHandler::getInstance()->startBiometricAuthentication();
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
			const std::vector<std::string>& buttons = {_("Retry"), _("Cancel")};
			MessageBox::createWith(_("Authentication Failed"), _("Oops! Biometric authentication failed."), buttons, this);
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
    
    editBox_pin->focusAndShowKeyboard();
	
	_accept->setEnabled(editBox_pin->inputIsValid());
	editBox_pin->addChild(_accept,1);
    
    //---------- MODAL LABEL ------------
    
	enterYourPinTitle = Label::createWithTTF(_("You are now entering the Parents’ Zone. Enter your Parent PIN to continue."), Style::Font::Regular(), 65);
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
        requestUpdatedPin();
}

void RequestAdultPinLayer::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    
}

void RequestAdultPinLayer::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    
}

void RequestAdultPinLayer::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(messageBoxTitle == _("Biometric Authentication"))
    {
        if(buttonTitle == _("Yes"))
        {
            BiometricAuthenticationHandler::getInstance()->startBiometricAuthentication();
        }
        
        if(buttonTitle == _("No"))
        {
            BiometricAuthenticationHandler::getInstance()->biometricAuthenticationNotNeeded();
        }
        
        this->scheduleOnce(schedule_selector(RequestAdultPinLayer::removeSelf), 0.1);
        return;
    }
    
    if(messageBoxTitle == _("Authentication Failed"))
    {
        if(buttonTitle == _("Retry"))
        {
            BiometricAuthenticationHandler::getInstance()->startBiometricAuthentication();
            return;
        }
        
        if(buttonTitle == _("Cancel"))
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
