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
#include <AzoomeeCommon/UI/PrivacyLayer.h>

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
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    
    ModalMessages::getInstance()->startLoading();
    
    addTitleToScene();
    addTextboxScene();
    addLabelToScene();
    addButtonsScene();
    addPrivacyButton();
    addOomeesToScene();

    
    Node::onEnter();
}

void ChildAccountScene::onEnterTransitionDidFinish()
{
    if(FlowDataSingleton::getInstance()->hasError())
    {
        _childNameInputText->setVisible(false);
        _dayInputText->setVisible(false);
        _monthInputText->setVisible(false);
        _yearInputText->setVisible(false);
        MessageBox::createWith(FlowDataSingleton::getInstance()->getErrorCode(), this);
    }
    else
        _childNameInputText->focusAndShowKeyboard();
}

//----------------- SCENE SETUP ---------------

void ChildAccountScene::addTitleToScene()
{
    if(FlowDataSingleton::getInstance()->isSignupFlow() || FlowDataSingleton::getInstance()->isSignupNewProfileFlow())
    {
        _sceneTitle = createLabelFlowMainTitle(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_MAIN_TITLE_SIGNUP_LABEL));
    }
    else
    {
        _sceneTitle = createLabelFlowMainTitle(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_MAIN_TITLE_ADD_CHILD_LABEL));
    }
    this->addChild(_sceneTitle);
}

void ChildAccountScene::addTextboxScene()
{
    _childNameInputText = TextInputLayer::createWithSize(Size(_visibleSize.width*0.8f,197.0f), INPUT_IS_CHILD_NAME);
    _childNameInputText->setPositionY(_sceneTitle->getPositionY()-_childNameInputText->getContentSize().height*2.4f);
    _childNameInputText->setDelegate(this);
    this->addChild(_childNameInputText);
    
    _dayInputText = TextInputLayer::createWithSize(Size(330.0f,197.0f), INPUT_IS_DAY);
    _dayInputText->setPositionY(_childNameInputText->getPositionY() -_dayInputText->getContentSize().height*2.0f );
    _dayInputText->setPositionX(_origin.x+_visibleSize.width/2.0f - 275.0f -_dayInputText->getContentSize().width);
    _dayInputText->setDelegate(this);
    this->addChild(_dayInputText);
    
    _monthInputText = TextInputLayer::createWithSize(Size(330.0f,197.0f), INPUT_IS_MONTH);
    _monthInputText->setPositionY(_dayInputText->getPositionY());
    _monthInputText->setPositionX(_origin.x+_visibleSize.width/2.0f - 225.0f);
    _monthInputText->setDelegate(this);
    this->addChild(_monthInputText);
    
    _yearInputText = TextInputLayer::createWithSize(Size(450.0f,197.0f), INPUT_IS_YEAR);
    _yearInputText->setPositionY(_dayInputText->getPositionY());
    _yearInputText->setPositionX(_origin.x+_visibleSize.width/2.0f + 155.0f);
    _yearInputText->setDelegate(this);
    this->addChild(_yearInputText);
}

void ChildAccountScene::addLabelToScene()
{
    _profileNameTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_NAME_LABEL));
    _profileNameTitle->setPositionY(_childNameInputText->getPositionY()+(_childNameInputText->getContentSize().height) + (_profileNameTitle->getContentSize().height*0.6f));
    this->addChild(_profileNameTitle);
    
    _profileDOBTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_DOB_LABEL));
    _profileDOBTitle->setPositionY(_dayInputText->getPositionY()+(_dayInputText->getContentSize().height) + (_profileDOBTitle->getContentSize().height*0.6f));
    this->addChild(_profileDOBTitle);
    
    _profileDOBSubTitle = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_DOB_SUB_LABEL));
    _profileDOBSubTitle->setPositionY(_dayInputText->getPositionY()- (_profileDOBSubTitle->getContentSize().height*.3));
    _profileDOBSubTitle->setLineSpacing(20.0f);
    this->addChild(_profileDOBSubTitle);
    
    _oomeesTitle = createLabelHeader(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_REQUEST_OOMEE_LABEL));
    _oomeesTitle->setPosition(_profileNameTitle->getPosition());
    _oomeesTitle->setVisible(false);
    this->addChild(_oomeesTitle);
}

