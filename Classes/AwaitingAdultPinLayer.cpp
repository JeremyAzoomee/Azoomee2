#include "AwaitingAdultPinLayer.h"
#include "ModalMessages.h"
#include "StringStorage.h"
#include "BackEndCaller.h"
#include "ParentDataProvider.h"
#include "AudioMixer.h"

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
    backgroundLayer = LayerColor::create(Color4B(0,0,0,255),origin.x+ visibleSize.width, origin.y + visibleSize.height);
    
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
    //---------- MODAL LABEL ------------
    
    auto enterYourPinTitle = Label::createWithTTF(PIN_REQUEST_TITLE, "fonts/azoomee.ttf", 90);
    enterYourPinTitle->setPosition(origin.x+visibleSize.width /2, origin.y+visibleSize.height*0.8);
    enterYourPinTitle->setColor(Color3B::WHITE);
    enterYourPinTitle->setOpacity(0);
    backgroundLayer->addChild(enterYourPinTitle);
    
    enterYourPinTitle->runAction(FadeTo::create(0.5, 255));
    
    //-------- PIN EDITBOX -------------
    
    editBox_pin = TextInputLayer::createWithSize(Size(400,131), INPUT_IS_PIN);
    editBox_pin->setCenterPosition(Vec2(origin.x+visibleSize.width /2, origin.y+visibleSize.height*0.65));
    editBox_pin->setDelegate(this);
    backgroundLayer->addChild(editBox_pin);
    
    editBox_pin->focusAndShowKeyboard();
    
    //-------- CANCEL BUTTON ----------
    
    cancelButton = ElectricDreamsButton::createCancelButton();
    cancelButton->setCenterPosition(Vec2(origin.x + visibleSize.width /2 - 400, origin.y + visibleSize.height * 0.65));
    cancelButton->setDelegate(this);
    backgroundLayer->addChild(cancelButton);
    
    //------- ACCEPT BUTTON -----------
    
    acceptButton = ElectricDreamsButton::createAcceptButton();
    acceptButton->setCenterPosition(Vec2(origin.x + visibleSize.width /2 + 400, origin.y + visibleSize.height * 0.65));
    acceptButton->setDelegate(this);
    acceptButton->setVisible(false);
    backgroundLayer->addChild(acceptButton);
    
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

//---------------------- public Functions After Setup -----------------------------

void AwaitingAdultPinLayer::setCenterPosition(Vec2 position)
{
    this->setPosition(Vec2(position.x - this->getContentSize().width/2, position.y - this->getContentSize().height/2));
}

Vec2 AwaitingAdultPinLayer::getCenterPosition()
{
    return Vec2(this->getPositionX() + this->getContentSize().width/2, this->getPositionY() + this->getContentSize().height/2);
}

//----------------------- Delegate Functions ----------------------------

void AwaitingAdultPinLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    acceptButton->setVisible(isValid);
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
        BackEndCaller::getInstance()->updateParent(this, "pin");
}

void AwaitingAdultPinLayer::secondCheckForPin()
{
    //Please implement your second check here. If first check is not okay, please call: BackEndCaller::getInstance->updateParent(this);
    CCLOG("Second check for pin callback was called");
    
    if(editBox_pin->getText() == ParentDataProvider::getInstance()->getParentPin() || ("" == ParentDataProvider::getInstance()->getParentPin() && editBox_pin->getText() == "1234"))
    {
        //Schedule so it calls delegate before removing self. Avoiding crash
        this->scheduleOnce(schedule_selector(AwaitingAdultPinLayer::removeSelf), 0.1);
        this->getDelegate()->AdultPinAccepted(this);
    }
    else
    {
        ModalMessages::getInstance()->createErrorMessage(ERROR_CODE_INCORRECT_PIN,editBox_pin);
        acceptButton->setVisible(false);
    }
    
}
