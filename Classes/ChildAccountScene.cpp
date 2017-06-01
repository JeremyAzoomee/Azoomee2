#include "ChildAccountScene.h"
#include "BackEndCaller.h"
#include "ChildSelectorScene.h"
#include <AzoomeeCommon/Input/TextInputChecker.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "SceneManagerScene.h"
#include "FlowDataSingleton.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>

USING_NS_CC;
using namespace Azoomee;


#define NO_OF_OOMEES 5

Scene* ChildAccountScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ChildAccountScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool ChildAccountScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void ChildAccountScene::onEnter()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    ModalMessages::getInstance()->startLoading();
    
    AddTitleToScene();
    addTextboxScene();
    addLabelToScene();
    addButtonsScene();
    addOomeesToScene();

    
    Node::onEnter();
}

void ChildAccountScene::onEnterTransitionDidFinish()
{
    if(FlowDataSingleton::getInstance()->hasError())
    {
        childNameInputText->setVisible(false);
        dayInputText->setVisible(false);
        monthInputText->setVisible(false);
        yearInputText->setVisible(false);
        MessageBox::createWith(FlowDataSingleton::getInstance()->getErrorCode(), this);
    }
    else
        childNameInputText->focusAndShowKeyboard();
}

//----------------- SCENE SETUP ---------------

void ChildAccountScene::AddTitleToScene()
{
    sceneTitle = createLabelMessageBoxBody(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_TITLE_LABEL));
    sceneTitle->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height - sceneTitle->getContentSize().height * 1.5);
    this->addChild(sceneTitle);
}

void ChildAccountScene::addTextboxScene()
{
    childNameInputText = TextInputLayer::createWithSize(Size(visibleSize.width*.80,197), INPUT_IS_CHILD_NAME);
    childNameInputText->setPositionY(sceneTitle->getPositionY()-childNameInputText->getContentSize().height*2.25);
    childNameInputText->setDelegate(this);
    this->addChild(childNameInputText);
    
    dayInputText = TextInputLayer::createWithSize(Size(330,197), INPUT_IS_DAY);
    dayInputText->setPositionY(childNameInputText->getPositionY() -dayInputText->getContentSize().height*2.7 );
    dayInputText->setPositionX(origin.x+visibleSize.width/2 - 275-dayInputText->getContentSize().width);
    dayInputText->setDelegate(this);
    this->addChild(dayInputText);
    
    monthInputText = TextInputLayer::createWithSize(Size(330,197), INPUT_IS_MONTH);
    monthInputText->setPositionY(dayInputText->getPositionY());
    monthInputText->setPositionX(origin.x+visibleSize.width/2 - 225);
    monthInputText->setDelegate(this);
    this->addChild(monthInputText);
    
    yearInputText = TextInputLayer::createWithSize(Size(450,197), INPUT_IS_YEAR);
    yearInputText->setPositionY(dayInputText->getPositionY());
    yearInputText->setPositionX(origin.x+visibleSize.width/2 + 155);
    yearInputText->setDelegate(this);
    this->addChild(yearInputText);
}

void ChildAccountScene::addLabelToScene()
{
    profileNameTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_NAME_LABEL));
    profileNameTitle->setPositionY(childNameInputText->getPositionY()+(childNameInputText->getContentSize().height) + (profileNameTitle->getContentSize().height*.6));
    this->addChild(profileNameTitle);
    
    profileDOBSubTitle = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_DOB_SUB_LABEL));
    profileDOBSubTitle->setPositionY(dayInputText->getPositionY()+(dayInputText->getContentSize().height) + (profileDOBSubTitle->getContentSize().height*1.5));
    this->addChild(profileDOBSubTitle);
    
    profileDOBTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_DOB_LABEL));
    profileDOBTitle->setPositionY(profileDOBSubTitle->getPositionY()+ (profileDOBTitle->getContentSize().height*.5));
    this->addChild(profileDOBTitle);
    
    oomeesTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_OOMEE_LABEL));
    oomeesTitle->setPosition(profileNameTitle->getPosition());
    oomeesTitle->setVisible(false);
    this->addChild(oomeesTitle);
}


