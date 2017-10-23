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
#include <AzoomeeCommon/Utils/StringFunctions.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN


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
    
    addBackgroundandDecoration();
    addProgressIndicator();
    AddTitleToScene();
    addTextboxScene();
    addLabelToScene();
    addButtonsScene();
    setNewLayout();
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

void ChildAccountScene::addBackgroundandDecoration()
{
    Layer* background = createTopGradientAndParticles();
    background->setPosition(origin.x,origin.y);
    this->addChild(background);
}

void ChildAccountScene::addProgressIndicator()
{
    if(FlowDataSingleton::getInstance()->isSignupFlow())
    {
        _progressIndicatior = Sprite::create("res/decoration/progress2.png");
        _progressIndicatior->setPosition(origin.x+visibleSize.width/2,origin.y+visibleSize.height - _progressIndicatior->getContentSize().height*1.5);
        this->addChild(_progressIndicatior);
    }
}

void ChildAccountScene::AddTitleToScene()
{
    float progressIndicatorHeight = 0.0f;
    
    if(_progressIndicatior != nullptr)
    {
        progressIndicatorHeight = _progressIndicatior->getContentSize().height;
    }
    
    if(FlowDataSingleton::getInstance()->isSignupFlow() || FlowDataSingleton::getInstance()->isSignupNewProfileFlow())
    {
        sceneTitle = createLabelFlowMainTitle(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_MAIN_TITLE_SIGNUP_LABEL));
        sceneTitle->setPositionY(_progressIndicatior->getPositionY()-_progressIndicatior->getContentSize().height-sceneTitle->getContentSize().height/2 - progressIndicatorHeight);
    }
    else
    {
        sceneTitle = createLabelFlowMainTitle(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_MAIN_TITLE_ADD_CHILD_LABEL));
    }
    this->addChild(sceneTitle);
}

void ChildAccountScene::addTextboxScene()
{
    childNameInputText = TextInputLayer::createWithSize(Size(visibleSize.width*.80,160), INPUT_IS_CHILD_NAME);
    childNameInputText->setPositionY(sceneTitle->getPositionY()-childNameInputText->getContentSize().height*2.4);
    childNameInputText->setDelegate(this);
    this->addChild(childNameInputText);
    
    dayInputText = TextInputLayer::createWithSize(Size(330,160), INPUT_IS_DAY);
    dayInputText->setPositionX(origin.x+visibleSize.width/2 - 275-dayInputText->getContentSize().width);
    dayInputText->setDelegate(this);
    this->addChild(dayInputText);
    
    monthInputText = TextInputLayer::createWithSize(Size(330,160), INPUT_IS_MONTH);
    monthInputText->setPositionX(origin.x+visibleSize.width/2 - 225);
    monthInputText->setDelegate(this);
    this->addChild(monthInputText);
    
    yearInputText = TextInputLayer::createWithSize(Size(450, 160), INPUT_IS_YEAR);
    yearInputText->setPositionX(origin.x+visibleSize.width/2 + 155);
    yearInputText->setDelegate(this);
    this->addChild(yearInputText);
}

void ChildAccountScene::addLabelToScene()
{
    profileNameTitle = createLabelFlowSubTitle(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_NAME_LABEL));
    profileNameTitle->setPositionY(childNameInputText->getPositionY()+(childNameInputText->getContentSize().height) + (profileNameTitle->getContentSize().height*.6));
    this->addChild(profileNameTitle);
    
    profileNameError = createLabelBodyCentred("Profile (nick)name should be unique.",Style::Color::watermelon);
    profileNameError->setLineSpacing(20);
    profileNameError->setVisible(false);
    this->addChild(profileNameError);
    
    profileDOBTitle = createLabelFlowSubTitle(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_DOB_LABEL));
    this->addChild(profileDOBTitle);
    
    profileDOBError = createLabelBodyCentred("Date is incorrect.",Style::Color::watermelon);
    profileDOBError->setLineSpacing(20);
    profileDOBError->setVisible(false);
    this->addChild(profileDOBError);
    
    profileDOBSubTitle = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_DOB_SUB_LABEL),Style::Color::black);
    profileDOBSubTitle->setLineSpacing(20);
    this->addChild(profileDOBSubTitle);
    
    oomeesTitle = createLabelFlowSubTitle(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_OOMEE_LABEL));
    oomeesTitle->setPosition(profileNameTitle->getPosition());
    oomeesTitle->setVisible(false);
    this->addChild(oomeesTitle);
}


