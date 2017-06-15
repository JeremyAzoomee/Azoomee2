#include "OnboardingSuccessScene.h"
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "IAPUpsaleLayer.h"
#include "RoutePaymentSingleton.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include "FlowDataSingleton.h"

USING_NS_CC;
using namespace Azoomee;


Scene* OnboardingSuccessScene::createScene()
{
    auto scene = Scene::create();
    auto layer = OnboardingSuccessScene::create();
    scene->addChild(layer);
    layer->IAPSuccess = FlowDataSingleton::getInstance()->getIAPSuccess();
    layer->setupScene();
    
    return scene;
}

bool OnboardingSuccessScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    return true;
}

 void OnboardingSuccessScene::setupScene()
{
    addVisualElementsToScene();
    addTitleLabelsToLayer();
    addButtonsToScene();
    addSubLabelsToLayer();
}

void OnboardingSuccessScene::onEnterTransitionDidFinish()
{
    if(!IAPSuccess)
    {
        AnalyticsSingleton::getInstance()->displayIAPUpsaleEvent("OnboardingSuccess");
        IAPUpsaleLayer::create();
    }
}

//----------------- SCENE SETUP ---------------
void OnboardingSuccessScene::addVisualElementsToScene()
{
    addSideWiresToScreen(this, 0, 2);
}

void OnboardingSuccessScene::addTitleLabelsToLayer()
{
    std::string TitleText = StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_IAP_SUCCESS_TITLE_LABEL);
    std::string BottomText = StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_IAP_SUCCESS_BOTTOM_LABEL);
    std::string subTitleString = stringReplace(StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_SUCCESS_SUB_LABEL), "%s", ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(0));
    
    if(!IAPSuccess)
    {
        TitleText = StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_IAP_FAIL_TITLE_LABEL);
        BottomText = StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_IAP_FAIL_BOTTOM_LABEL);
        subTitleString = stringReplace(StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_FAIL_SUB_LABEL), "%s", ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(0));
    }
    
    
    Label* title;
    
    if(IAPSuccess)
    {
        title = createLabelHeader(TitleText);
        title->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.88);
        title->setLineSpacing(20);
        this->addChild(title);
    }
    else
    {
        title = createLabelMessageBoxTitle(TitleText);
        title->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.88);
        this->addChild(title);
    }

    subTitleLabel = createLabelHeaderWhite(subTitleString);
    subTitleLabel->setPosition(origin.x + visibleSize.width * 0.5, title->getPositionY() - subTitleLabel->getContentSize().height*1.7);
    this->addChild(subTitleLabel);
}

void OnboardingSuccessScene::addButtonsToScene()
{
    oomeeButton = ElectricDreamsButton::createOomeeButtonWithOutline(FlowDataSingleton::getInstance()->getOomeeColourNumber(), ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(0));
    oomeeButton->setCenterPosition(Vec2(origin.x + visibleSize.width/2, subTitleLabel->getPositionY()-visibleSize.height*.10 -oomeeButton->getContentSize().height/2));
    oomeeButton->setDelegate(this);
    oomeeButton->setMixPanelButtonName("OnboardingSuccessOomeePressed");
    this->addChild(oomeeButton);
}

void OnboardingSuccessScene::addSubLabelsToLayer()
{
    if(!IAPSuccess && ParentDataProvider::getInstance()->getAmountOfAvailableChildren() == 1)
    {
        Label* OrText = createLabelBodyCentred("- or -");
        OrText->setAnchorPoint(Vec2(0.5,0.5));
        OrText->setPosition(origin.x + visibleSize.width/2, oomeeButton->getPositionY() - OrText->getContentSize().height * 1.5);
        this->addChild(OrText);
        
        startTrial = ElectricDreamsButton::createSecondaryButtonWithWidth("Start your FREE trial now", 10);
        startTrial->setDelegate(this);
        startTrial->setMixPanelButtonName("OnboardingSuccessStartTrialButton");
        startTrial->setCenterPosition(Vec2(origin.x + visibleSize.width/2, OrText->getPositionY() - startTrial->getContentSize().height ));
        this->addChild(startTrial);
        
        Label* restOfBottomText = Label::createWithTTF("And don’t forget, you can cancel anytime.", "fonts/Sofia Pro Soft Regular.otf", 45);
        restOfBottomText->setColor(Color3B(28, 244, 244));
        restOfBottomText->setPosition(origin.x + visibleSize.width/2, startTrial->getPositionY() - restOfBottomText->getContentSize().height*1.3);
        restOfBottomText->setAnchorPoint(Vec2(0.5,0.5));
        this->addChild(restOfBottomText);
    }
}

//--------------DELEGATE FUNCTIONS---------

void OnboardingSuccessScene::buttonPressed(ElectricDreamsButton* button)
{
    if(!buttonHasBeenPressed)
    {
        if(button == oomeeButton)
        {
            buttonHasBeenPressed = true;
            oomeeButton->playOomeeAnimation("Build_Pop", false);
            this->scheduleOnce(schedule_selector(OnboardingSuccessScene::callDelegateFunction), 2);
        }
        else if(button == startTrial)
            RoutePaymentSingleton::getInstance()->startInAppPayment();
    }
}

void OnboardingSuccessScene::callDelegateFunction(float dt)
{
    oomeeButton->hideOomee();
    AudioMixer::getInstance()->stopOomeeEffect();
    ParentDataParser::getInstance()->logoutChild();
    BackEndCaller::getInstance()->childLogin(0);
}
