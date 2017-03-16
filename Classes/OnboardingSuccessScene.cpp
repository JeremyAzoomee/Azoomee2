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
    
    addVisualElementsToScene();
    addButtonsToScene();
    addLabelsToLayer();
    
    return true;
}

void OnboardingSuccessScene::onEnterTransitionDidFinish()
{
    if(IAPUpsaleLayer::isAmazonDevice())
        IAPUpsaleLayer::createRequiresPin();
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
    
    if(IAPUpsaleLayer::isAmazonDevice())
    {
        startTrial = ElectricDreamsButton::createButtonWithText("Start Your Free Trial!", 100);
        startTrial->setPosition(origin.x + startTrial->getContentSize().height,origin.y+ startTrial->getContentSize().height);
        startTrial->setDelegate(this);
        this->addChild(startTrial);
    }
}

void OnboardingSuccessScene::addLabelsToLayer()
{
    auto title = createLabelHeader(StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_TITLE_LABEL));
    title->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.88);
    this->addChild(title);
    
    std::string subTitleString = stringReplace(StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_SUB_LABEL), "%s", ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(0));

    auto subTitle = createLabelHeaderWhite(subTitleString);
    subTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.78);
    this->addChild(subTitle);
    
    auto checkEmail = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_CHECKEMAIL_LABEL));
    checkEmail->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.2);
    this->addChild(checkEmail);
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