void ChildAccountScene::addButtonsScene()
{
    
    nextButton = ElectricDreamsButton::createGreenButtonWithWidth(StringMgr::getInstance()->getStringForKey(BUTTON_CONTINUE),visibleSize.width/3);
    nextButton->setOpacity(80);
    nextButton->setCenterPosition(Vec2(visibleSize.width*.75+origin.x, dayInputText->getPositionY()-nextButton->getContentSize().height*1.9));
    nextButton->setMixPanelButtonName("childAccountSceneContinueButton");
    this->addChild(nextButton);
    
    cancelButton = ElectricDreamsButton::createTextAsButtonWithColor(StringMgr::getInstance()->getStringForKey(BUTTON_CANCEL), 75, true, Color3B::BLACK);
    cancelButton->setDelegate(this);
    cancelButton->setMixPanelButtonName("childAccountSceneCancelButton");
    this->addChild(cancelButton);
        
    if(FlowDataSingleton::getInstance()->isSignupFlow() || FlowDataSingleton::getInstance()->isSignupNewProfileFlow())
    {
        cancelButton->setVisible(false);
        nextButton->setCenterPosition(Vec2(visibleSize.width/2+origin.x, dayInputText->getPositionY()-nextButton->getContentSize().height*1.9));
    }
    
    submitButton = ElectricDreamsButton::createGreenButtonWithWidth(StringMgr::getInstance()->getStringForKey(BUTTON_CONTINUE),visibleSize.width/3);
    submitButton->setCenterPosition(Vec2(visibleSize.width*.7+origin.x,origin.y + submitButton->getContentSize().height));
    submitButton->setOpacity(80);
    submitButton->setMixPanelButtonName("childAccountSceneDoneButton");
    submitButton->setVisible(false);
    this->addChild(submitButton);
    
    backButton = ElectricDreamsButton::createTextAsButtonWithColor(StringMgr::getInstance()->getStringForKey(BUTTON_BACK), 75, true, Color3B::BLACK);
    backButton->setCenterPosition(Vec2(visibleSize.width*.25+origin.x, submitButton->getCenterPosition().y));
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
    backButton->setVisible(false);
    submitButton->setVisible(false);
    submitButton->setOpacity(80);
    submitButton->setDelegate(nullptr);
    
    hideOomees();
}

void ChildAccountScene::changeElementsToTextInputScreen()
{
    clearElementsOnScreen();
    
    if(_progressIndicatior != nullptr)
    {
        _progressIndicatior->setTexture("res/decoration/progress2.png");
    }

    if(FlowDataSingleton::getInstance()->isSignupFlow() || FlowDataSingleton::getInstance()->isSignupNewProfileFlow())
        sceneTitle->setString(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_MAIN_TITLE_SIGNUP_LABEL));
    else
        sceneTitle->setString(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_MAIN_TITLE_ADD_CHILD_LABEL));
    
    childNameInputText->setEditboxVisibility(true);
    dayInputText->setEditboxVisibility(true);
    monthInputText->setEditboxVisibility(true);
    yearInputText->setEditboxVisibility(true);
    
    profileNameTitle->setVisible(true);
    profileDOBTitle->setVisible(true);
    profileDOBSubTitle->setVisible(true);
    
    if(!(FlowDataSingleton::getInstance()->isSignupFlow() || FlowDataSingleton::getInstance()->isSignupNewProfileFlow()))
        cancelButton->setVisible(true);
    
    nextButton->setVisible(true);
    
    childNameInputText->focusAndShowKeyboard();
}

void ChildAccountScene::changeElementsToOomeeScreen()
{
    clearElementsOnScreen();
    
    if(_progressIndicatior != nullptr)
    {
        _progressIndicatior->setTexture("res/decoration/progress3.png");
    }
    
    sceneTitle->setString(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_SELECT_OOMEE_TITLE_LABEL));
    
    oomeesTitle->setVisible(true);
    
    submitButton->setVisible(true);
    backButton->setVisible(true);
    
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
            submitButton->setOpacity(255);
            submitButton->setDelegate(this);
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
    if(!DOBisDate() || !childNameInputText->inputIsValid())
        childNameInputText->focusAndShowKeyboard();
    else if(childNameExists(trim(childNameInputText->getText())))
    {
        MessageBox::createWith(ERROR_CODE_NAME_EXISTS, childNameInputText, this);
        AnalyticsSingleton::getInstance()->childProdileNameErrorEvent();
    }
    else if(DOBisDateInFuture())
    {
        MessageBox::createWith(422, nullptr);
    }
    else
        changeElementsToOomeeScreen();
}

