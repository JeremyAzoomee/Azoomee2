//
//  AddChildScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 03/07/2018.
//

#include "AddChildScene.h"

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
    setSceneForFlow();
}

void AddChildScene::onSizeChanged()
{
    this->removeAllChildren();
    addBackground();
    setSceneForFlow();
}

void AddChildScene::setFlowStage(const AddChildFlow& flowStage)
{
    _currentFlowStage = flowStage;
}

Azoomee::Scene* AddChildScene::createWithFlowStage(const AddChildFlow& flowStage)
{
    auto scene = AddChildScene::create();
    scene->setFlowStage(flowStage);
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
    switch (_currentFlowStage) {
        case AddChildFlow::FIRST_TIME_SETUP_NAME:
        
        break;
        
        case AddChildFlow::ADDITIONAL_NAME:
        
        break;
        
        case AddChildFlow::AGE:
        
        break;
        
        case AddChildFlow::OOMEE:
        
        break;
        
        default:
        break;
    }
}

// Delegate Functions
void AddChildScene::nextLayer()
{
    
}

void AddChildScene::prevLayer()
{
    
}

NS_AZOOMEE_END
