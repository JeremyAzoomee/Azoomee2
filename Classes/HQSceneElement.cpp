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
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
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

USING_NS_CC;
using namespace Azoomee;
using namespace network;


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

void HQSceneElement::setCategory(std::string category)
{
    elementCategory = category;
}

void HQSceneElement::setItemData(std::map<std::string, std::string> itemData)
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
            CCLOG("Action to come: %s", elementItemData["uri"].c_str());
            
            if(!ChildDataProvider::getInstance()->getIsChildLoggedIn())
            {
                CCLOG("MixPanel: %s, %s, %s", elementItemData["title"].c_str(), elementItemData["description"].c_str(), elementCategory.c_str());
                AnalyticsSingleton::getInstance()->previewContentClickedEvent(elementItemData["title"], elementItemData["description"], elementItemData["type"]);
                PreviewLoginSignupMessageBox::create();
                return true;
            }
            
            if(elementItemData["type"] == "MANUAL")
            {
                ManualGameInputLayer::create();
                return true;
            }
            
            if(elementItemData["entitled"] == "false")
            {
                AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
                AnalyticsSingleton::getInstance()->displayIAPUpsaleEvent("MainHub");
                IAPUpsaleLayer::createRequiresPin();
                return true;
            }
                
            AnalyticsSingleton::getInstance()->openContentEvent(elementItemData["title"], elementItemData["description"], elementItemData["type"], elementItemData["id"], elementRowNumber, elementIndex, HQDataProvider::getInstance()->getHumanReadableHighlightDataForSpecificItem(elementCategory, elementRowNumber, elementIndex));
            startUpElementDependingOnType();
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), elementVisual->baseLayer);
}

void HQSceneElement::startUpElementDependingOnType()
{
    
    CCLOG("uri: %s, contentid: %s, category: %s", elementItemData["uri"].c_str(), elementItemData["id"].c_str(), elementCategory.c_str());
    
    this->getParent()->getParent()->getParent()->stopAllActions();
    
    if(HQDataProvider::getInstance()->getTypeForSpecificItem(elementCategory, elementItemData["id"]) == "GAME")
    {
        GameDataManager::getInstance()->startProcessingGame(elementItemData);
    }
    else if((HQDataProvider::getInstance()->getTypeForSpecificItem(elementCategory, elementItemData["id"]) == "VIDEO")||(HQDataProvider::getInstance()->getTypeForSpecificItem(elementCategory, elementItemData["id"]) == "AUDIO"))
    {
        VideoPlaylistManager::getInstance()->setPlaylist(HQDataProvider::getInstance()->getAllElementDataInRow(elementCategory, elementRowNumber));
        auto webViewSelector = WebViewSelector::create();
        webViewSelector->loadWebView(elementItemData["uri"].c_str());
    }
    else if(HQDataProvider::getInstance()->getTypeForSpecificItem(elementCategory, elementItemData["id"]) == "AUDIOGROUP")
    {
        NavigationLayer *navigationLayer = (NavigationLayer *)Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("NavigationLayer");
        navigationLayer->startLoadingGroupHQ(elementItemData["uri"]);
        
        auto funcCallAction = CallFunc::create([=](){
            HQDataProvider::getInstance()->getDataForGroupHQ(elementItemData["uri"]);
            HQHistoryManager::getInstance()->setGroupHQSourceId(elementItemData["id"]);
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
    }
    else if(HQDataProvider::getInstance()->getTypeForSpecificItem(elementCategory, elementItemData["id"]) == "GROUP")
    {
        NavigationLayer *navigationLayer = (NavigationLayer *)Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("NavigationLayer");
        navigationLayer->startLoadingGroupHQ(elementItemData["uri"]);
        
        auto funcCallAction2 = CallFunc::create([=](){
            HQDataProvider::getInstance()->getDataForGroupHQ(elementItemData["uri"]);
            HQHistoryManager::getInstance()->setGroupHQSourceId(elementItemData["id"]);
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction2, NULL));
    }
}
