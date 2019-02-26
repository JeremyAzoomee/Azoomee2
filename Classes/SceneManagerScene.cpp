#include "SceneManagerScene.h"
#include "HQHistoryManager.h"
#include "ChildSelectorScene.h"
#include "LoginScene.h"
#include "OfflineHubScene.h"
#include "HQScene2.h"
#include <AzoomeeCommon/Application.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "FlowDataSingleton.h"
#include <AzoomeeChat/UI/FriendListScene.h>
#include <AzoomeeChat/UI/FriendListSceneArtPreview.h>
#include "ChatDelegate.h"
#include "../ArtApp/Classes/AzoomeeArt/MainScene.h"
#include "../ArtApp/Classes/AzoomeeArt/AzoomeeArtApp.h"
#include "OomeeMakerDelegate.h"
#include <AzoomeeOomeeMaker/UI/OomeeSelectScene.h>
#include "ArtAppDelegate.h"
#include "WebViewSelector.h"
#include "IntroVideoScene.h"
#include "ContentHistoryManager.h"
#include "AddChildScene.h"
#include "WelcomeScene.h"
#include "NavigationScene.h"
#include "ContentFeedHQScene.h"
#include "LocalContentHQScene.h"
#include "GalleryHQScene.h"
#include "MeHQ.h"

#include "SettingsHub.h"
#include "ChildSettingsScene.h"

#include "CoinCollectLayer.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>

#ifdef VODACOM_BUILD
#include "Vodacom/VodacomOnboardingScene.h"
#endif

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

