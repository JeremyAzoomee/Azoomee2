//
//  AddChildScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 03/07/2018.
//

#include "AddChildScene.h"
#include "ChildNameLayerFirstTime.h"
#include "ChildNameLayer.h"
#include "ChildAgeLayer.h"
#include "ChildOomeeLayer.h"
#include "SceneManagerScene.h"
#include "FlowDataSingleton.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/ErrorCodes.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Tutorial/TutorialController.h>
#include <AzoomeeCommon/Tutorial/TutorialMessagingNode.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/UI/NotificationNodeDisplayManager.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::map<AddChildFlow, std::string> AddChildScene::kFlowStateStrConvMap = {
	{AddChildFlow::FIRST_TIME_SETUP_NAME, "FirstChildSetupName"},
	{AddChildFlow::ADDITIONAL_NAME, "AdditionalChildName"},
	{AddChildFlow::AGE, "ChildAge"},
	{AddChildFlow::OOMEE, "Oomee"},
	{AddChildFlow::ANON_NAME, "AnonChildName"},
	{AddChildFlow::ANON_AGE, "AnonChildAge"},
	{AddChildFlow::ANON_OOMEE, "AnonOomee"}
};

bool AddChildScene::init()
{
    if(!Super::init())
    {
        return false;
    }

	CCASSERT(kFlowStateStrConvMap.size() == (int)AddChildFlow::FLOW_COUNT, "Not all flow states in str conversion map!");

    addBackground();
    
    return true;
}

void AddChildScene::onEnter()
{
    _childCreator = ChildCreator::create();
    _childCreator->setHttpRespnseDelegate(this);
    _childCreator->setFirstTime(_addingFirstChild);
    setSceneForFlow();
    Super::onEnter();
}

void AddChildScene::onSizeChanged()
{
    this->removeAllChildrenWithCleanup(true);
    _sceneLayer = nullptr;
    addBackground();
    setSceneForFlow();
    Super::onSizeChanged();
}

void AddChildScene::setFlowStage(const AddChildFlow& flowStage)
{
    _currentFlowStage = flowStage;
}

Azoomee::Scene* AddChildScene::createWithFlowStage(const AddChildFlow& flowStage)
{
    auto scene = AddChildScene::create();
    scene->setFlowStage(flowStage);
    if(flowStage == AddChildFlow::FIRST_TIME_SETUP_NAME)
    {
        scene->_addingFirstChild = true;
    }
	else if(flowStage == AddChildFlow::ANON_NAME)
	{
		if(TutorialController::getInstance()->isTutorialActive())
		{
			TutorialController::getInstance()->endTutorial();
		}
		TutorialController::getInstance()->startTutorial(TutorialController::kFTUAddChildID);
		scene->_addingAnonChild = true;
	}
    return scene;
}

void AddChildScene::addBackground()
{
    const Size& contentSize = this->getContentSize();
    bool isPortrait = contentSize.height > contentSize.width;
    
    LayerColor* bgColour = LayerColor::create(Color4B::BLACK, contentSize.width, contentSize.height);
    this->addChild(bgColour);
    
    auto wireLeft = Sprite::create(StringUtils::format("res/childSelection/wireLeft%s.png", isPortrait ? "_portrait" : ""));
    wireLeft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    wireLeft->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    wireLeft->setScale(contentSize.height / wireLeft->getContentSize().height);
    this->addChild(wireLeft);
    
    auto wireRight = Sprite::create(StringUtils::format("res/childSelection/wireRight%s.png", isPortrait ? "_portrait" : ""));
    wireRight->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    wireRight->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    wireRight->setScale(contentSize.height / wireRight->getContentSize().height);
    this->addChild(wireRight);
}

void AddChildScene::setSceneForFlow()
{
    if(_sceneLayer)
    {
        _sceneLayer->removeFromParent();
        _sceneLayer = nullptr;
    }
    AddChildLayer* nextLayer = nullptr;
    switch (_currentFlowStage) {
        case AddChildFlow::FIRST_TIME_SETUP_NAME:
        {
            nextLayer = ChildNameLayerFirstTime::create();
            break;
        }
        case AddChildFlow::ADDITIONAL_NAME:
        {
            nextLayer = ChildNameLayer::create();
            break;
        }
        case AddChildFlow::AGE:
        {
            nextLayer = ChildAgeLayer::create();
            break;
        }
        case AddChildFlow::OOMEE:
        {
            nextLayer = ChildOomeeLayer::create();
            break;
        }
		case AddChildFlow::ANON_NAME:
		{
			if(TutorialController::getInstance()->isTutorialActive() && TutorialController::getInstance()->getCurrentState() == TutorialController::kNameEntry)
			{
				NotificationNodeDisplayManager::getInstance()->clearMessagingLayer();
				NotificationNodeDisplayManager::getInstance()->addMessagingNode(TutorialMessagingNode::create(_("What's your name?")));
			}
			nextLayer = ChildNameLayerFirstTime::create();
			break;
		}
		case AddChildFlow::ANON_AGE:
		{
			if(TutorialController::getInstance()->isTutorialActive() && TutorialController::getInstance()->getCurrentState() == TutorialController::kAgeEntry)
			{
				NotificationNodeDisplayManager::getInstance()->clearMessagingLayer();
				NotificationNodeDisplayManager::getInstance()->addMessagingNode(TutorialMessagingNode::create(_("How old are you?")));
			}
			nextLayer = ChildAgeLayer::create();
			break;
		}
		case AddChildFlow::ANON_OOMEE:
		{
			nextLayer = ChildOomeeLayer::create();
			break;
		}
        default:
        break;
    }
    if(nextLayer)
    {
		AnalyticsSingleton::getInstance()->createChildFlowEvent(getAnalyticsStringForFlowState(_currentFlowStage));
        nextLayer->setChildCreator(_childCreator);
        nextLayer->setDelegate(this);
        nextLayer->setContentSize(this->getContentSize());
        this->addChild(nextLayer);
        _sceneLayer = nextLayer;
    }
}

