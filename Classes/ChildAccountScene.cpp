#include "ChildAccountScene.h"
#include "BackEndCaller.h"
#include "ChildSelectorScene.h"
#include "TextInputChecker.h"
#include "StringMgr.h"
#include "ConfigStorage.h"
#include "AudioMixer.h"
#include "AnalyticsSingleton.h"
#include "MessageBox.h"
#include "ElectricDreamsTextStyles.h"
#include "ElectricDreamsDecoration.h"
#include "ModalMessages.h"

USING_NS_CC;

#define NO_OF_OOMEES 5

Scene* ChildAccountScene::createScene(std::string ChildName, long errorCode)
{
    auto scene = Scene::create();
    auto layer = ChildAccountScene::create();
    
    scene->addChild(layer);
    
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
    
    addSideWiresToScreen(this, 0, 2);
    addLabelToScene();
    addTextboxScene();
    addButtonsScene();
    addOomeesToScene();
    
    return true;
}

void ChildAccountScene::onEnterTransitionDidFinish()
{
    if(_errorCode !=0)
    {
        MessageBox::createWith(_errorCode, childNameInputText, this);
    }
    else
        childNameInputText->focusAndShowKeyboard();
}

//----------------- SCENE SETUP ---------------
void ChildAccountScene::addLabelToScene()
{
    title = createLabelHeader(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_NAME_LABEL));
    this->addChild(title);
    
    subTitle = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_OOMEE_SUB_LABEL));
    subTitle->setVisible(false);
    this->addChild(subTitle);
}

void ChildAccountScene::addTextboxScene()
{
    childNameInputText = TextInputLayer::createWithSize(Size(1500,197), INPUT_IS_CHILD_NAME);
    childNameInputText->setDelegate(this);
    this->addChild(childNameInputText);
    
    dayInputText = TextInputLayer::createWithSize(Size(250,197), INPUT_IS_DAY);
    dayInputText->setPositionX(origin.x+visibleSize.width/2 - 400-dayInputText->getContentSize().width/2);
    dayInputText->setEditboxVisibility(false);
    dayInputText->setDelegate(this);
    this->addChild(dayInputText);
    
    monthInputText = TextInputLayer::createWithSize(Size(250,197), INPUT_IS_MONTH);
    monthInputText->setPositionX(origin.x+visibleSize.width/2 - 50-monthInputText->getContentSize().width/2);
    monthInputText->setEditboxVisibility(false);
    monthInputText->setDelegate(this);
    this->addChild(monthInputText);
    
    yearInputText = TextInputLayer::createWithSize(Size(350,197), INPUT_IS_YEAR);
    yearInputText->setPositionX(origin.x+visibleSize.width/2 + 350-yearInputText->getContentSize().width/2);
    yearInputText->setEditboxVisibility(false);
    yearInputText->setDelegate(this);
    this->addChild(yearInputText);
}

void ChildAccountScene::addButtonsScene()
{
    backButton = ElectricDreamsButton::createBackButton();
    backButton->setCenterPosition(Vec2(origin.x +backButton->getContentSize().width*.7, origin.y + visibleSize.height - backButton->getContentSize().height*.7));
    backButton->setDelegate(this);
    this->addChild(backButton);
    
    nextButton = ElectricDreamsButton::createNextButton();
    nextButton->setCenterPosition(Vec2(origin.x + visibleSize.width -nextButton->getContentSize().width*.7, origin.y+ visibleSize.height - nextButton->getContentSize().height*.7));
    nextButton->setDelegate(this);
    nextButton->setVisible(false);
    this->addChild(nextButton);
}

//------------CHANGE SCREEN VISUALS ON BUTTON PRESS----------------------
void ChildAccountScene::clearElementsOnScreen()
{
    childNameInputText->setEditboxVisibility(false);
    dayInputText->setEditboxVisibility(false);
    monthInputText->setEditboxVisibility(false);
    yearInputText->setEditboxVisibility(false);
    subTitle->setVisible(false);
    nextButton->setVisible(false);
    hideOomees();
}

void ChildAccountScene::changeElementsToDOBScreen()
{
    clearElementsOnScreen();
    
    std::string DOBLabel = StringUtils::format("%s %s.", StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_DOB_LABEL).c_str(), childNameInputText->getText().c_str());
    
    title->setString(DOBLabel);
    subTitle->setString(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_DOB_SUB_LABEL));
    subTitle->setVisible(true);
    
    dayInputText->setEditboxVisibility(true);
    monthInputText->setEditboxVisibility(true);
    yearInputText->setEditboxVisibility(true);
    
    setDOBNextButtonVisible();
    
    currentScreen = childDOBScreen;
    dayInputText->focusAndShowKeyboard();
}

void ChildAccountScene::changeElementsToChildNameScreen()
{
    clearElementsOnScreen();
    
    title->setString(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_NAME_LABEL));
    
    childNameInputText->setEditboxVisibility(true);
    currentScreen = childNameScreen;
    nextButton->setVisible(isValidChildName(childNameInputText->getText().c_str()));
    childNameInputText->focusAndShowKeyboard();
}

void ChildAccountScene::changeElementsToOomeeScreen()
{
    clearElementsOnScreen();
    
    title->setString(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_OOMEE_LABEL));
    subTitle->setString(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_OOMEE_SUB_LABEL));
    subTitle->setVisible(true);
    
    currentScreen = childSelectOomeeScreen;
    
    showOomees();
}

