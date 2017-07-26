#include "RequestAdultPinLayer.h"
#include "../Strings.h"
#include "../Data/Parent/ParentDataProvider.h"
#include "../Audio/AudioMixer.h"
#include "../UI/ElectricDreamsTextStyles.h"
#include "../UI/ElectricDreamsDecoration.h"
#include "../API/API.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool RequestAdultPinLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setName("RequestPinLayer");
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    AudioMixer::getInstance()->pauseBackgroundMusic();
    
    createBackgroundLayer();
    addListenerToBackgroundLayer();
    onSizeChanged();
    
    return true;
}

//---------------------- Create Layer -----------------------------

void RequestAdultPinLayer::createBackgroundLayer()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    
    backgroundLayer = LayerColor::create(Color4B(15,14,7,255),currentRunningScene->getContentSize().width, currentRunningScene->getContentSize().height);
    this->addChild(backgroundLayer);
    
    currentRunningScene->addChild(this);
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

void RequestAdultPinLayer::addUIObjects()
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

void RequestAdultPinLayer::onSizeChanged()
{
    auto currentRunningScene = Director::getInstance()->getRunningScene();
    backgroundLayer->setContentSize(currentRunningScene->getContentSize());
    backgroundLayer->removeAllChildren();
    
    if(windowLayer)
        windowLayer->removeAllChildren();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    if(currentRunningScene->getContentSize().width < currentRunningScene->getContentSize().height)
    {
        //percentageOfScreenForBox = 0.85;
    }
    else
    {
        //percentageOfScreenForBox = 0.66;
        addSideWiresToScreen(backgroundLayer, 0, 2);
    }
    
    addUIObjects();
}


//---------------------- Actions -----------------

void RequestAdultPinLayer::removeSelf(float dt)
{
    if(this)
    {
        this->removeChild(backgroundLayer);
        this->removeFromParent();
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
    {
        HttpRequestCreator* request = API::UpdateParentPinRequest(this);
        request->execute();
    }
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
        this->getDelegate()->AdultPinCancelled(this);
    }
    else if(button == acceptButton)
    {
        HttpRequestCreator* request = API::UpdateParentPinRequest(this);
        request->execute();
    }
}

void RequestAdultPinLayer::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    editBox_pin->focusAndShowKeyboard();
}

void RequestAdultPinLayer::secondCheckForPin()
{
    //Please implement your second check here. If first check is not okay, please call: BackEndCaller::getInstance->updateParentPin(this);
    CCLOG("Second check for pin callback was called");
    
    if(editBox_pin->getText() == ParentDataProvider::getInstance()->getParentPin() || ("" == ParentDataProvider::getInstance()->getParentPin() && editBox_pin->getText() == "1234"))
    {
        //Schedule so it calls delegate before removing self. Avoiding crash
        this->scheduleOnce(schedule_selector(RequestAdultPinLayer::removeSelf), 0.1);
        this->getDelegate()->AdultPinAccepted(this);
    }
    else
    {
        editBox_pin->setText("");
        MessageBox::createWith(ERROR_CODE_INCORRECT_PIN, editBox_pin, this);
        acceptButton->setVisible(false);
    }
}

void RequestAdultPinLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{

}

void RequestAdultPinLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    //do nothing
}

NS_AZOOMEE_END