cocos2d::Scene* SceneManagerScene::createWebview(Orientation _orientation, const std::string& URL, Vec2 closeButtonAnchor)
{
    auto scene = cocos2d::Scene::create();
    auto layer = SceneManagerScene::create();
    
    if(_orientation == Orientation::Portrait)
        layer->nextScene = WebviewPortrait;
    else
        layer->nextScene = WebviewLandscape;
    
    layer->webviewURL = URL;
    layer->_closeButtonAnchor = closeButtonAnchor;
    
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
            acceptAnyOrientation();
            cocos2d::Scene*  goToScene = LoginScene::create();
            AnalyticsSingleton::getInstance()->registerCurrentScene("LOGIN");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case Base:
        {
            FlowDataSingleton::getInstance()->clearData();
            returnToPrevOrientation();
            acceptAnyOrientation();
            HQHistoryManager::getInstance()->addDefaultHQIfHistoryEmpty();
			const std::string& currentHQ = HQHistoryManager::getInstance()->getCurrentHQ();
			
			ContentFeedHQScene* hqScene = ContentFeedHQScene::create();
			hqScene->setHQCategory(currentHQ);
			cocos2d::Scene* goToScene = hqScene;
			
			if(currentHQ == ConfigStorage::kMeHQName)
			{
				MeHQ* hqScene = MeHQ::create();
				hqScene->setHQCategory(currentHQ);
				goToScene = hqScene;
			}
			else if(currentHQ == ConfigStorage::kArtAppHQName)
			{
				GalleryHQScene* hqScene = GalleryHQScene::create();
				hqScene->setHQCategory(currentHQ);
				goToScene = hqScene;
			}
            //Azoomee::Scene* goToScene = BaseScene::create();
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case BaseWithNoHistory:
        {
            FlowDataSingleton::getInstance()->clearData();
            returnToPrevOrientation();
            acceptAnyOrientation();
            HQHistoryManager::getInstance()->emptyHistory();
            cocos2d::Scene* goToScene = NavigationScene::create();
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case ChildSelector:
        {
            returnToPrevOrientation();
            acceptAnyOrientation();
            cocos2d::Scene* goToScene = ChildSelectorScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("CHILD_SELECTOR");
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
            cocos2d::Scene* goToScene = HQScene2::createSceneForOfflineArtsAppHQ();
            AnalyticsSingleton::getInstance()->registerCurrentScene("OFFLINE_ARTS_APP");
            Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, goToScene));
            break;
        }
        case ChatEntryPointScene:
        {
            // Make sure we set the chat delegate
            Azoomee::Chat::delegate = ChatDelegate::getInstance();
            returnToPrevOrientation();
            acceptAnyOrientation();
            cocos2d::Scene* goToScene;
            if(Azoomee::Chat::delegate->_imageFileName != "")
            {
                goToScene = Azoomee::Chat::FriendListSceneArtPreview::create();
            }
            else
            {
                goToScene = Azoomee::Chat::FriendListScene::create();
            }
            AnalyticsSingleton::getInstance()->registerCurrentScene("CHAT");
            Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, goToScene));
            break;
        }
        case ArtAppEntryPointScene:
        {
            HQHistoryManager::getInstance()->updatePrevOrientation();
            Azoomee::ArtApp::delegate = ArtAppDelegate::getInstance();
            ArtAppDelegate::getInstance()->ArtAppRunning = true;
            
            std::string fileName = ArtAppDelegate::getInstance()->getFileName();
            cocos2d::Scene* goToScene;
            forceToLandscape();
            if(FileUtils::getInstance()->isFileExist(fileName))
                goToScene = Azoomee::ArtApp::MainScene::createSceneWithDrawing(fileName);
            else
                goToScene = Azoomee::ArtApp::MainScene::createScene();
            Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, goToScene));
            break;
        }
        case OomeeMakerEntryPointScene:
        {
            HQHistoryManager::getInstance()->updatePrevOrientation();
            Azoomee::OomeeMaker::delegate = OomeeMakerDelegate::getInstance();
            forceToLandscape();
            cocos2d::Scene* goToScene = Azoomee::OomeeMaker::OomeeSelectScene::create();
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case SettingsFromChat:
        {
            HQHistoryManager::getInstance()->updatePrevOrientation();
            forceToPortrait();
			auto* goToScene = SettingsHub::create();
			goToScene->setOrigin(SettingsOrigin::CHAT);
            AnalyticsSingleton::getInstance()->registerCurrentScene("SETTINGS");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case SettingsFromHQ:
        {
            HQHistoryManager::getInstance()->updatePrevOrientation();
            forceToPortrait();
			auto* goToScene = SettingsHub::create();
			goToScene->setOrigin(SettingsOrigin::HQ);
            AnalyticsSingleton::getInstance()->registerCurrentScene("SETTINGS");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
		case SettingsFromChildSelect:
		{
			HQHistoryManager::getInstance()->updatePrevOrientation();
			forceToPortrait();
			auto* goToScene = SettingsHub::create();
			goToScene->setOrigin(SettingsOrigin::CHILD_SELECT);
			AnalyticsSingleton::getInstance()->registerCurrentScene("SETTINGS");
			Director::getInstance()->replaceScene(goToScene);
			break;
		}
        case WebviewPortrait:
        {
            HQHistoryManager::getInstance()->updatePrevOrientation();
            #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                forceToPortrait();
            #endif
            AnalyticsSingleton::getInstance()->registerCurrentScene("WEBVIEWPORTRAIT");
            cocos2d::Scene* goToScene = WebViewSelector::createSceneWithUrl(webviewURL, Orientation::Portrait, _closeButtonAnchor);
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case WebviewLandscape:
        {
            HQHistoryManager::getInstance()->updatePrevOrientation();
            forceToLandscape();
            AnalyticsSingleton::getInstance()->registerCurrentScene("WEBVIEWLANDSCAPE");
            cocos2d::Scene* goToScene = WebViewSelector::createSceneWithUrl(webviewURL, Orientation::Landscape, _closeButtonAnchor);
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case introVideo:
        {
            if(Director::getInstance()->getVisibleSize().width / Director::getInstance()->getVisibleSize().height > 1.5)
            {
                forceToPortrait();
            }
            else
            {
                forceToLandscape();
            }
            HQHistoryManager::getInstance()->updatePrevOrientation();
            Director::getInstance()->replaceScene(IntroVideoScene::create());
            break;
        }
        case AddChild:
        {
            acceptAnyOrientation();
            HQHistoryManager::getInstance()->updatePrevOrientation();
            Director::getInstance()->replaceScene(AddChildScene::createWithFlowStage(AddChildFlow::ADDITIONAL_NAME));
            break;
        }
        case AddChildFirstTime:
        {
            acceptAnyOrientation();
            HQHistoryManager::getInstance()->updatePrevOrientation();
            Director::getInstance()->replaceScene(AddChildScene::createWithFlowStage(AddChildFlow::FIRST_TIME_SETUP_NAME));
            break;
        }
		case AddChildAnon:
		{
			acceptAnyOrientation();
			HQHistoryManager::getInstance()->updatePrevOrientation();
			Director::getInstance()->replaceScene(AddChildScene::createWithFlowStage(AddChildFlow::ANON_NAME));
			break;
		}
		case WelcomeScene:
		{
			acceptAnyOrientation();
			HQHistoryManager::getInstance()->updatePrevOrientation();
			Director::getInstance()->replaceScene(WelcomeScene::create());
			break;
		}
		case ChildSettingsHub:
		{
			acceptAnyOrientation();
			HQHistoryManager::getInstance()->updatePrevOrientation();
			Director::getInstance()->replaceScene(ChildSettingsScene::create());
			break;
		}
#ifdef VODACOM_BUILD
		case VodacomOnboarding:
		{
			HQHistoryManager::getInstance()->updatePrevOrientation();
			forceToPortrait();
			Director::getInstance()->replaceScene(VodacomOnboardingScene::create());
			break;
		}
#endif
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

void SceneManagerScene::returnToPrevOrientation()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if(ContentHistoryManager::getInstance()->getReturnedFromContent() || HQHistoryManager::getInstance()->hasReturnedFromForcedOrientation())
    {
        if(HQHistoryManager::getInstance()->getPrevHQOrientation() == Portrait)
        {
            forceToPortrait();
        }
        else
        {
            forceToLandscape();
        }
    }
#endif
    HQHistoryManager::getInstance()->setReturnedFromForcedOrientation(false);
}

NS_AZOOMEE_END
