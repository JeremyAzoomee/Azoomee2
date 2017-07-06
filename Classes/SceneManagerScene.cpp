#include "SceneManagerScene.h"
#include "OnboardingScene.h"
#include "HQHistoryManager.h"
#include "BaseScene.h"
#include "OnboardingSuccessScene.h"
#include "ChildAccountScene.h"
#include "ChildSelectorScene.h"
#include "LoginScene.h"
#include "ChildAccountSuccessScene.h"
#include "OfflineHubScene.h"
#include "HQScene.h"
#include <AzoomeeCommon/Application.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "FlowDataSingleton.h"
#include "FTUScene.h"
#include <AzoomeeChat/UI/FriendListScene.h>
#include "ChatDelegate.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

cocos2d::Scene* SceneManagerScene::createScene(SceneNameEnum sceneName)
{
    auto scene = cocos2d::Scene::create();
    auto layer = SceneManagerScene::create();
    
    layer->nextScene = sceneName;
    scene->addChild(layer);
    
    return scene;
}

bool SceneManagerScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void SceneManagerScene::onEnterTransitionDidFinish()
{
    cocos2d::Scene* goToScene;
    
    switch (nextScene) {
        case Login:
        {
            forceToLandscape();
            goToScene = LoginScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("LOGIN");
            break;
        }
        case Onboarding:
        {
            forceToPortrait();
            goToScene = OnboardingScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("ONBOARDING");
            break;
        }
        case Base:
        {
            FlowDataSingleton::getInstance()->clearData();
            forceToLandscape();
            HQHistoryManager::getInstance()->addHomeIfHistoryEmpty();
            goToScene = BaseScene::createScene();
            break;
        }
        case BaseWithNoHistory:
        {
            FlowDataSingleton::getInstance()->clearData();
            forceToLandscape();
            HQHistoryManager::getInstance()->emptyHistory();
            goToScene = BaseScene::createScene();
            break;
        }
        case ChildAccount:
        {
            forceToPortrait();
            goToScene = ChildAccountScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("CHILD_ACCOUNT");
            break;
        }
        case ChildAccountSuccessScene:
        {
            forceToLandscape();
            goToScene = ChildAccountSuccessScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("CHILD_ACCOUNT_SUCCESS");
            break;
        }
        case ChildSelector:
        {
            forceToLandscape();
            goToScene = ChildSelectorScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("CHILD_SELECTOR");
            break;
        }
        case OnboardingSuccessScene:
        {
            forceToLandscape();
            goToScene = OnboardingSuccessScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("ONBOARDING_SUCCESS");
            break;
        }
        case OfflineHub:
        {
            forceToLandscape();
            OfflineChecker::getInstance()->setDelegate(nullptr);
            goToScene = OfflineHubScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("OFFLINE");
            break;
        }
        case OfflineArtsAppHQ:
        {
            forceToLandscape();
            goToScene = HQScene::createSceneForOfflineArtsAppHQ();
            AnalyticsSingleton::getInstance()->registerCurrentScene("OFFLINE_ARTS_APP");
            break;
        }
        case FTUScene:
        {
            forceToLandscape();
            goToScene = FTUScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("FTU_SCENE");
            break;
        }
        case ChatEntryPointScene:
        {
            // Make sure we set the chat delegate
            Azoomee::Chat::delegate = ChatDelegate::getInstance();
            
            acceptAnyOrientation();
            goToScene = Azoomee::Chat::FriendListScene::create();
            AnalyticsSingleton::getInstance()->registerCurrentScene("CHAT");
            break;
        }
        default:
            break;
    }
    
    Director::getInstance()->replaceScene(goToScene);
}

void SceneManagerScene::forceToPortrait()
{
    AnalyticsSingleton::getInstance()->setPortraitOrientation();
    Azoomee::Application::setOrientation(Azoomee::Application::Orientation::Portrait);
}

void SceneManagerScene::forceToLandscape()
{
    AnalyticsSingleton::getInstance()->setLandscapeOrientation();
    Azoomee::Application::setOrientation(Azoomee::Application::Orientation::Landscape);
}

void SceneManagerScene::acceptAnyOrientation()
{
    Azoomee::Application::setOrientation(Azoomee::Application::Orientation::Any);
}

NS_AZOOMEE_END
