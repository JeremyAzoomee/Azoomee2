#include "ChildAccountSuccessScene.h"
#include "StringMgr.h"
#include "ElectricDreamsTextStyles.h"
#include "ElectricDreamsDecoration.h"
#include "ConfigStorage.h"
#include "BackEndCaller.h"
#include "ParentDataProvider.h"
#include "AudioMixer.h"
#include "ChildAccountScene.h"


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
    auto glow = createGlow();
    glow->setPosition(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2);
    glow->setScale(0.6);
    this->addChild(glow);
    
    addSideWiresToScreen(this, 0, 2);
    
    auto circle = Sprite::create("res/childSelection/selection.png");
    circle->setPosition(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2);
    circle->setScale(3.5);
    this->addChild(circle);
}

void ChildAccountSuccessScene::addButtonsToScene()
{
    oomeeButton = ElectricDreamsButton::createOomeeAsButton(0);
    oomeeButton->setCenterPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
    oomeeButton->setDelegate(this);
    oomeeButton->setScale(1.8);
    this->addChild(oomeeButton);
    
    addChildButton = ElectricDreamsButton::createAddButton();
    addChildButton->setCenterPosition(Vec2(origin.x+visibleSize.width*.2, origin.y+visibleSize.height/2));
    addChildButton->setDelegate(this);
    addChildButton->setScale(1.4);
    this->addChild(addChildButton);
}

void ChildAccountSuccessScene::addLabelsToLayer()
{
    auto title = createLabelHeader(StringMgr::getInstance()->getStringForKey(CHILDACCOUNTSUCCESSSCENE_REQUEST_NAME_LABEL));
    this->addChild(title);
    
    auto addButtonLabel = createLabelBody(StringMgr::getInstance()->getStringForKey(BUTTON_ADD_ANOTHER_LABEL));
    addButtonLabel->setPosition(addChildButton->getCenterPosition().x, addChildButton->getCenterPosition().y - addChildButton->getContentSize().height*1.2);
    this->addChild(addButtonLabel);
}

//--------------DELEGATE FUNCTIONS---------

void ChildAccountSuccessScene::buttonPressed(ElectricDreamsButton* button)
{
    if(!buttonHasBeenPressed)
    {
        buttonHasBeenPressed = true;
        
        if(button == oomeeButton)
        {
            oomeeButton->playOomeeAnimation("Build_Pop", false);
            this->scheduleOnce(schedule_selector(ChildAccountSuccessScene::callDelegateFunction), 2);
        }
        else if(button == addChildButton)
            Director::getInstance()->replaceScene(ChildAccountScene::createScene("", 0));
    }
}

void ChildAccountSuccessScene::callDelegateFunction(float dt)
{
    //oomeeButton->hideOomee();
    //AudioMixer::getInstance()->stopOomeeEffect();
    //BackEndCaller::getInstance()->childLogin(0);
}