void ChildAccountScene::registerChildAccount()
{
    std::string profileName = trim(childNameInputText->getText());
    
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

void ChildAccountScene::setNewLayout()
{
    float additionYForErrorText = 0.0f;
    
    if(profileNameError->isVisible())
    {
        additionYForErrorText = profileNameError->getContentSize().height * 1.75f;
        profileNameError->setPositionY(childNameInputText->getPositionY()-profileNameError->getContentSize().height/2);
    }
    
    dayInputText->setPositionY(childNameInputText->getPositionY() -dayInputText->getContentSize().height*2.1 - additionYForErrorText);
    monthInputText->setPositionY(dayInputText->getPositionY());
    yearInputText->setPositionY(dayInputText->getPositionY());
    
    profileDOBTitle->setPositionY(dayInputText->getPositionY()+(dayInputText->getContentSize().height) + (profileDOBTitle->getContentSize().height*.6));
    
    if(profileDOBError->isVisible())
    {
        additionYForErrorText = profileDOBError->getContentSize().height * 1.75f;
        profileDOBError->setPositionY(dayInputText->getPositionY()-profileDOBError->getContentSize().height/2);
    }
    else
    {
        additionYForErrorText = 0.0f;
    }
    
    profileDOBSubTitle->setPositionY(dayInputText->getPositionY()- (profileDOBSubTitle->getContentSize().height*.3) - additionYForErrorText);
    
    nextButton->setCenterPosition(Vec2(nextButton->getCenterPosition().x, dayInputText->getPositionY()-nextButton->getContentSize().height*1.9 - additionYForErrorText));
    cancelButton->setCenterPosition(Vec2(visibleSize.width*.25+origin.x, nextButton->getCenterPosition().y));
}

//----------------------- Delegate Functions ----------------------------

void ChildAccountScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    if(DOBisDate() && childNameInputText->inputIsValid())
    {
        nextButton->setOpacity(255);
        nextButton->setDelegate(this);
    }
    else
    {
        nextButton->setOpacity(80);
        nextButton->setDelegate(nullptr);
    }
    
    if(inputLayer==dayInputText && dayInputText->getText().length() == 2)
    {
        monthInputText->focusAndShowKeyboard();
        profileDOBError->setVisible(false);
        dayInputText->setEditboxHasError(false);
        monthInputText->setEditboxHasError(false);
        yearInputText->setEditboxHasError(false);
        setNewLayout();
    }
    else if(inputLayer==monthInputText && (monthInputText->getText().length()==2 || std::atoi(monthInputText->getText().c_str()) >2))
    {
        yearInputText->focusAndShowKeyboard();
        profileDOBError->setVisible(false);
        dayInputText->setEditboxHasError(false);
        monthInputText->setEditboxHasError(false);
        yearInputText->setEditboxHasError(false);
        setNewLayout();
    }
}

void ChildAccountScene::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(inputLayer==childNameInputText)
    {
        dayInputText->focusAndShowKeyboard();
        
        if(!childNameInputText->inputIsValid() || childNameExists(trim(childNameInputText->getText())))
        {
            profileNameError->setVisible(true);
            childNameInputText->setEditboxHasError();
            setNewLayout();
        }
    }
    else if(inputLayer == dayInputText)
    {
        monthInputText->focusAndShowKeyboard();
        
        profileDOBError->setVisible(false);
        dayInputText->setEditboxHasError(false);
        monthInputText->setEditboxHasError(false);
        yearInputText->setEditboxHasError(false);
        setNewLayout();
    }
    else if(inputLayer == monthInputText)
    {
        yearInputText->focusAndShowKeyboard();
    }
    else if(inputLayer == yearInputText)
    {
        shouldChangeElementsToOomeeScreen();
        if(!DOBisDate())
        {
            profileDOBError->setVisible(true);
            dayInputText->setEditboxHasError();
            monthInputText->setEditboxHasError();
            yearInputText->setEditboxHasError();
            setNewLayout();
        }
    }
}

void ChildAccountScene::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    if(inputLayer == childNameInputText)
    {
        profileNameError->setVisible(false);
        childNameInputText->setEditboxHasError(false);
        setNewLayout();
    }
    else
    {
        profileDOBError->setVisible(false);
        dayInputText->setEditboxHasError(false);
        monthInputText->setEditboxHasError(false);
        yearInputText->setEditboxHasError(false);
        setNewLayout();
    }
}

void ChildAccountScene::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    if((!childNameInputText->inputIsValid() || childNameExists(trim(childNameInputText->getText()))) && inputLayer == childNameInputText)
    {
        profileNameError->setVisible(true);
        childNameInputText->setEditboxHasError();
        setNewLayout();
    }
    else if(!DOBisDate())
    {
        profileDOBError->setVisible(true);
        dayInputText->setEditboxHasError();
        monthInputText->setEditboxHasError();
        yearInputText->setEditboxHasError();
        setNewLayout();
    }
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

NS_AZOOMEE_END