void ChildAccountScene::addButtonsScene()
{
    nextButton = ElectricDreamsButton::createButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_CONTINUE));
    nextButton->setCenterPosition(Vec2(visibleSize.width*.7+origin.x, dayInputText->getPositionY()-nextButton->getContentSize().height*1.2));
    nextButton->setDelegate(this);
    nextButton->setMixPanelButtonName("childAccountSceneContinueButton");
    nextButton->setVisible(false);
    
    nextButtonPlaceholder = ElectricDreamsButton::createPlaceHolderButton(nextButton->getContentSize().width);
    nextButtonPlaceholder->setCenterPosition(nextButton->getCenterPosition());
    this->addChild(nextButtonPlaceholder);
    this->addChild(nextButton);
    
    cancelButton = ElectricDreamsButton::createOutlineButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_CANCEL));
    cancelButton->setCenterPosition(Vec2(visibleSize.width*.3+origin.x, nextButton->getCenterPosition().y));
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("childAccountSceneCancelButton");
    this->addChild(cancelButton);
        
    if(FlowDataSingleton::getInstance()->isSignupFlow())
        cancelButton->setVisible(false);
    
    submitButton = ElectricDreamsButton::createButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_DONE));
    submitButton->setCenterPosition(Vec2(nextButton->getCenterPosition().x,origin.y + submitButton->getContentSize().height));
    submitButton->setDelegate(this);
    submitButton->setMixPanelButtonName("childAccountSceneDoneButton");
    submitButton->setVisible(false);
    
    submitButtonPlaceholder = ElectricDreamsButton::createPlaceHolderButton(submitButton->getContentSize().width);
    submitButtonPlaceholder->setCenterPosition(submitButton->getCenterPosition());
    submitButtonPlaceholder->setVisible(false);
    this->addChild(submitButtonPlaceholder);
    this->addChild(submitButton);
    
    backButton = ElectricDreamsButton::createOutlineButtonWithText("Back");
    backButton->setCenterPosition(Vec2(cancelButton->getCenterPosition().x, submitButton->getCenterPosition().y));
    backButton->setDelegate(this);
    backButton->setMixPanelButtonName("childAccountSceneBackButton");
    backButton->setVisible(false);
    this->addChild(backButton);
}

//------------CHANGE SCREEN VISUALS ON BUTTON PRESS----------------------

void ChildAccountScene::clearElementsOnScreen()
{
    AudioMixer::getInstance()->stopOomeeEffect();
    
    //TextBoxes
    childNameInputText->setEditboxVisibility(false);
    dayInputText->setEditboxVisibility(false);
    monthInputText->setEditboxVisibility(false);
    yearInputText->setEditboxVisibility(false);
    
    //Labels
    profileNameTitle->setVisible(false);
    profileDOBTitle->setVisible(false);
    profileDOBSubTitle->setVisible(false);
    oomeesTitle->setVisible(false);
    
    //Buttons
    cancelButton->setVisible(false);
    nextButton->setVisible(false);
    nextButtonPlaceholder->setVisible(false);
    backButton->setVisible(false);
    submitButton->setVisible(false);
    submitButtonPlaceholder->setVisible(false);
    
    hideOomees();
}

void ChildAccountScene::changeElementsToTextInputScreen()
{
    clearElementsOnScreen();
    
    childNameInputText->setEditboxVisibility(true);
    dayInputText->setEditboxVisibility(true);
    monthInputText->setEditboxVisibility(true);
    yearInputText->setEditboxVisibility(true);
    
    profileNameTitle->setVisible(true);
    profileDOBTitle->setVisible(true);
    profileDOBSubTitle->setVisible(true);
    
    if(!FlowDataSingleton::getInstance()->isSignupFlow())
        cancelButton->setVisible(true);
    
    nextButton->setVisible(DOBisDate() && childNameInputText->inputIsValid());
    nextButtonPlaceholder->setVisible(true);
    
    childNameInputText->focusAndShowKeyboard();
}