void ChildAccountScene::addButtonsScene()
{
    
    _nextButton = ElectricDreamsButton::createButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_CONTINUE));
    _nextButton->setCenterPosition(Vec2(_visibleSize.width*0.7f +_origin.x, _dayInputText->getPositionY()-_nextButton->getContentSize().height* 1.7f));
    _nextButton->setDelegate(this);
    _nextButton->setMixPanelButtonName("childAccountSceneContinueButton");
    _nextButton->setVisible(false);
    
    _nextButtonPlaceholder = ElectricDreamsButton::createPlaceHolderButton(_nextButton->getContentSize().width);
    _nextButtonPlaceholder->setCenterPosition(_nextButton->getCenterPosition());
    this->addChild(_nextButtonPlaceholder);
    this->addChild(_nextButton);
    
    _cancelButton = ElectricDreamsButton::createOutlineButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_CANCEL));
    _cancelButton->setCenterPosition(Vec2(_visibleSize.width*0.3f +_origin.x, _nextButton->getCenterPosition().y));
    _cancelButton->setDelegate(this);
    _cancelButton->setMixPanelButtonName("childAccountSceneCancelButton");
    this->addChild(_cancelButton);
        
    if(FlowDataSingleton::getInstance()->isSignupFlow() || FlowDataSingleton::getInstance()->isSignupNewProfileFlow())
    {
        _cancelButton->setVisible(false);
        _nextButton->setCenterPosition(Vec2(_visibleSize.width/2.0f+_origin.x, _dayInputText->getPositionY()-_nextButton->getContentSize().height* 1.9f));
        _nextButtonPlaceholder->setCenterPosition(_nextButton->getCenterPosition());
        
    }
    _submitButton = ElectricDreamsButton::createButtonWithText(StringMgr::getInstance()->getStringForKey(BUTTON_CONTINUE));
    _submitButton->setCenterPosition(Vec2(_visibleSize.width*0.7f+_origin.x,_origin.y + _submitButton->getContentSize().height));
    _submitButton->setDelegate(this);
    _submitButton->setMixPanelButtonName("childAccountSceneDoneButton");
    _submitButton->setVisible(false);
    
    _submitButtonPlaceholder = ElectricDreamsButton::createPlaceHolderButton(_submitButton->getContentSize().width);
    _submitButtonPlaceholder->setCenterPosition(_submitButton->getCenterPosition());
    _submitButtonPlaceholder->setVisible(false);
    this->addChild(_submitButtonPlaceholder);
    this->addChild(_submitButton);
    
    _backButton = ElectricDreamsButton::createOutlineButtonWithText("Back");
    _backButton->setCenterPosition(Vec2(_cancelButton->getCenterPosition().x, _submitButton->getCenterPosition().y));
    _backButton->setDelegate(this);
    _backButton->setMixPanelButtonName("childAccountSceneBackFromOomeesButton");
    _backButton->setVisible(false);
    this->addChild(_backButton);
}

void ChildAccountScene::addPrivacyButton()
{
    _privacyLayer = PrivacyLayer::create();
    _privacyLayer->setCenterPosition(Vec2(_origin.x + _privacyLayer->getContentSize().height + _privacyLayer->getContentSize().width/2.0f,_nextButtonPlaceholder->getPositionY() - _privacyLayer->getContentSize().height*2.0f));
    this->addChild(_privacyLayer);
}

//------------CHANGE SCREEN VISUALS ON BUTTON PRESS----------------------

void ChildAccountScene::clearElementsOnScreen()
{
    AudioMixer::getInstance()->stopOomeeEffect();
    
    //TextBoxes
    _childNameInputText->setEditboxVisibility(false);
    _dayInputText->setEditboxVisibility(false);
    _monthInputText->setEditboxVisibility(false);
    _yearInputText->setEditboxVisibility(false);
    
    //Labels
    _profileNameTitle->setVisible(false);
    _profileDOBTitle->setVisible(false);
    _profileDOBSubTitle->setVisible(false);
    _oomeesTitle->setVisible(false);
    
    //Buttons
    _cancelButton->setVisible(false);
    _nextButton->setVisible(false);
    _nextButtonPlaceholder->setVisible(false);
    _backButton->setVisible(false);
    _submitButton->setVisible(false);
    _submitButtonPlaceholder->setVisible(false);
    _privacyLayer->setVisible(false);
    
    hideOomees();
}

