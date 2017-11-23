//This class is responsible for displaying an element on the scrollviews. The following are set up here:
// - highlight (the size) of an icon
// - setting up touch listener for a button and loading the content that it addresses (usually a webview, that is native in android and ios)

//Structure:
// - we need to create a LayerColor first, 10px bigger than the image
// - we put the image on
// - we put the overlay on the top of the image (colour depending on the category) for touch actions
// - we put game icon and labels on top of the overlay image

//waiting for addHQSceneElement command from HQScene after init.

#include "HQSceneElement.h"
#include "WebViewSelector.h"
#include "HQDataProvider.h"
#include "GameDataManager.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQDataParser.h"
#include "NavigationLayer.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "PreviewLoginSignupMessageBox.h"
#include "HQScene.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include "RoutePaymentSingleton.h"
#include "IAPUpsaleLayer.h"
#include "ManualGameInputLayer.h"
#include "VideoPlaylistManager.h"
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectStorage.h>
#include "DynamicNodeHandler.h"
#include "ContentHistoryManager.h"

using namespace cocos2d;
using namespace network;

NS_AZOOMEE_BEGIN

Scene* HQSceneElement::createScene()
{
    auto scene = Scene::create();
    auto layer = HQSceneElement::create();
    scene->addChild(layer);

    return scene;
}

bool HQSceneElement::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void HQSceneElement::setCategory(const std::string &category)
{
    elementCategory = category;
}

void HQSceneElement::setItemData(const HQContentItemObjectRef &itemData)
{
    elementItemData = itemData;
}

void HQSceneElement::setElementRow(int rowNumber)
{
    elementRowNumber = rowNumber;
}

void HQSceneElement::setElementIndex(int index)
{
    elementIndex = index;
}

void HQSceneElement::addHQSceneElement() //This method is being called by HQScene.cpp with all variables.
{
    elementVisual = HQSceneElementVisual::create();
    elementVisual->setCategory(elementCategory);
    elementVisual->setItemData(elementItemData);
    elementVisual->setShape(HQDataProvider::getInstance()->getHighlightDataForSpecificItem(elementCategory, elementRowNumber, elementIndex));
    elementVisual->setDelay(elementRowNumber * 0.5 + elementIndex * 0.1);
    elementVisual->setCreatedForOffline(false);
    elementVisual->createHQSceneElement();
    
    this->addChild(elementVisual);
    this->setContentSize(elementVisual->getContentSize());
    
    addListenerToElement();
}

//-------------------All elements below this are used internally-----------------
void HQSceneElement::addListenerToElement()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getBoundingBox().size;//getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("contentLayer")->getNumberOfRunningActions() > 0) return false;
        
        if(rect.containsPoint(locationInNode))
        {
            if(elementVisual->overlayWhenTouched) elementVisual->overlayWhenTouched->setOpacity(150);
            
            movedAway = false;
            iamtouched = true;
            touchPoint = touch->getLocation();
            
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [=](Touch *touch, Event *event)
    {
        if((touch->getLocation().distance(touchPoint) > 10)&&(!movedAway))
        {
            movedAway = true;
            iamtouched = false;
             if(elementVisual->overlayWhenTouched) elementVisual->overlayWhenTouched->setOpacity(0);
        }
        
        return true;
    };
    
    listener->onTouchEnded = [=](Touch *touch, Event *event)
    {
        if(iamtouched)
        {
            if(elementVisual->overlayWhenTouched) elementVisual->overlayWhenTouched->setOpacity(0);
            
            if(Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("contentLayer")->getNumberOfRunningActions() > 0) return false;
            
            AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
            iamtouched = false;
            
            if(elementItemData->getType() == "MANUAL")
            {
                ManualGameInputLayer::create();
                return true;
            }
            
            if(!elementItemData->isEntitled())
            {
                AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
                AnalyticsSingleton::getInstance()->contentItemSelectedEvent(elementItemData, elementRowNumber, elementIndex, HQDataProvider::getInstance()->getHumanReadableHighlightDataForSpecificItem(elementCategory, elementRowNumber, elementIndex));
                
                DynamicNodeHandler::getInstance()->createDynamicNodeByGroupId(DynamicNodeHandler::kUpgradeGroup);
                
                return true;
            }
                
            AnalyticsSingleton::getInstance()->contentItemSelectedEvent(elementItemData, elementRowNumber, elementIndex, HQDataProvider::getInstance()->getHumanReadableHighlightDataForSpecificItem(elementCategory, elementRowNumber, elementIndex));
            startUpElementDependingOnType();
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), elementVisual->baseLayer);
}

void HQSceneElement::startUpElementDependingOnType()
{
    this->getParent()->getParent()->getParent()->stopAllActions();
    
    if(elementItemData->getType() == "GAME")
    {
        ContentHistoryManager::getInstance()->setLastOppenedContent(elementItemData);
        GameDataManager::getInstance()->startProcessingGame(elementItemData);
    }
    else if((elementItemData->getType() == "VIDEO") || (elementItemData->getType() == "AUDIO"))
    {
        ContentHistoryManager::getInstance()->setLastOppenedContent(elementItemData);
        VideoPlaylistManager::getInstance()->setPlaylist(HQDataObjectStorage::getInstance()->getHQDataObjectForKey(elementCategory)->getHqCarousels().at(elementRowNumber));
        
        auto webViewSelector = WebViewSelector::create();
        webViewSelector->loadWebView(elementItemData->getUri().c_str(),Orientation::Landscape);
    }
    else if((elementItemData->getType() == "AUDIOGROUP")||(elementItemData->getType() == "GROUP"))
    {
        NavigationLayer *navigationLayer = (NavigationLayer *)Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("NavigationLayer");
        navigationLayer->startLoadingGroupHQ(elementItemData->getUri());
        
        auto funcCallAction = CallFunc::create([=](){
            HQDataProvider::getInstance()->getDataForGroupHQ(elementItemData->getUri());
            HQHistoryManager::getInstance()->setGroupHQSourceId(elementItemData->getContentItemId());
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
    }
}

NS_AZOOMEE_END
