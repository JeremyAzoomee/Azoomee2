#include "ChildAccountScene.h"
#include "ModalMessages.h"
#include "BackEndCaller.h"
#include "ChildSelectorScene.h"
#include "TextInputChecker.h"
#include "StringStorage.h"
#include "ConfigStorage.h"
#include "AudioMixer.h"

USING_NS_CC;

#define NO_OF_OOMEES 5

Scene* ChildAccountScene::createScene(std::string ChildName, long errorCode)
{
    auto scene = Scene::create();
    auto layer = ChildAccountScene::create();
    
    scene->addChild(layer);
    layer->setChildName(ChildName);
    layer->addFunctionalElementsToScene();
    
    layer->_errorCode = errorCode;
    
    return scene;
}

bool ChildAccountScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    addVisualElementsToScene();
    
    return true;
}

void ChildAccountScene::onEnterTransitionDidFinish()
{
    if(_errorCode !=0)
    {
        ModalMessages::getInstance()->createErrorMessage(_errorCode,editBox_childName);
    }
    
    editBox_childName->focusAndShowKeyboard();
}

//----------------- SCENE SETUP ---------------

void ChildAccountScene::setChildName(std::string ChildName)
{
    this->passedChildName = ChildName;

    if(ChildName.compare("") == 0)
        this->isNewChildAccount = true;
    else
        this->isNewChildAccount = false;
}

void ChildAccountScene::addVisualElementsToScene()
{
    auto bg = Sprite::create("res/mainhub/bg_glow.png");
    bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(bg);
    
    auto leftBg = Sprite::create("res/login/wire_left.png");
    leftBg->setPosition(0 + leftBg->getContentSize().width / 2,0);
    this->addChild(leftBg);
    
    auto rightBg = Sprite::create("res/login/wire_right.png");
    rightBg->setPosition(origin.x + visibleSize.width - rightBg->getContentSize().width / 2, 0);
    this->addChild(rightBg);
}

void ChildAccountScene::addFunctionalElementsToScene()
{
    addContentLayerToScene();
    addLabelsToLayer();
    addTextBoxesToLayer();
    addButtonsToLayer();
    addOomeesToLayer();
}

void ChildAccountScene::addContentLayerToScene()
{
    childAccountContent = Layer::create();
    childAccountContent->setContentSize(Size(visibleSize.width * 3, visibleSize.height));
    childAccountContent->setPosition(Point(origin.x, origin.y));
    childAccountContent->setName("childAccountContent");
    this->addChild(childAccountContent);
}

void ChildAccountScene::addLabelsToLayer()
{
    auto addChildNameTitle = Label::create();
    addChildNameTitle->setSystemFontName("fonts/azoomee.ttf");
    addChildNameTitle->setSystemFontSize(90);
    
    //Add diferent text depending on new or editing account
    if(this->isNewChildAccount)
        addChildNameTitle->setString(StringStorage::getInstance()->getStringForChildAccount("requestChildName"));
    else
        addChildNameTitle->setString(StringStorage::getInstance()->getStringForChildAccount("editChildName"));
    
    addChildNameTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.7);
    addChildNameTitle->setColor(Color3B(28, 244, 244));
    childAccountContent->addChild(addChildNameTitle);
    
    labelDob = Label::create();
    labelDob->setSystemFontName("fonts/azoomee.ttf");
    labelDob->setSystemFontSize(90);
    labelDob->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.7);
    labelDob->setColor(Color3B(28, 244, 244));
    childAccountContent->addChild(labelDob);
    
    auto labelDobSubTitle = Label::createWithTTF(StringStorage::getInstance()->getStringForChildAccount("requestChildBirthDaySubTitle"), "fonts/azoomee.ttf", 60);
    labelDobSubTitle->setPosition(origin.x + visibleSize.width * 1.5, origin.y + visibleSize.height * 0.6);
    labelDobSubTitle->setColor(Color3B::WHITE);
    childAccountContent->addChild(labelDobSubTitle);
    
    labelOomee = Label::create();
    labelOomee->setSystemFontName("fonts/azoomee.ttf");
    labelOomee->setSystemFontSize(90);
    if(this->isNewChildAccount)
        labelOomee->setString(StringStorage::getInstance()->getStringForChildAccount("requestChildOomee"));
    else
        labelOomee->setString(StringStorage::getInstance()->getStringForChildAccount("editChildName"));
    labelOomee->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.7);
    labelOomee->setColor(Color3B(28, 244, 244));
    childAccountContent->addChild(labelOomee);
    
    if(this->isNewChildAccount)
    {
        auto selectOomeeDetail = Label::createWithTTF(StringStorage::getInstance()->getStringForChildAccount("requestChildOomeeSub"), "fonts/azoomee.ttf", 60);
        selectOomeeDetail->setPosition(origin.x + visibleSize.width * 2.5, origin.y + visibleSize.height * 0.6);
        selectOomeeDetail->setColor(Color3B::WHITE);
        childAccountContent->addChild(selectOomeeDetail);
    }
}

