#include "OfflineHubHQElement.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "GameDataManager.h"
#include "WebViewSelector.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN


Scene* OfflineHubHQElement::createScene()
{
    auto scene = Scene::create();
    auto layer = OfflineHubHQElement::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool OfflineHubHQElement::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void OfflineHubHQElement::addHQSceneElement(std::string category, std::map<std::string, std::string> itemData, Vec2 shape, float delay)
{
    //category = "GAME HQ";
    
    elementVisual = HQSceneElementVisual::create();
    elementVisual->setCategory(category);
    elementVisual->setItemData(itemData);
    elementVisual->setShape(shape);
    elementVisual->setDelay(delay);
    elementVisual->setCreatedForOffline(true);
    elementVisual->createHQSceneElement();
    
    this->addChild(elementVisual);
    
    this->setContentSize(elementVisual->getContentSize());
    
    addListenerToElement(itemData, false);
}

//-------------------All elements below this are used internally-----------------

void OfflineHubHQElement::addListenerToElement(std::map<std::string, std::string> itemData, bool preview)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getBoundingBox().size;//getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
                
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
            
            if(Director::getInstance()->getRunningScene()->getChildByName("baseLayer")) //this is needed because of the offline hub
            {
                if(Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("contentLayer")->getNumberOfRunningActions() > 0) return false;
            }
            
            AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
            iamtouched = false;
            std::string startUrl = FileUtils::getInstance()->getWritablePath() + "gameCache/" + itemData.at("id") + "/" +  itemData.at("uri").c_str();
            
            CCLOG("Action to come: %s", startUrl.c_str());
            
            AnalyticsSingleton::getInstance()->contentItemSelectedEvent(itemData.at("title"), itemData.at("description"), itemData.at("type"), itemData.at("id"), -1, -1, "1,1");
            
            WebViewSelector::createSceneWithUrl(startUrl.c_str());
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), elementVisual->baseLayer);
}

void OfflineHubHQElement::startUpElementDependingOnType(std::map<std::string, std::string> itemData)
{
    this->getParent()->getParent()->getParent()->stopAllActions();
    GameDataManager::getInstance()->startProcessingGame(itemData);
}

NS_AZOOMEE_END
