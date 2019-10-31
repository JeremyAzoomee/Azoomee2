#include "SceneManagerScene.h"
#include "HQHistoryManager.h"
#include "ChildSelectorScene.h"
#include "LoginScene.h"
#include "OfflineHubScene.h"
#include <AzoomeeCommon/Application.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "FlowDataSingleton.h"
#include "AzoomeeChat/UI/FriendListScene.h"
#include "AzoomeeChat/UI/FriendListSceneArtPreview.h"
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
#include "ContentFeedHQScene.h"
#include "MeHQ.h"
#include "RewardDisplayHandler.h"
#include "HQSceneArtsApp.h"

#include "SettingsHub.h"
#include "ChildSettingsScene.h"
#include "ShopScene.h"

#include "IAPScene.h"
#include "SignupScene.h"

#include "HQScene.h"

#include <AzoomeeCommon/UI/RoundedRectSprite.h>

#include "CoinCollectLayer.h"
#include <AzoomeeCommon/Crashlytics/CrashlyticsConfig.h>

#ifdef AZOOMEE_VODACOM_BUILD
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
        layer->nextScene = SceneNameEnum::WebviewPortrait;
    else
        layer->nextScene = SceneNameEnum::WebviewLandscape;
    
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
        case SceneNameEnum::Login:
        {
            acceptAnyOrientation();
            cocos2d::Scene*  goToScene = LoginScene::create();
            AnalyticsSingleton::getInstance()->registerCurrentScene("LOGIN");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case SceneNameEnum::Base:
        {
            FlowDataSingleton::getInstance()->clearData();
            returnToPrevOrientation();
            acceptAnyOrientation();
			if(ContentHistoryManager::getInstance()->getReturnedFromContent())
			{
				setCrashlyticsKeyWithString(CrashlyticsConsts::kContentIdKey, "");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				showHoldingUI();
#else
				this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5), CallFunc::create([this](){
					showHoldingUI();
				})));