void ChildAccountScene::addTextBoxesToLayer()
{
    editBox_childName = TextInputLayer::createWithSize(Size(736,131), INPUT_IS_CHILD_NAME);
    editBox_childName->setCenterPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height*0.5));
    editBox_childName->setDelegate(this);
    childAccountContent->addChild(editBox_childName);
    
    editBox_day = TextInputLayer::createWithSize(Size(250,131), INPUT_IS_DAY);
    editBox_day->setCenterPosition(Vec2(origin.x+visibleSize.width * 1.5 - 350, origin.y+visibleSize.height*0.5));
    editBox_day->setDelegate(this);
    childAccountContent->addChild(editBox_day);
    
    editBox_month = TextInputLayer::createWithSize(Size(250,131), INPUT_IS_MONTH);
    editBox_month->setCenterPosition(Vec2(origin.x+visibleSize.width * 1.5 - 50, origin.y+visibleSize.height*0.5));
    editBox_month->setDelegate(this);
    childAccountContent->addChild(editBox_month);
    
    editBox_year = TextInputLayer::createWithSize(Size(350,131), INPUT_IS_YEAR);
    editBox_year->setCenterPosition(Vec2(origin.x+visibleSize.width * 1.5 + 300, origin.y+visibleSize.height*0.5));
    editBox_year->setDelegate(this);
    childAccountContent->addChild(editBox_year);
}

void ChildAccountScene::addButtonsToLayer()
{
    buttonCloseChild = ElectricDreamsButton::createBackButton();
    buttonCloseChild->setCenterPosition(Vec2(origin.x + visibleSize.width * 0.2, origin.y + visibleSize.height * 0.5));
    buttonCloseChild->setDelegate(this);
    childAccountContent->addChild(buttonCloseChild);

    buttonNextChild = ElectricDreamsButton::createNextButton();
    buttonNextChild->setCenterPosition(Vec2(origin.x + visibleSize.width * 0.8, origin.y + visibleSize.height * 0.5));
    buttonNextChild->setDelegate(this);
    
    //only show button if there is text
    if(this->isNewChildAccount)
        buttonNextChild->setVisible(false);
    
    childAccountContent->addChild(buttonNextChild);
    
    buttonBackDob = ElectricDreamsButton::createBackButton();
    buttonBackDob->setCenterPosition(Vec2(origin.x + visibleSize.width * 1.2, origin.y + visibleSize.height * 0.5));
    buttonBackDob->setDelegate(this);
    childAccountContent->addChild(buttonBackDob);
    
    buttonNextDob = ElectricDreamsButton::createNextButton();
    buttonNextDob->setCenterPosition(Vec2(origin.x + visibleSize.width * 1.8, origin.y + visibleSize.height * 0.5));
    buttonNextDob->setDelegate(this);
    buttonNextDob->setVisible(false);
    childAccountContent->addChild(buttonNextDob);
    
    buttonBackOomee = ElectricDreamsButton::createBackButton();
    buttonBackOomee->setCenterPosition(Vec2(origin.x + visibleSize.width * 2.2, origin.y + visibleSize.height * 0.5));
    buttonBackOomee->setDelegate(this);
    childAccountContent->addChild(buttonBackOomee);
    
    buttonNextOomee = ElectricDreamsButton::createNextButton();
    buttonNextOomee->setCenterPosition(Vec2(origin.x + visibleSize.width * 2.8, origin.y + visibleSize.height * 0.5));
    buttonNextOomee->setScale(1.2);
    buttonNextOomee->setVisible(false);
    buttonNextOomee->setDelegate(this);
    childAccountContent->addChild(buttonNextOomee);
}

