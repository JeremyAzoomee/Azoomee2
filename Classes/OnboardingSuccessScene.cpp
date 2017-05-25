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
    addButtonsToScene();
    addLabelsToLayer();
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

void OnboardingSuccessScene::addButtonsToScene()
{
    oomeeButton = ElectricDreamsButton::createOomeeButtonWithOutline(FlowDataSingleton::getInstance()->getOomeeColourNumber(), ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(0));
    oomeeButton->setCenterPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    oomeeButton->setDelegate(this);
    oomeeButton->setMixPanelButtonName("OnboardingSuccessOomeePressed");
    this->addChild(oomeeButton);
    
    if(!IAPSuccess)
    {
        startTrial = ElectricDreamsButton::createButtonWithText("Start Trial!", 100);
        startTrial->setPosition(origin.x + startTrial->getContentSize().height,oomeeButton->getPositionY());
        startTrial->setDelegate(this);
        startTrial->setMixPanelButtonName("OnboardingSuccessStartTrialButton");
        this->addChild(startTrial);
    }
}

void OnboardingSuccessScene::addLabelsToLayer()
{
    std::string TitleText = StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_TITLE_LABEL);
    std::string SubTitleText = StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_CHECKEMAIL_LABEL);
    
    if(IAPSuccess)
    {
        TitleText = "Your Azoomee subscription is now active!\nEnjoy unlimited access to TV shows, games and audiobooks";
        SubTitleText = "You can choose your own Oomee later.";
    }
    else
    {
        TitleText = "Don’t worry, you can still access lots of exciting TV shows,\ngames and audiobooks for free.";
        SubTitleText = "You can always unlock full access later.";
    }
    
    auto title = createLabelHeader(TitleText);
    title->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.88);
    this->addChild(title);
    
    std::string subTitleString = stringReplace(StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_SUB_LABEL), "%s", ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(0));

    auto subTitle = createLabelHeaderWhite(subTitleString);
    subTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.78);
    this->addChild(subTitle);
    
    if(!IAPSuccess && ParentDataProvider::getInstance()->getAmountOfAvailableChildren() == 1)
    {
        auto checkEmail = createLabelBodyCentred(SubTitleText);
        checkEmail->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.2);
        this->addChild(checkEmail);
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