void ChildAccountScene::changeElementsToTextInputScreen()
{
    clearElementsOnScreen();

    if(FlowDataSingleton::getInstance()->isSignupFlow() || FlowDataSingleton::getInstance()->isSignupNewProfileFlow())
    {
        _sceneTitle->setString(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_MAIN_TITLE_SIGNUP_LABEL));
    }
    else
    {
        _sceneTitle->setString(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_MAIN_TITLE_ADD_CHILD_LABEL));
    }
    
    _childNameInputText->setEditboxVisibility(true);
    _dayInputText->setEditboxVisibility(true);
    _monthInputText->setEditboxVisibility(true);
    _yearInputText->setEditboxVisibility(true);
    
    _profileNameTitle->setVisible(true);
    _profileDOBTitle->setVisible(true);
    _profileDOBSubTitle->setVisible(true);
    
    if(!(FlowDataSingleton::getInstance()->isSignupFlow() || FlowDataSingleton::getInstance()->isSignupNewProfileFlow()))
    {
        _cancelButton->setVisible(true);
    }
    
    _nextButton->setVisible(DOBisDate() && _childNameInputText->inputIsValid());
    _nextButtonPlaceholder->setVisible(true);
    _privacyLayer->setVisible(true);
    
    _childNameInputText->focusAndShowKeyboard();
}

void ChildAccountScene::changeElementsToOomeeScreen()
{
    clearElementsOnScreen();
    
    _sceneTitle->setString(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSCENE_SELECT_OOMEE_TITLE_LABEL));
    
    _oomeesTitle->setVisible(true);
    
    _backButton->setVisible(true);
    _submitButtonPlaceholder->setVisible(true);
    
    showOomees();
}

//----------------OOMEE SCREEN FUNCTIONS---------------
void ChildAccountScene::addOomeesToScene()
{
    _oomeeGlow = createGlow();
    _oomeeGlow->setVisible(false);
    _oomeeGlow->setScale(.6);
    this->addChild(_oomeeGlow);
    
    std::vector<cocos2d::Vec2> oomeePositions = {
        Vec2(0.5f, 0.47f),
        Vec2(0.3f, 0.29f),
        Vec2(0.7f, 0.29f),
        Vec2(0.3f, 0.65f),
        Vec2(0.7f, 0.65f)
    };
    
    for(int i=0;i< NO_OF_OOMEES;i++)
    {
        auto oomeeButton = ElectricDreamsButton::createOomeeAsButton(i);
        oomeeButton->setDelegate(this);
        oomeeButton->setVisible(false);
        oomeeButton->setScale(1.3f);
        oomeeButton->setCenterPosition(Vec2(_origin.x + _visibleSize.width * oomeePositions.at(i).x, _origin.y + _visibleSize.height * oomeePositions.at(i).y));
        this->addChild(oomeeButton);
        _oomeeButtons.push_back(oomeeButton);
    }
    
    ModalMessages::getInstance()->stopLoading();
}

void ChildAccountScene::hideOomees()
{
    for(int i=0;i< _oomeeButtons.size();i++)
    {
        _oomeeButtons.at(i)->setVisible(false);
        _oomeeButtons.at(i)->setScale(1.3f);
    }
    
    _oomeeGlow->setVisible(false);
}

void ChildAccountScene::showOomees()
{
    for(int i=0;i< _oomeeButtons.size();i++)
    {
        _oomeeButtons.at(i)->setVisible(true);
        _oomeeButtons.at(i)->playOomeeAnimationNoSound("Build_Simple_Wave");
    }
}

void ChildAccountScene::selectOomee(int oomeeNumber)
{
    AnalyticsSingleton::getInstance()->childProfileOomeeEvent(oomeeNumber);
    _selectedOomeeNo = oomeeNumber;
    
    for(int i=0;i< _oomeeButtons.size();i++)
    {
        if(_oomeeButtons.at(i)->getTag() == oomeeNumber)
        {
            _submitButton->setVisible(true);
            _selectedOomeeNo = oomeeNumber;
            _oomeeButtons.at(i)->playOomeeAnimation("Build_Dance_Wave", true);
            _oomeeButtons.at(i)->setScale(1.7f);
            _oomeeGlow->setPosition(_oomeeButtons.at(i)->getCenterPosition());
            _oomeeGlow->setVisible(true);
        }
        else
        {
            _oomeeButtons.at(i)->playOomeeAnimationNoSound("Build_Fall_Asleep");
            _oomeeButtons.at(i)->setScale(1.0f);
        }
    }
}

