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
#include "../ArtApp/Classes/AzoomeeArt/MainScene.h"
#include "../ArtApp/Classes/AzoomeeArt/AzoomeeArtApp.h"
#include "ArtAppDelegate.h"
#include "EmptySceneForSettings.h"
#include "WebViewSelector.h"

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

cocos2d::Scene* SceneManagerScene::createWebview(Orientation _orientation, const std::string& URL)
{
    auto scene = cocos2d::Scene::create();
    auto layer = SceneManagerScene::create();
    
    if(_orientation == Orientation::Portrait)
        layer->nextScene = WebviewPortrait;
    else
        layer->nextScene = WebviewLandscape;
    
    layer->webviewURL = URL;
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
    switch (nextScene) {
        case Login:
        {
            forceToLandscape();
            cocos2d::Scene*  goToScene = LoginScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("LOGIN");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case Onboarding:
        {
            forceToPortrait();
            AnalyticsSingleton::getInstance()->registerCurrentScene("ONBOARDING");
            
            auto funcCallAction = CallFunc::create([=](){
                
                cocos2d::Scene* goToScene = OnboardingScene::createScene();
                Director::getInstance()->replaceScene(goToScene);
            });
            
            auto action = Sequence::create(DelayTime::create(0.6f), funcCallAction, NULL);
            this->runAction(action);
            
            break;
        }
        case Base:
        {
            FlowDataSingleton::getInstance()->clearData();
            forceToLandscape();
            HQHistoryManager::getInstance()->addHomeIfHistoryEmpty();
            cocos2d::Scene* goToScene = BaseScene::createScene();
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case BaseWithNoHistory:
        {
            FlowDataSingleton::getInstance()->clearData();
            forceToLandscape();
            HQHistoryManager::getInstance()->emptyHistory();
            cocos2d::Scene* goToScene = BaseScene::createScene();
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case ChildAccount:
        {
            forceToPortrait();
            AnalyticsSingleton::getInstance()->registerCurrentScene("CHILD_ACCOUNT");
            
            //requires delay to ensure portrait is set before rendering scene.
            auto funcCallAction = CallFunc::create([=](){
                
                cocos2d::Scene* goToScene = ChildAccountScene::createScene();
                Director::getInstance()->replaceScene(goToScene);
            });
            
            auto action = Sequence::create(DelayTime::create(0.6f), funcCallAction, NULL);
            this->runAction(action);
            
            break;
        }
        case ChildAccountSuccessScene:
        {
            forceToLandscape();
            cocos2d::Scene* goToScene = ChildAccountSuccessScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("CHILD_ACCOUNT_SUCCESS");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case ChildSelector:
        {
            forceToLandscape();
            cocos2d::Scene* goToScene = ChildSelectorScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("CHILD_SELECTOR");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case OnboardingSuccessScene:
        {
            forceToLandscape();
            cocos2d::Scene* goToScene = OnboardingSuccessScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("ONBOARDING_SUCCESS");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case OfflineHub:
        {
            forceToLandscape();
            OfflineChecker::getInstance()->setDelegate(nullptr);
            cocos2d::Scene* goToScene = OfflineHubScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("OFFLINE");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case OfflineArtsAppHQ:
        {
            forceToLandscape();
            cocos2d::Scene* goToScene = HQScene::createSceneForOfflineArtsAppHQ();
            AnalyticsSingleton::getInstance()->registerCurrentScene("OFFLINE_ARTS_APP");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case FTUScene:
        {
            forceToLandscape();
            cocos2d::Scene* goToScene = FTUScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("FTU_SCENE");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case ChatEntryPointScene:
        {
            // Make sure we set the chat delegate
            Azoomee::Chat::delegate = ChatDelegate::getInstance();
            
            acceptAnyOrientation();
            cocos2d::Scene* goToScene = Azoomee::Chat::FriendListScene::create();
            AnalyticsSingleton::getInstance()->registerCurrentScene("CHAT");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case ArtAppEntryPointScene:
        {
            Azoomee::ArtApp::delegate = ArtAppDelegate::getInstance();
            ArtAppDelegate::getInstance()->ArtAppRunning = true;
            
            std::string fileName = ArtAppDelegate::getInstance()->getFileName();
            cocos2d::Scene* goToScene;
            forceToLandscape();
            if(FileUtils::getInstance()->isFileExist(fileName))
                goToScene = Azoomee::ArtApp::MainScene::createSceneWithDrawing(fileName);
            else
                goToScene = Azoomee::ArtApp::MainScene::createScene();
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case SettingsFromChat:
        {
            forceToLandscape();
            cocos2d::Scene* goToScene = EmptySceneForSettings::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("SETTINGS");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case WebviewPortrait:
        {
            #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                forceToPortrait();
            #endif
            AnalyticsSingleton::getInstance()->registerCurrentScene("WEBVIEWPORTRAIT");
            WebViewSelector::createSceneWithUrl(webviewURL, Orientation::Portrait);
            break;
        }
        case WebviewLandscape:
        {
            forceToLandscape();
            AnalyticsSingleton::getInstance()->registerCurrentScene("WEBVIEWLANDSCAPE");
            WebViewSelector::createSceneWithUrl(webviewURL, Orientation::Landscape);
            break;
        }
        default:
            break;
    }
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
