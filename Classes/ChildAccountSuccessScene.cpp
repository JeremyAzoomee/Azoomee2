#include "ChildAccountSuccessScene.h"
#include "StringMgr.h"
#include "ElectricDreamsTextStyles.h"
#include "ElectricDreamsDecoration.h"
#include "ConfigStorage.h"
#include "BackEndCaller.h"
#include "ParentDataProvider.h"
#include "AudioMixer.h"


USING_NS_CC;

Scene* ChildAccountSuccessScene::createScene()
{
    auto scene = Scene::create();
    auto layer = ChildAccountSuccessScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool ChildAccountSuccessScene::init()
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
void ChildAccountSuccessScene::addVisualElementsToScene()
{
    addGlowToScreen(this, 1);
    addSideWiresToScreen(this, 0, 2);
    
    auto circle = Sprite::create("res/childSelection/selection.png");
    circle->setPosition(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2);
    this->addChild(circle);
}

void ChildAccountSuccessScene::addButtonsToScene()
{
    auto oomeeButton = ElectricDreamsButton::createOomeeAsButton(0);
    oomeeButton->setCenterPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
    oomeeButton->setDelegate(this);
    oomeeButton->setVisible(false);
    oomeeButton->setScale(2);
    this->addChild(oomeeButton);
}

void ChildAccountSuccessScene::addLabelsToLayer()
{
    auto title = createLabelHeader(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSUCCESSSCENE_REQUEST_NAME_LABEL));
    this->addChild(title);
}

//--------------DELEGATE FUNCTIONS---------

void ChildAccountSuccessScene::buttonPressed(ElectricDreamsButton* button)
{
    oomeeButton->playOomeeAnimation("Build_Pop", false);
    this->scheduleOnce(schedule_selector(ChildAccountSuccessScene::callDelegateFunction), 2);
}

void ChildAccountSuccessScene::callDelegateFunction(float dt)
{
    oomeeButton->hideOomee();
    AudioMixer::getInstance()->stopOomeeEffect();
    BackEndCaller::getInstance()->childLogin(0);
}
