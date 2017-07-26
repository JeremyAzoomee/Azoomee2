#include "RequestAdultPinLayer.h"
#include "../Strings.h"
#include "../Data/Parent/ParentDataProvider.h"
#include "../Audio/AudioMixer.h"
#include "../UI/ElectricDreamsTextStyles.h"
#include "../UI/ElectricDreamsDecoration.h"
#include "../API/API.h"
#include "../Data/Parent/ParentDataParser.h"
#include "ModalMessages.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool RequestAdultPinLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setName("RequestPinLayer");
    
    AudioMixer::getInstance()->pauseBackgroundMusic();
    
    createBackgroundLayer();
    addListenerToBackgroundLayer();
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
    Rect spriteRect = Rect(0, 0, 455, 268);
    Rect capInsents = Rect(100, 100, 255, 1);
    
    windowLayer = ui::Scale9Sprite::create("res/decoration/windowScale9.png", spriteRect, capInsents);
    windowLayer->setContentSize(Size(currentRunningScene->getContentSize().width * percentageOfScreenForBox, 750));
    windowLayer->setPosition(currentRunningScene->getContentSize().width/2,currentRunningScene->getContentSize().height*.72);
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
    
    editBox_pin = TextInputLayer::createWithSize(Size(windowLayer->getContentSize().width/2,acceptButton->getContentSize().height), INPUT_IS_PIN);
    editBox_pin->setPosition(Vec2(windowLayer->getContentSize().width/2 - editBox_pin->getContentSize().width/2 - acceptButton->getContentSize().width*.66, windowLayer->getContentSize().height*.25));
    editBox_pin->setDelegate(this);
    windowLayer->addChild(editBox_pin);
    
    editBox_pin->focusAndShowKeyboard();
    
    //--------- LOCATION FOR ACCEPT BUTTON---------
    
    placeHolderAcceptButton->setPosition(Vec2(editBox_pin->getPositionX() + editBox_pin->getContentSize().width + acceptButton->getContentSize().width/2,editBox_pin->getPositionY()));
    acceptButton->setPosition(placeHolderAcceptButton->getPosition());
    
    //---------- MODAL LABEL ------------
    
    enterYourPinTitle = createLabelHeaderWhite(StringMgr::getInstance()->getStringForKey(PIN_REQUEST_LABEL));
    enterYourPinTitle->setPosition(editBox_pin->getPositionX() + enterYourPinTitle->getContentSize().width/2, windowLayer->getContentSize().height*.66+enterYourPinTitle->getContentSize().height/2);
    windowLayer->addChild(enterYourPinTitle);
    
    //-------- CLOSE BUTTON ----------
    
    cancelButton = ElectricDreamsButton::createWindowCloselButton();
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
    windowLayer->setPosition(currentRunningScene->getContentSize().width/2,currentRunningScene->getContentSize().height*.72);
    
    editBox_pin->setNewWidth(windowLayer->getContentSize().width/2);
    editBox_pin->setPosition(Vec2(windowLayer->getContentSize().width/2 - editBox_pin->getContentSize().width/2 - acceptButton->getContentSize().width*.66, windowLayer->getContentSize().height*.25));

    placeHolderAcceptButton->setPosition(Vec2(editBox_pin->getPositionX() + editBox_pin->getContentSize().width + acceptButton->getContentSize().width/2,editBox_pin->getPositionY()));
    
    acceptButton->setPosition(placeHolderAcceptButton->getPosition());
    
    enterYourPinTitle->setPosition(editBox_pin->getPositionX() + enterYourPinTitle->getContentSize().width/2, windowLayer->getContentSize().height*.66+enterYourPinTitle->getContentSize().height/2);
    
    cancelButton->setCenterPosition(Vec2(windowLayer->getContentSize().width-cancelButton->getContentSize().width*0.75, windowLayer->getContentSize().height-cancelButton->getContentSize().height*.75));
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
        this->removeChild(backgroundLayer);
        this->removeFromParent();
    }
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

NS_AZOOMEE_END
