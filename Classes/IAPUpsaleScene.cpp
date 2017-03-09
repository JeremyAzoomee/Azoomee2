#include "IAPUpsaleScene.h"
#include "StringMgr.h"
#include "ElectricDreamsTextStyles.h"
#include "ElectricDreamsDecoration.h"
#include "ConfigStorage.h"
#include "BackEndCaller.h"
#include "ParentDataProvider.h"
#include "AudioMixer.h"


USING_NS_CC;

Scene* IAPUpsaleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = IAPUpsaleScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool IAPUpsaleScene::init()
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
void IAPUpsaleScene::addVisualElementsToScene()
{
    addSideWiresToScreen(this, 0, 2);
}

void IAPUpsaleScene::addButtonsToScene()
{

}

void IAPUpsaleScene::addLabelsToLayer()
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

void IAPUpsaleScene::buttonPressed(ElectricDreamsButton* button)
{

}