void ChildAccountScene::addOomeesToLayer()
{
    float gapSize = 50;
    float oomeeWidth = 190;
    float startX = origin.x + visibleSize.width * 2.5 - (oomeeWidth*2 + gapSize*2);
    
    for(int i = 0; i < NO_OF_OOMEES; i++)
    {
        auto oomee = Sprite::create(StringUtils::format(ConfigStorage::getInstance()->getOomeePNGName(i).c_str(),i));
        oomee->setPosition(startX + ((oomeeWidth+gapSize)*i), origin.y + visibleSize.height * 0.4);
        oomee->setOpacity(0);
        oomee->setTag(i);
        addListenerToOomeeButton(oomee);
        childAccountContent->addChild(oomee);
    }
}

// --------------------Oomee Selection Listener ----------------------
// TEMPORARY LISTENER. AWAITING SOLUTION FOR BUTTON THAT HAS OOMEE ANIMATION

void ChildAccountScene::addListenerToOomeeButton(Sprite *oomeeSprite)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Rect rect = Rect(0, 0, target->getContentSize().width, target->getContentSize().height);
        
        if(rect.containsPoint(locationInNode))
        {
            selectOomee(oomeeSprite);
            
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, oomeeSprite);
}

//------------------- Button Functions -----------------------

void ChildAccountScene::disableButton(Node* button)
{
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(button);
}

void ChildAccountScene::enableButton(Node* button)
{
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(button);
}

void ChildAccountScene::setTextInputFocus(TextInputLayer* textInputLayer)
{
    textInputLayer->focusAndShowKeyboard();
}

void ChildAccountScene::backButtonCloseScene()
{
    auto childSelectorScene = ChildSelectorScene::createScene(0);
    Director::getInstance()->replaceScene(childSelectorScene);
}

void ChildAccountScene::moveToAndSetupDOBInput(ElectricDreamsButton* button)
{
    setDOBLabel();
    
    std::string newChildsName = editBox_childName->getText();
    
    if(childNameExists(newChildsName) && passedChildName != newChildsName)
    {
        ModalMessages::getInstance()->createErrorMessage(ERROR_CODE_NAME_EXISTS,editBox_childName);
        enableButton(button);
    }
    else
    {
        auto action = EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width + origin.x, origin.y)), 2);
        auto enableButtonCallback = CallFunc::create(CC_CALLBACK_0(ChildAccountScene::enableButton, this,button));
        auto setTextInputFocusCallback = CallFunc::create(CC_CALLBACK_0(ChildAccountScene::setTextInputFocus, this,editBox_day));
        
        auto sequence = Sequence::create(action, setTextInputFocusCallback, enableButtonCallback, NULL);
        childAccountContent->runAction(sequence);
    }
}

void ChildAccountScene::setDOBLabel()
{
    if(this->isNewChildAccount)
    {
        std::string newChildsName = editBox_childName->getText();
        labelDob->setString(StringUtils::format("%s %s.", StringStorage::getInstance()->getStringForChildAccount("requestChildBirthDay").c_str(), newChildsName.c_str()));
    }
    else
        labelDob->setString(StringStorage::getInstance()->getStringForChildAccount("editChildBirthDay").c_str());
}

