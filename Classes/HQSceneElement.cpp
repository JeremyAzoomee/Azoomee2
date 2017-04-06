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
#include "ImageDownloader.h"
#include "HQDataProvider.h"
#include "GameDataManager.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "HQDataParser.h"
#include "NavigationLayer.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "MessageBox.h"
#include "HQScene.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "ElectricDreamsTextStyles.h"
#include "PaymentSingleton.h"
#include "IAPUpsaleLayer.h"

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

void HQSceneElement::addHQSceneElement(std::string category, std::map<std::string, std::string> itemData, Vec2 shape, float delay) //This method is being called by HQScene.cpp with all variables.
{
    elementVisual = HQSceneElementVisual::create();
    elementVisual->addHQSceneElement(category, itemData, shape, delay,false);
    this->addChild(elementVisual);
    this->setContentSize(elementVisual->getContentSize());
    
    if(itemData["entitled"] == "false" && !ChildDataProvider::getInstance()->getIsChildLoggedIn())
    {
        addListenerToElement(itemData["uri"], itemData["id"], category, itemData["title"], itemData["description"], itemData["type"], true, itemData["entitled"], false);
        
    }
    else
    {
        addListenerToElement(itemData["uri"], itemData["id"], category, itemData["title"], itemData["description"], itemData["type"], false,itemData["entitled"], PaymentSingleton::getInstance()->showIAPContent());
    }
}

//-------------------All elements below this are used internally-----------------
void HQSceneElement::addListenerToElement(std::string uri, std::string contentId, std::string category, std::string title, std::string description, std::string type, bool preview, std::string entitled, bool IAPEnabled)
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
            CCLOG("Action to come: %s", uri.c_str());
            
            if(preview)
            {
                CCLOG("MixPanel: %s, %s, %s", title.c_str(),description.c_str(),category.c_str());
                AnalyticsSingleton::getInstance()->previewContentClickedEvent(title,description,type);
                MessageBox::createPreviewLoginSignupMessageBox();
                return true;
            }
            else
            {
                if(entitled == "false")
                {
                    if(IAPEnabled)
                    {
                        AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
                        AnalyticsSingleton::getInstance()->displayIAPUpsaleEvent("MainHub");
                        IAPUpsaleLayer::createRequiresPin();
                    }
                }
                else
                {
                    AnalyticsSingleton::getInstance()->openContentEvent(title, description, type, contentId);
                    startUpElementDependingOnType(uri, contentId, category);
                }
            }
            
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), elementVisual->baseLayer);
}

void HQSceneElement::startUpElementDependingOnType(std::string uri, std::string contentId, std::string category)
{
    
    CCLOG("uri: %s, contentid: %s, category: %s", uri.c_str(), contentId.c_str(), category.c_str());
    
    this->getParent()->getParent()->getParent()->stopAllActions();
    
    if(HQDataProvider::getInstance()->getTypeForSpecificItem(category, contentId) == "GAME")
    {

        GameDataManager::getInstance()->startProcessingGame(uri, contentId);
    }
    else if((HQDataProvider::getInstance()->getTypeForSpecificItem(category, contentId) == "VIDEO")||(HQDataProvider::getInstance()->getTypeForSpecificItem(category, contentId) == "AUDIO"))
    {
        auto webViewSelector = WebViewSelector::create();
        webViewSelector->loadWebView(uri.c_str());
    }
    else if(HQDataProvider::getInstance()->getTypeForSpecificItem(category, contentId) == "AUDIOGROUP")
    {
        NavigationLayer *navigationLayer = (NavigationLayer *)Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("NavigationLayer");
        navigationLayer->startLoadingGroupHQ(uri);
        
        auto funcCallAction = CallFunc::create([=](){
            HQDataProvider::getInstance()->getDataForGroupHQ(uri);
            HQHistoryManager::getInstance()->setGroupHQSourceId(contentId);
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
    }
    else if(HQDataProvider::getInstance()->getTypeForSpecificItem(category, contentId) == "GROUP")
    {
        NavigationLayer *navigationLayer = (NavigationLayer *)Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("NavigationLayer");
        navigationLayer->startLoadingGroupHQ(uri);
        
        auto funcCallAction2 = CallFunc::create([=](){
            HQDataProvider::getInstance()->getDataForGroupHQ(uri);
            HQHistoryManager::getInstance()->setGroupHQSourceId(contentId);
        });
        
        this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction2, NULL));
    }
}