#endif
			}
			else
			{
				Director::getInstance()->replaceScene(getBaseScene());
			}
            break;
        }
        case SceneNameEnum::BaseWithNoHistory:
        {
            FlowDataSingleton::getInstance()->clearData();
            returnToPrevOrientation();
            acceptAnyOrientation();
            HQHistoryManager::getInstance()->emptyHistory();
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
			Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case SceneNameEnum::ChildSelector:
        {
            returnToPrevOrientation();
            acceptAnyOrientation();
            cocos2d::Scene* goToScene = ChildSelectorScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("CHILD_SELECTOR");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case SceneNameEnum::OfflineHub:
        {
            forceToLandscape();
            OfflineChecker::getInstance()->setDelegate(nullptr);
            cocos2d::Scene* goToScene = OfflineHubScene::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("OFFLINE");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case SceneNameEnum::OfflineArtsAppHQ:
        {
			forceToLandscape();
			cocos2d::Scene* goToScene = HQSceneArtsApp::createScene();
            AnalyticsSingleton::getInstance()->registerCurrentScene("OFFLINE_ARTS_APP");
            Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, goToScene));
            break;
        }
        case SceneNameEnum::ChatEntryPointScene:
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
        case SceneNameEnum::ArtAppEntryPointScene:
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
			AnalyticsSingleton::getInstance()->registerCurrentScene("ART_APP");
            Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, goToScene));
            break;
        }
        case SceneNameEnum::OomeeMakerEntryPointScene:
        {
            HQHistoryManager::getInstance()->updatePrevOrientation();
            Azoomee::OomeeMaker::delegate = OomeeMakerDelegate::getInstance();
			OomeeMakerDelegate::getInstance()->_oomeeMakerFromShop = false;
            forceToLandscape();
            cocos2d::Scene* goToScene = Azoomee::OomeeMaker::OomeeSelectScene::create();
			AnalyticsSingleton::getInstance()->registerCurrentScene("OOMEE_MAKER");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
		case SceneNameEnum::OomeeMakerEntryPointSceneFromShop:
		{
			HQHistoryManager::getInstance()->updatePrevOrientation();
			Azoomee::OomeeMaker::delegate = OomeeMakerDelegate::getInstance();
			OomeeMakerDelegate::getInstance()->_oomeeMakerFromShop = true;
			forceToLandscape();
			cocos2d::Scene* goToScene = Azoomee::OomeeMaker::OomeeSelectScene::create();
			AnalyticsSingleton::getInstance()->registerCurrentScene("OOMEE_MAKER");
			Director::getInstance()->replaceScene(goToScene);
			break;
		}
        case SceneNameEnum::SettingsFromChat:
        {
            HQHistoryManager::getInstance()->updatePrevOrientation();
            forceToPortrait();
			auto* goToScene = SettingsHub::create();
			goToScene->setOrigin(SettingsOrigin::CHAT);
            AnalyticsSingleton::getInstance()->registerCurrentScene("SETTINGS");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case SceneNameEnum::SettingsFromHQ:
        {
            HQHistoryManager::getInstance()->updatePrevOrientation();
            forceToPortrait();
			auto* goToScene = SettingsHub::create();
			goToScene->setOrigin(SettingsOrigin::HQ);
            AnalyticsSingleton::getInstance()->registerCurrentScene("SETTINGS");
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
		case SceneNameEnum::SettingsFromChildSelect:
		{
			HQHistoryManager::getInstance()->updatePrevOrientation();
			forceToPortrait();
			auto* goToScene = SettingsHub::create();
			goToScene->setOrigin(SettingsOrigin::CHILD_SELECT);
			AnalyticsSingleton::getInstance()->registerCurrentScene("SETTINGS");
			Director::getInstance()->replaceScene(goToScene);
			break;
		}
        case SceneNameEnum::WebviewPortrait:
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
        case SceneNameEnum::WebviewLandscape:
        {
            HQHistoryManager::getInstance()->updatePrevOrientation();
            forceToLandscape();
            AnalyticsSingleton::getInstance()->registerCurrentScene("WEBVIEWLANDSCAPE");
            cocos2d::Scene* goToScene = WebViewSelector::createSceneWithUrl(webviewURL, Orientation::Landscape, _closeButtonAnchor);
            Director::getInstance()->replaceScene(goToScene);
            break;
        }
        case SceneNameEnum::introVideo:
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
        case SceneNameEnum::AddChild:
        {
            acceptAnyOrientation();
            HQHistoryManager::getInstance()->updatePrevOrientation();
			AnalyticsSingleton::getInstance()->registerCurrentScene("ADD_CHILD");
            Director::getInstance()->replaceScene(AddChildScene::createWithFlowStage(AddChildFlow::ADDITIONAL_NAME));
            break;
        }
        case SceneNameEnum::AddChildFirstTime:
        {
            acceptAnyOrientation();
            HQHistoryManager::getInstance()->updatePrevOrientation();
			AnalyticsSingleton::getInstance()->registerCurrentScene("ADD_CHILD");
            Director::getInstance()->replaceScene(AddChildScene::createWithFlowStage(AddChildFlow::FIRST_TIME_SETUP_NAME));
            break;
        }
		case SceneNameEnum::AddChildAnon:
		{
			acceptAnyOrientation();
			HQHistoryManager::getInstance()->updatePrevOrientation();
			AnalyticsSingleton::getInstance()->registerCurrentScene("ADD_CHILD");
			Director::getInstance()->replaceScene(AddChildScene::createWithFlowStage(AddChildFlow::ANON_NAME));
			break;
		}
		case SceneNameEnum::WelcomeScene:
		{
			acceptAnyOrientation();
			HQHistoryManager::getInstance()->updatePrevOrientation();
			Director::getInstance()->replaceScene(WelcomeScene::create());
			break;
		}
		case SceneNameEnum::ChildSettingsHub:
		{
			acceptAnyOrientation();
			HQHistoryManager::getInstance()->updatePrevOrientation();
			AnalyticsSingleton::getInstance()->registerCurrentScene("CHILD_SETTINGS");
			Director::getInstance()->replaceScene(ChildSettingsScene::create());
			break;
		}
		case SceneNameEnum::Shop:
		{
			returnToPrevOrientation();
			acceptAnyOrientation();
			HQHistoryManager::getInstance()->updatePrevOrientation();
			AnalyticsSingleton::getInstance()->registerCurrentScene("SHOP");
			Director::getInstance()->replaceScene(ShopScene::create());
			break;
		}
		case SceneNameEnum::IAP:
		{
#ifdef AZOOMEE_VODACOM_BUILD
			HQHistoryManager::getInstance()->updatePrevOrientation();
			forceToPortrait();
			Director::getInstance()->replaceScene(VodacomOnboardingScene::create());
#else
			returnToPrevOrientation();
			acceptAnyOrientation();
			HQHistoryManager::getInstance()->updatePrevOrientation();
			AnalyticsSingleton::getInstance()->registerCurrentScene("IAP");
			Director::getInstance()->replaceScene(IAPScene::create());
#endif
            break;
		}
		case SceneNameEnum::Signup:
		{
			returnToPrevOrientation();
			acceptAnyOrientation();
			HQHistoryManager::getInstance()->updatePrevOrientation();
			AnalyticsSingleton::getInstance()->registerCurrentScene("SIGNUP");
			Director::getInstance()->replaceScene(SignupScene::create());
			break;
		}
#ifdef AZOOMEE_VODACOM_BUILD
		case SceneNameEnum::VodacomOnboarding:
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
	if(ConfigStorage::getInstance()->isDevicePhone())
	{
		forceToPortrait();
	}
	else
	{
    	Azoomee::Application::setOrientation(Azoomee::Application::Orientation::Any);
	}
}

void SceneManagerScene::returnToPrevOrientation()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if(ContentHistoryManager::getInstance()->getReturnedFromContent() || HQHistoryManager::getInstance()->hasReturnedFromForcedOrientation())
    {
        if(HQHistoryManager::getInstance()->getPrevHQOrientation() == Portrait || ConfigStorage::getInstance()->isDevicePhone())
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

cocos2d::Scene* SceneManagerScene::getBaseScene()
{
	/*HQHistoryManager::getInstance()->addDefaultHQIfHistoryEmpty();
	const std::string& currentHQ = HQHistoryManager::getInstance()->getCurrentHQ();
	
	HQSceneDepreciated* scene = HQHistoryManager::getInstance()->getCachedHQScene(currentHQ);
	cocos2d::Scene* goToScene = scene;
	if(!scene)
	{
		if(currentHQ == ConfigStorage::kMeHQName)
		{
			scene = MeHQ::create();
			scene->setHQCategory(currentHQ);
		}
		else
		{
			scene = ContentFeedHQScene::create();
			scene->setHQCategory(currentHQ);
		}
		HQHistoryManager::getInstance()->addHQSceneToCache(currentHQ, scene);
		goToScene = scene;
	}
    return HQScene::create();//goToScene;
    */
    HQScene* scene = HQHistoryManager::getInstance()->getCachedHQScene();
    if(!scene)
    {
        scene = HQScene::create();
        HQHistoryManager::getInstance()->cacheHQScene(scene);
    }
    return scene;
}

void SceneManagerScene::showHoldingUI()
{
    LayerColor* bgColour = LayerColor::create(Color4B(Style::Color::darkIndigo));
	this->addChild(bgColour, -1);
	
	this->setPosition(Director::getInstance()->getVisibleOrigin());
	this->setContentSize(Director::getInstance()->getVisibleSize());
	
    RoundedRectSprite* pattern = RoundedRectSprite::create();
    pattern->setTexture("res/decoration/pattern_stem_tile.png");
    pattern->setCornerRadius(0);
    pattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    pattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    pattern->setColor(Style::Color::white);
    pattern->setScaleMode(RoundedRectSprite::ScaleMode::TILE);
    pattern->setContentSize(getContentSize());
    pattern->setOpacity(100);
    addChild(pattern);
	
	LayerColor* overlay = LayerColor::create(Color4B(7,4,34,80));
	this->addChild(overlay);
	
	for(int i = 0; i < 3; i++)
	{
		auto loadingCircle = Sprite::create("res/modal/loading.png");
		loadingCircle->setNormalizedPosition(Vec2(0.5,0.5));
		loadingCircle->setOpacity(0);
		loadingCircle->setRotation(RandomHelper::random_int(0, 360));
		loadingCircle->setScale(0.6 + i * 0.2);
		
		this->addChild(loadingCircle);
		
		int direction = CCRANDOM_0_1() < 0.5 ? 1 : -1;
		
		loadingCircle->runAction(RepeatForever::create(RotateBy::create(CCRANDOM_0_1() + 1, 360 * direction)));
		loadingCircle->runAction(FadeTo::create(0.5, 255));
	}
	
	RewardDisplayHandler::getInstance()->showNextReward();
	
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.5), CallFunc::create([this](){
		Director::getInstance()->replaceScene(getBaseScene());
	})));
}

NS_AZOOMEE_END