void ChildAccountScene::backButtonPressed()
{
    if(currentScreen == childNameScreen)
    {
        auto childSelectorScene = ChildSelectorScene::createScene(0);
        Director::getInstance()->replaceScene(childSelectorScene);
    }
    else if(currentScreen == childDOBScreen)
        changeElementsToChildNameScreen();
    else if(currentScreen == childSelectOomeeScreen)
        changeElementsToDOBScreen();
}

void ChildAccountScene::nextButtonPressed()
{
    if(currentScreen == childNameScreen && childNameExists(childNameInputText->getText()))
    {
        MessageBox::createWith(ERROR_CODE_NAME_EXISTS, childNameInputText, this);
        AnalyticsSingleton::getInstance()->childProdileNameErrorEvent();
    }
    else if(currentScreen == childNameScreen)
    {
        AnalyticsSingleton::getInstance()->childProfileNameEvent();
        changeElementsToDOBScreen();
    }
    else if(currentScreen == childDOBScreen)
    {
        AnalyticsSingleton::getInstance()->childProfileDOBEvent();
        changeElementsToOomeeScreen();
    }
    else if(currentScreen == childSelectOomeeScreen)
        registerChildAccount();
}

//----------------OOMEE SCREEN FUNCTIONS---------------
void ChildAccountScene::addOomeesToScene()
{
    oomeeGlow = createGlow();
    oomeeGlow->setVisible(false);
    oomeeGlow->setScale(.6);
    this->addChild(oomeeGlow);
    
    for(int i=0;i< NO_OF_OOMEES;i++)
    {
        auto oomeeButton = ElectricDreamsButton::createOomeeAsButton(i);
        oomeeButton->setDelegate(this);
        oomeeButton->setVisible(false);
        oomeeButton->setScale(1.3);
        this->addChild(oomeeButton);
        OomeeButtons.push_back(oomeeButton);
    }
    
    OomeeButtons.at(0)->setCenterPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height*.55));
    OomeeButtons.at(1)->setCenterPosition(Vec2(origin.x + visibleSize.width*.2, origin.y + visibleSize.height*.55));
    OomeeButtons.at(2)->setCenterPosition(Vec2(origin.x + visibleSize.width*.8, origin.y + visibleSize.height*.55));
    OomeeButtons.at(3)->setCenterPosition(Vec2(origin.x + visibleSize.width*.35, origin.y + visibleSize.height*.3));
    OomeeButtons.at(4)->setCenterPosition(Vec2(origin.x + visibleSize.width*.65, origin.y + visibleSize.height*.3));
}

void ChildAccountScene::hideOomees()
{
    for(int i=0;i< OomeeButtons.size();i++)
    {
        OomeeButtons.at(i)->setVisible(false);
        OomeeButtons.at(i)->setScale(1.3);
    }
    
    oomeeGlow->setVisible(false);
}

void ChildAccountScene::showOomees()
{
    for(int i=0;i< OomeeButtons.size();i++)
    {
        OomeeButtons.at(i)->setVisible(true);
        OomeeButtons.at(i)->playOomeeAnimationNoSound("Build_Simple_Wave");
    }
}

void ChildAccountScene::selectOomee(int oomeeNumber)
{
    selectedOomeeNo = oomeeNumber;
    
    for(int i=0;i< OomeeButtons.size();i++)
    {
        if(OomeeButtons.at(i)->getTag() == oomeeNumber)
        {
            nextButton->setVisible(true);
            selectedOomeeNo = oomeeNumber;
            OomeeButtons.at(i)->playOomeeAnimation("Build_Dance_Wave", true);
            OomeeButtons.at(i)->setScale(1.7);
            oomeeGlow->setPosition(OomeeButtons.at(i)->getCenterPosition());
            oomeeGlow->setVisible(true);
        }
        else
        {
            OomeeButtons.at(i)->playOomeeAnimationNoSound("Build_Fall_Asleep");
            OomeeButtons.at(i)->setScale(1);
        }
    }
}

//------------PRIVATE OTHER FUNCTIONS------------
void ChildAccountScene::registerChildAccount()
{
    std::string profileName = childNameInputText->getText();
    
    int day = std::atoi(dayInputText->getText().c_str());
    int month = std::atoi(monthInputText->getText().c_str());
    int year = std::atoi(yearInputText->getText().c_str());
    
    std::string DOB = StringUtils::format("%04d-%02d-%02d",year,month,day);
    std::string gender = "MALE";
    
    auto backEndCaller = BackEndCaller::getInstance();
    backEndCaller->registerChild(profileName, gender, DOB, this->selectedOomeeNo);
}

void ChildAccountScene::setDOBNextButtonVisible()
{
    int day = std::atoi(dayInputText->getText().c_str());
    int month = std::atoi(monthInputText->getText().c_str());
    int year = std::atoi(yearInputText->getText().c_str());
    
    nextButton->setVisible(isDate(day, month, year));
}

//----------------------- Delegate Functions ----------------------------

void ChildAccountScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    if(currentScreen == childDOBScreen)
        setDOBNextButtonVisible();
    else
        nextButton->setVisible(isValid);
}

void ChildAccountScene::buttonPressed(ElectricDreamsButton* button)
{
    AudioMixer::getInstance()->stopOomeeEffect();
    
    if(button == nextButton)
        nextButtonPressed();
    else if(button == backButton)
        backButtonPressed();
    else
        selectOomee(button->getTag());
}

void ChildAccountScene::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(currentScreen == childNameScreen)
        childNameInputText->focusAndShowKeyboard();
    else if(currentScreen == childDOBScreen)
        dayInputText->focusAndShowKeyboard();
}