void ChildAccountScene::moveBackToChildNameInput(ElectricDreamsButton* button)
{
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(origin.x, origin.y)), 2);
    auto enableButtonCallback = CallFunc::create(CC_CALLBACK_0(ChildAccountScene::enableButton, this,button));
    auto setTextInputFocusCallback = CallFunc::create(CC_CALLBACK_0(ChildAccountScene::setTextInputFocus, this,editBox_childName));
    
    auto sequence = Sequence::create(action, setTextInputFocusCallback, enableButtonCallback, NULL);
    childAccountContent->runAction(sequence);
}

void ChildAccountScene::moveToAndSetupOomeeSelection(ElectricDreamsButton* button)
{
    auto action = EaseInOut::create(MoveTo::create(1, Vec2(-visibleSize.width*2 + origin.x, origin.y)), 2);
    auto callback = CallFunc::create(CC_CALLBACK_0(ChildAccountScene::enableButton, this,button));
    
    auto sequence = Sequence::create(action, callback, NULL);
    childAccountContent->runAction(sequence);
    
    for(int i=0;i<NO_OF_OOMEES;i++)
    {
        Sprite* oomee = (Sprite *)childAccountContent->getChildByTag(i);
        float delayTime = (CCRANDOM_0_1()* 0.5) +1;
        oomee->runAction(Sequence::create(DelayTime::create(delayTime), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    }
}

void ChildAccountScene::selectOomee(Sprite* selectedOomee)
{
    AudioMixer::getInstance()->playEffect(SELECT_OOMEE_AUDIO_EFFECT);
    buttonNextOomee->setVisible(true);
    
    for(int i=0;i<NO_OF_OOMEES;i++)
    {
        cocos2d::Sprite* oomee = (cocos2d::Sprite *)childAccountContent->getChildByTag(i);
        oomee->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1)));
    }
    
    selectedOomee->runAction(EaseElasticOut::create(ScaleTo::create(0.5, 1.2)));
    
    this->selectedOomeeNo = selectedOomee->getTag();
}

void ChildAccountScene::registerChildAccount()
{
    std::string profileName = editBox_childName->getText();
    
    int day = std::atoi(editBox_day->getText().c_str());
    int month = std::atoi(editBox_month->getText().c_str());
    int year = std::atoi(editBox_year->getText().c_str());
    
    std::string DOB = StringUtils::format("%04d-%02d-%02d",year,month,day);
    std::string gender = "MALE";
    
    if(this->isNewChildAccount)
    {
       auto backEndCaller = BackEndCaller::getInstance();
        backEndCaller->registerChild(profileName, gender, DOB, this->selectedOomeeNo);
    }
}

//----------------------- Delegate Functions ----------------------------

void ChildAccountScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    if(inputLayer == editBox_childName )
        buttonNextChild->setVisible(isValid);
    else
    {
        int day = std::atoi(editBox_day->getText().c_str());
        int month = std::atoi(editBox_month->getText().c_str());
        int year = std::atoi(editBox_year->getText().c_str());
        
        buttonNextDob->setVisible(isDate(day, month, year));
    }
}

void ChildAccountScene::buttonPressed(ElectricDreamsButton* button)
{
    disableButton(button);
    
    if(button == buttonCloseChild) backButtonCloseScene();
    else if(button == buttonNextChild) moveToAndSetupDOBInput(button);
    else if(button == buttonBackDob) moveBackToChildNameInput(button);
    else if(button == buttonNextDob) moveToAndSetupOomeeSelection(button);
    else if(button == buttonBackOomee) moveToAndSetupDOBInput(button);
    else if(button == buttonNextOomee) registerChildAccount();
}