// Delegate Functions
void AddChildScene::nextLayer()
{
	AnalyticsSingleton::getInstance()->createChildNextPressed();
	AudioMixer::getInstance()->playEffect(NEXT_BUTTON_AUDIO_EFFECT);
    switch(_currentFlowStage)
    {
        case AddChildFlow::FIRST_TIME_SETUP_NAME:
        case AddChildFlow::ADDITIONAL_NAME:
        {
            _currentFlowStage = AddChildFlow::AGE;
            setSceneForFlow();
            break;
        }
        case AddChildFlow::AGE:
        {
             _childCreator->addChild();
            break;
        }
        case AddChildFlow::OOMEE:
        {
            BackEndCaller::getInstance()->getAvailableChildren();
            break;
        }
		case AddChildFlow::ANON_NAME:
		{
			if(TutorialController::getInstance()->isTutorialActive() && TutorialController::getInstance()->getCurrentState() == TutorialController::kNameEntry)
			{
				TutorialController::getInstance()->nextStep();
			}
			_currentFlowStage = AddChildFlow::ANON_AGE;
			setSceneForFlow();
			break;
		}
		case AddChildFlow::ANON_AGE:
		{
			if(TutorialController::getInstance()->isTutorialActive() && TutorialController::getInstance()->getCurrentState() == TutorialController::kAgeEntry)
			{
				TutorialController::getInstance()->nextStep();
			}
			_childCreator->updateChild(ParentDataProvider::getInstance()->getChild(0));
			break;
		}
		case AddChildFlow::ANON_OOMEE:
		{
			UserDefault::getInstance()->setBoolForKey("anonOnboardingComplete", true);
			UserDefault::getInstance()->flush();
			BackEndCaller::getInstance()->anonymousDeviceLogin();
			break;
		}
        default:
        break;
    }
}

void AddChildScene::prevLayer()
{
	AnalyticsSingleton::getInstance()->createChildBackPressed();
	AudioMixer::getInstance()->playEffect(BACK_BUTTON_AUDIO_EFFECT);
    switch(_currentFlowStage)
    {
        case AddChildFlow::FIRST_TIME_SETUP_NAME:
        case AddChildFlow::ADDITIONAL_NAME:
        {
            BackEndCaller::getInstance()->getAvailableChildren();
            break;
        }
        case AddChildFlow::AGE:
        {
            _currentFlowStage = _addingFirstChild ? AddChildFlow::FIRST_TIME_SETUP_NAME : AddChildFlow::ADDITIONAL_NAME;
            setSceneForFlow();
            break;
        }
        case AddChildFlow::OOMEE:
        {
            _addingFirstChild = false;
            _childCreator = ChildCreator::create();
            _childCreator->setHttpRespnseDelegate(this);
            _childCreator->setFirstTime(_addingFirstChild);
            _currentFlowStage = AddChildFlow::ADDITIONAL_NAME;
            setSceneForFlow();
            break;
        }
		case AddChildFlow::ANON_AGE:
		{
			if(TutorialController::getInstance()->isTutorialActive() && TutorialController::getInstance()->getCurrentState() == TutorialController::kAgeEntry)
			{
				TutorialController::getInstance()->endTutorial();
				TutorialController::getInstance()->startTutorial(TutorialController::kFTUAddChildID);
			}
			_currentFlowStage = AddChildFlow::ANON_NAME;
			setSceneForFlow();
			break;
		}
		case AddChildFlow::ANON_NAME:
		{
			if(TutorialController::getInstance()->isTutorialActive() && TutorialController::getInstance()->getCurrentState() == TutorialController::kNameEntry)
			{
				TutorialController::getInstance()->endTutorial();
			}
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Login));
		}
        default:
            break;
    }
}

std::string AddChildScene::getAnalyticsStringForFlowState(const AddChildFlow& state)
{
	return kFlowStateStrConvMap.at(state);
}

void AddChildScene::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagRegisterChild)
	{
		AnalyticsSingleton::getInstance()->childProfileCreatedSuccessEvent();
		rapidjson::Document data;
		data.Parse(body.c_str());
		_childCreator->setCreatedChildId(getStringFromJson("id", data));
		_currentFlowStage = AddChildFlow::OOMEE;
		setSceneForFlow();
	}
	else if(requestTag == API::TagUpdateChild)
	{
		AnalyticsSingleton::getInstance()->childProfileCreatedSuccessEvent();
		rapidjson::Document data;
		data.Parse(body.c_str());
		_childCreator->setCreatedChildId(getStringFromJson("id", data));
		_currentFlowStage = AddChildFlow::ANON_OOMEE;
		setSceneForFlow();
	}
}

void AddChildScene::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	if(errorCode == ERROR_CODE_ALREADY_REGISTERED)
	{
		errorCode = ERROR_CODE_NAME_EXISTS;
	}
    AnalyticsSingleton::getInstance()->childProfileCreatedErrorEvent(errorCode);
	FlowDataSingleton::getInstance()->setErrorCode(errorCode);
	Director::getInstance()->replaceScene(SceneManagerScene::createScene(_currentFlowStage == AddChildFlow::ANON_AGE ? SceneNameEnum::Base : SceneNameEnum::ChildSelector));
}

NS_AZOOMEE_END