//------------PRIVATE OTHER FUNCTIONS------------
void ChildAccountScene::shouldChangeElementsToOomeeScreen()
{
    if(!DOBisDate() || !_childNameInputText->inputIsValid())
    {
        _childNameInputText->focusAndShowKeyboard();
    }
    else if(childNameExists(trim(_childNameInputText->getText())))
    {
        MessageBox::createWith(ERROR_CODE_NAME_EXISTS, _childNameInputText, this);
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
    std::string profileName = trim(_childNameInputText->getText());
    
    int day = std::atoi(_dayInputText->getText().c_str());
    int month = std::atoi(_monthInputText->getText().c_str());
    int year = std::atoi(_yearInputText->getText().c_str());
    
    std::string DOB = StringUtils::format("%04d-%02d-%02d",year,month,day);
    std::string gender = "MALE";
    
    auto backEndCaller = BackEndCaller::getInstance();
    if((FlowDataSingleton::getInstance()->isSignupFlow() || FlowDataSingleton::getInstance()->isSignupNewProfileFlow()) && ParentDataProvider::getInstance()->getAmountOfAvailableChildren() !=0)
    {
        backEndCaller->updateChild(ParentDataProvider::getInstance()->getIDForAvailableChildren(0), profileName, gender, DOB, _selectedOomeeNo);
    }
    else
    {
        backEndCaller->registerChild(profileName, gender, DOB, _selectedOomeeNo);
    }
}

bool ChildAccountScene::DOBisDate()
{
    int day = std::atoi(_dayInputText->getText().c_str());
    int month = std::atoi(_monthInputText->getText().c_str());
    int year = std::atoi(_yearInputText->getText().c_str());
    
    return isDate(day, month, year);
}

bool ChildAccountScene::DOBisDateInFuture()
{
    return isDateInFuture(StringUtils::format("%s-%s-%s",_yearInputText->getText().c_str(),_monthInputText->getText().c_str(),_dayInputText->getText().c_str()));
}

//----------------------- Delegate Functions ----------------------------

void ChildAccountScene::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
    _nextButton->setVisible(DOBisDate() && _childNameInputText->inputIsValid());
    
    if(inputLayer==_dayInputText && _dayInputText->getText().length() == 2)
    {
        _monthInputText->focusAndShowKeyboard();
    }
    else if(inputLayer==_monthInputText && (_monthInputText->getText().length()==2 || std::atoi(_monthInputText->getText().c_str()) >2))
    {
        _yearInputText->focusAndShowKeyboard();
    }
}

void ChildAccountScene::textInputReturnPressed(TextInputLayer* inputLayer)
{
    if(inputLayer == _childNameInputText)
    {
        _dayInputText->focusAndShowKeyboard();
    }
    else if(inputLayer == _dayInputText)
    {
        _monthInputText->focusAndShowKeyboard();
    }
    else if(inputLayer == _monthInputText)
    {
        _yearInputText->focusAndShowKeyboard();
    }
    else if(inputLayer == _yearInputText)
    {
        shouldChangeElementsToOomeeScreen();
    }
}

void ChildAccountScene::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
    
}

void ChildAccountScene::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
    
}

void ChildAccountScene::buttonPressed(ElectricDreamsButton* button)
{
    if(button == _cancelButton)
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
    }
    else if(button == _nextButton)
    {
        shouldChangeElementsToOomeeScreen();
    }
    else if(button == _backButton)
    {
        changeElementsToTextInputScreen();
    }
    else if(button == _submitButton)
    {
        registerChildAccount();
    }
    else
    {
        selectOomee(button->getTag());
    }
}

void ChildAccountScene::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    _childNameInputText->focusAndShowKeyboard();
    _childNameInputText->setVisible(true);
    _dayInputText->setVisible(true);
    _monthInputText->setVisible(true);
    _yearInputText->setVisible(true);
}

NS_AZOOMEE_END
