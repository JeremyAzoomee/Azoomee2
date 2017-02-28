#include "OnboardingSuccessScene.h"
#include "StringMgr.h"
#include "MessageBox.h"
#include "ElectricDreamsTextStyles.h"
#include "ElectricDreamsDecoration.h"

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
    addFunctionalElementsToScene();
    
    return true;
}

//----------------- SCENE SETUP ---------------
void OnboardingSuccessScene::addVisualElementsToScene()
{
    
}

void OnboardingSuccessScene::addFunctionalElementsToScene()
{
    
}

void OnboardingSuccessScene::addButtonsToLayer()
{
    
}
void OnboardingSuccessScene::addLabelsToLayer()
{
    
}
