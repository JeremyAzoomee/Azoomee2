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
    sceneTitle = createLabelFlowMainTitle(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_TITLE_LABEL));
    this->addChild(sceneTitle);
}

void ChildAccountScene::addTextboxScene()
{
    childNameInputText = TextInputLayer::createWithSize(Size(visibleSize.width*.80,197), INPUT_IS_CHILD_NAME);
    childNameInputText->setPositionY(sceneTitle->getPositionY()-childNameInputText->getContentSize().height*2.25);
    childNameInputText->setDelegate(this);
    this->addChild(childNameInputText);
    
    dayInputText = TextInputLayer::createWithSize(Size(330,197), INPUT_IS_DAY);
    dayInputText->setPositionY(childNameInputText->getPositionY() -dayInputText->getContentSize().height*2.1 );
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
    
    profileDOBTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_DOB_LABEL));
    profileDOBTitle->setPositionY(dayInputText->getPositionY()+(dayInputText->getContentSize().height) + (profileDOBTitle->getContentSize().height*.6));
    this->addChild(profileDOBTitle);
    
    profileDOBSubTitle = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_DOB_SUB_LABEL));
    profileDOBSubTitle->setPositionY(dayInputText->getPositionY()- (profileDOBSubTitle->getContentSize().height*.3));
    this->addChild(profileDOBSubTitle);
    
    oomeesTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_OOMEE_LABEL));
    oomeesTitle->setPosition(profileNameTitle->getPosition());
    oomeesTitle->setVisible(false);
    this->addChild(oomeesTitle);
}


void ChildAccountScene::addButtonsScene()
{
    
    nextButton = ElectricDreamsButton::createButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_CONTINUE));
    nextButton->setCenterPosition(Vec2(visibleSize.width*.7+origin.x, dayInputText->getPositionY()-nextButton->getContentSize().height*1.5));
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
        
    if(FlowDataSingleton::getInstance()->isSignupFlow() || FlowDataSingleton::getInstance()->isSignupNewProfileFlow())
    {
        cancelButton->setVisible(false);
        nextButton->setCenterPosition(Vec2(visibleSize.width/2+origin.x, dayInputText->getPositionY()-nextButton->getContentSize().height*1.5));
        nextButtonPlaceholder->setCenterPosition(nextButton->getCenterPosition());
        
    }
    submitButton = ElectricDreamsButton::createButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_DONE));
    submitButton->setCenterPosition(Vec2(visibleSize.width*.7+origin.x,origin.y + submitButton->getContentSize().height));
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
    backButton->setMixPanelButtonName("childAccountSceneBackFromOomeesButton");
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
    
    if(!(FlowDataSingleton::getInstance()->isSignupFlow() || FlowDataSingleton::getInstance()->isSignupNewProfileFlow()))
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
    
    std::vector<cocos2d::Vec2> oomeePositions = {Vec2(0.5, 0.47), Vec2(0.3, 0.29), Vec2(0.7, 0.29), Vec2(0.3, 0.65), Vec2(0.7, 0.65)};
    
    for(int i=0;i< NO_OF_OOMEES;i++)
    {
        auto oomeeButton = ElectricDreamsButton::createOomeeAsButton(i);
        oomeeButton->setDelegate(this);
        oomeeButton->setVisible(false);
        oomeeButton->setScale(1.3);
        oomeeButton->setCenterPosition(Vec2(origin.x + visibleSize.width * oomeePositions.at(i).x, origin.y + visibleSize.height * oomeePositions.at(i).y));
        this->addChild(oomeeButton);
        OomeeButtons.push_back(oomeeButton);
    }
    
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
    AnalyticsSingleton::getInstance()->childProfileOomeeEvent(oomeeNumber);
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
void ChildAccountScene::shouldChangeElementsToOomeeScreen()
{
    if(childNameExists(childNameInputText->getText()))
    {
        MessageBox::createWith(ERROR_CODE_NAME_EXISTS, childNameInputText, this);
        AnalyticsSingleton::getInstance()->childProdileNameErrorEvent();
    }
    else if(DOBisDateInFuture())
    {
        MessageBox::createWith(422, nullptr);
    }
    else if(DOBisDate() && childNameInputText->inputIsValid())
        changeElementsToOomeeScreen();
}

void ChildAccountScene::registerChildAccount()
{
    std::string profileName = childNameInputText->getText();
    
    int day = std::atoi(dayInputText->getText().c_str());
    int month = std::atoi(monthInputText->getText().c_str());
    int year = std::atoi(yearInputText->getText().c_str());
    
    std::string DOB = StringUtils::format("%04d-%02d-%02d",year,month,day);
    std::string gender = "MALE";
    
    auto backEndCaller = BackEndCaller::getInstance();
    if((FlowDataSingleton::getInstance()->isSignupFlow() || FlowDataSingleton::getInstance()->isSignupNewProfileFlow()) && ParentDataProvider::getInstance()->getAmountOfAvailableChildren() !=0)
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

bool ChildAccountScene::DOBisDateInFuture()
{
    return isDateInFuture(StringUtils::format("%s-%s-%s",yearInputText->getText().c_str(),monthInputText->getText().c_str(),dayInputText->getText().c_str()));
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
        shouldChangeElementsToOomeeScreen();
}

void ChildAccountScene::buttonPressed(ElectricDreamsButton* button)
{
    if(button == cancelButton)
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
    else if(button == nextButton)
        shouldChangeElementsToOomeeScreen();
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