void ChildAccountScene::changeElementsToOomeeScreen()
{
    clearElementsOnScreen();
    
    oomeesTitle->setVisible(true);
    
    backButton->setVisible(true);
    submitButtonPlaceholder->setVisible(true);
    
    showOomees();
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
    
    OomeeButtons.at(0)->setCenterPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height*.47));
    OomeeButtons.at(1)->setCenterPosition(Vec2(origin.x + visibleSize.width*.3, origin.y + visibleSize.height*.24));
    OomeeButtons.at(2)->setCenterPosition(Vec2(origin.x + visibleSize.width*.7, origin.y + visibleSize.height*.24));
    OomeeButtons.at(3)->setCenterPosition(Vec2(origin.x + visibleSize.width*.3, origin.y + visibleSize.height*.70));
    OomeeButtons.at(4)->setCenterPosition(Vec2(origin.x + visibleSize.width*.7, origin.y + visibleSize.height*.70));
    
    ModalMessages::getInstance()->stopLoading();
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
            submitButton->setVisible(true);
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
    if(FlowDataSingleton::getInstance()->isSignupFlow() && ParentDataProvider::getInstance()->getAmountOfAvailableChildren() !=0)
        backEndCaller->updateChild(ParentDataProvider::getInstance()->getIDForAvailableChildren(0), profileName, gender, DOB, this->selectedOomeeNo);
    else
        backEndCaller->registerChild(profileName, gender, DOB, this->selectedOomeeNo);
}

bool ChildAccountScene::DOBisDate()
{
    int day = std::atoi(dayInputText->getText().c_str());
    int month = std::atoi(monthInputText->getText().c_str());
    int year = std::atoi(yearInputText->getText().c_str());
    
    return isDate(day, month, year);
}

//----------------------- Delegate Functions ----------------------------

void ChildAccountScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    nextButton->setVisible(DOBisDate() && childNameInputText->inputIsValid());
    
    if(inputLayer==dayInputText && dayInputText->getText().length() == 2)
        monthInputText->focusAndShowKeyboard();
    else if(inputLayer==monthInputText && (monthInputText->getText().length()==2 || std::atoi(monthInputText->getText().c_str()) >2))
        yearInputText->focusAndShowKeyboard();
}

void ChildAccountScene::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(inputLayer==childNameInputText)
        dayInputText->focusAndShowKeyboard();
    else if(inputLayer == dayInputText)
        monthInputText->focusAndShowKeyboard();
    else if(inputLayer == monthInputText)
        yearInputText->focusAndShowKeyboard();
    else if(inputLayer == yearInputText)
    {
        if(DOBisDate() && childNameInputText->inputIsValid())
            changeElementsToOomeeScreen();
    }
}

void ChildAccountScene::buttonPressed(ElectricDreamsButton* button)
{
    if(button == cancelButton)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
    }
    else if(button == nextButton)
    {
        if(childNameExists(childNameInputText->getText()))
        {
            MessageBox::createWith(ERROR_CODE_NAME_EXISTS, childNameInputText, this);
            AnalyticsSingleton::getInstance()->childProdileNameErrorEvent();
        }
        else
            changeElementsToOomeeScreen();
    }
    else if(button == backButton)
        changeElementsToTextInputScreen();
    else if(button == submitButton)
        registerChildAccount();
    else
        selectOomee(button->getTag());
}

void ChildAccountScene::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    childNameInputText->focusAndShowKeyboard();
    childNameInputText->setVisible(true);
    dayInputText->setVisible(true);
    monthInputText->setVisible(true);
    yearInputText->setVisible(true);
}
