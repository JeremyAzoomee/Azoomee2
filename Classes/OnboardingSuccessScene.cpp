#include "OnboardingSuccessScene.h"
#include "StringMgr.h"
#include "ElectricDreamsTextStyles.h"
#include "ElectricDreamsDecoration.h"
#include "ConfigStorage.h"
#include "LoginScene.h"

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

//----------------- SCENE SETUP ---------------
void OnboardingSuccessScene::addVisualElementsToScene()
{
    addSideWiresToScreen(this, 0, 2);
}

void OnboardingSuccessScene::addButtonsToScene()
{
    oomeeButton = ElectricDreamsButton::createOomeeButton(2, "Ooubrey");
    oomeeButton->setCenterPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    oomeeButton->setDelegate(this);
    this->addChild(oomeeButton);
}

void OnboardingSuccessScene::addLabelsToLayer()
{
    auto title = createLabelHeader(StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_TITLE_LABEL));
    title->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.88);
    this->addChild(title);
    
    auto subTitle = createLabelHeaderWhite(StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_SUB_LABEL));
    subTitle->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.78);
    this->addChild(subTitle);
    
    auto checkEmail = createLabelBodyCentred(StringMgr::getInstance()->getStringForKey(ONBOARDINGSUCCESSSCENE_CHECKEMAIL_LABEL));
    checkEmail->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.2);
    this->addChild(checkEmail);
}

//--------------DELEGATE FUNCTIONS---------

void OnboardingSuccessScene::buttonPressed(ElectricDreamsButton* button)
{
    auto loginScene = LoginScene::createScene(0);
    Director::getInstance()->replaceScene(loginScene);
}
