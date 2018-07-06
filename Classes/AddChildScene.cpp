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

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool AddChildScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
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
    if(flowStage == AddChildFlow::FIRST_TIME_SETUP_NAME);
    {
        scene->_addingFirstChild = true;
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
        nextLayer = ChildNameLayerFirstTime::create();
        break;
        
        case AddChildFlow::ADDITIONAL_NAME:
        nextLayer = ChildNameLayer::create();
        break;
        
        case AddChildFlow::AGE:
        nextLayer = ChildAgeLayer::create();
        break;
        
        case AddChildFlow::OOMEE:
        nextLayer = ChildOomeeLayer::create();
        break;
        
        default:
        break;
    }
    if(nextLayer)
    {
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
    _prevFlowStage = _currentFlowStage;
    switch(_currentFlowStage)
    {
        case AddChildFlow::FIRST_TIME_SETUP_NAME:
        case AddChildFlow::ADDITIONAL_NAME:
        _currentFlowStage = AddChildFlow::AGE;
        setSceneForFlow();
        break;
        
        case AddChildFlow::AGE:
        _childCreator->addChild();
        break;
        
        case AddChildFlow::OOMEE:
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
        break;
        
        default:
        break;
    }
}

void AddChildScene::prevLayer()
{
    switch(_currentFlowStage)
    {
        case AddChildFlow::FIRST_TIME_SETUP_NAME:
        case AddChildFlow::ADDITIONAL_NAME:
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
        break;
        
        case AddChildFlow::AGE:
        _currentFlowStage = _prevFlowStage;
        setSceneForFlow();
        break;
        
        case AddChildFlow::OOMEE:
        _currentFlowStage = AddChildFlow::ADDITIONAL_NAME;
        setSceneForFlow();
        break;
        
        default:
        break;
    }
}

void AddChildScene::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    _currentFlowStage = AddChildFlow::OOMEE;
    setSceneForFlow();
}

void AddChildScene::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    FlowDataSingleton::getInstance()->setErrorCode(errorCode);
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
}

NS_AZOOMEE_END
