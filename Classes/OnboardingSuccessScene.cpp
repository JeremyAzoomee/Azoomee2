#include "OnboardingSuccessScene.h"
#include "StringMgr.h"
#include "ElectricDreamsTextStyles.h"
#include "ElectricDreamsDecoration.h"
#include "ConfigStorage.h"
#include "BackEndCaller.h"
#include "ParentDataProvider.h"
#include "AudioMixer.h"
#include "IAPUpsaleLayer.h"
#include "StringFunctions.h"
#include "PaymentSingleton.h"

USING_NS_CC;

Scene* OnboardingSuccessScene::createScene()
{
    auto scene = Scene::create();
    auto layer = OnboardingSuccessScene::create();
    scene->addChild(layer);
    layer->IAPEnabled = false;
    layer->IAPSuccess = false;
    layer->setupScene();
    
    return scene;
}

Scene* OnboardingSuccessScene::createScene(bool IAPEnabled, bool IAPSuccess)
{
    auto scene = Scene::create();
    auto layer = OnboardingSuccessScene::create();
    scene->addChild(layer);
    layer->IAPEnabled = IAPEnabled;
    layer->IAPSuccess = IAPSuccess;
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
    if(IAPEnabled && !IAPSuccess)
        IAPUpsaleLayer::create();
}

//----------------- SCENE SETUP ---------------
void OnboardingSuccessScene::addVisualElementsToScene()
{
    addSideWiresToScreen(this, 0, 2);
}

void OnboardingSuccessScene::addButtonsToScene()
{
    std::string oomeeUrl = ParentDataProvider::getInstance()->getAvatarForAnAvailableChildren(0);
    int oomeeNr = ConfigStorage::getInstance()->getOomeeNumberForUrl(oomeeUrl);
    
    oomeeButton = ElectricDreamsButton::createOomeeButtonWithOutline(oomeeNr, ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(0));
    oomeeButton->setCenterPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    oomeeButton->setDelegate(this);
    oomeeButton->setMixPanelButtonName("OnboardingSuccessOomeePressed");
    this->addChild(oomeeButton);
    
    if(IAPEnabled && !IAPSuccess)
    {
        startTrial = ElectricDreamsButton::createButtonWithText("Start Trial!", 100);
        startTrial->setPosition(origin.x + startTrial->getContentSize().height,oomeeButton->getPositionY());
        startTrial->setDelegate(this);
        this->addChild(startTrial);
    }
}

void OnboardingSuccessScene::addLabelsToLayer()
{
    std::string TitleText = StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_TITLE_LABEL);
    
    if(IAPSuccess)
        TitleText = "You've successfully enabled your trial";
    
    auto title = createLabelHeader(TitleText);
    title->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.88);
    this->addChild(title);
    
    std::string subTitleString = stringReplace(StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_SUB_LABEL), "%s", ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(0));

    auto subTitle = createLabelHeaderWhite(subTitleString);
    subTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.78);
    this->addChild(subTitle);
    
    if(!IAPSuccess)
    {
        auto checkEmail = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_CHECKEMAIL_LABEL));
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
            PaymentSingleton::getInstance()->startAmazonPayment();
    }
}

void OnboardingSuccessScene::callDelegateFunction(float dt)
{
    oomeeButton->hideOomee();
    AudioMixer::getInstance()->stopOomeeEffect();
    BackEndCaller::getInstance()->childLogin(0);
}
