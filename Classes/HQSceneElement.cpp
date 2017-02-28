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
#include "ConfigStorage.h"
#include "HQDataParser.h"
#include "NavigationLayer.h"
#include "ChildDataProvider.h"
#include "MessageBox.h"
#include "HQScene.h"
#include "AudioMixer.h"
#include "HQHistoryManager.h"
#include "AnalyticsSingleton.h"
#include "ElectricDreamsTextStyles.h"

USING_NS_CC;

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
    resizeSceneElement(shape, category);
    createColourLayer(category, delay / 10);
    
    std::string itemid = itemData["id"];
    std::string itementitled = itemData["entitled"];
    std::string itemuri = itemData["uri"];
    std::string itemTitle = itemData["title"];
    std::string itemDescription = itemData["description"];
    std::string itemType = itemData["type"];
    
    auto funcCallAction = CallFunc::create([=](){
    
    if(!aboutToExit) addImageToBaseLayer(HQDataProvider::getInstance()->getImageUrlForItem(itemid, shape));
    if(!aboutToExit) addGradientToBottom(category);
    
        if(!aboutToExit)
        {
            auto iconSprite = addIconToImage(category);
            addLabelsToImage(itemData, iconSprite);
        }
    
    if(!aboutToExit) addTouchOverlayToElement();
    
    if(itementitled == "true")
    {
        if(!aboutToExit) addListenerToElement(itemuri, itemid, category, itemTitle, itemDescription, itemType, false);
    }
    else
    {
        if(!ChildDataProvider::getInstance()->getIsChildLoggedIn())
        {
           if(!aboutToExit) addListenerToElement(itemuri, itemid, category, itemTitle, itemDescription, itemType, true);
        }
        
       if(!aboutToExit) addLockToElement();
    }
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(delay), funcCallAction, NULL));
}

//-------------------All elements below this are used internally-----------------

void HQSceneElement::addLockToElement()
{
    auto lockImage = Sprite::create("res/hqscene/locked.png");
    lockImage->setPosition(baseLayer->getContentSize() / 2);
    //lockImage->setScale(baseLayer->getContentSize().width / 445);
    baseLayer->addChild(lockImage);
}

Size HQSceneElement::getSizeOfLayerWithGap()
{
    float gapSize = 40.0f;
    return Size(baseLayer->getContentSize().width + gapSize, baseLayer->getContentSize().height + gapSize);
}

void HQSceneElement::addImageToBaseLayer(std::string url)
{
    ImageDownloader *imageDownloader = ImageDownloader::create();
    imageDownloader->initWithURLAndSize(url, Size(baseLayer->getContentSize().width - 20, baseLayer->getContentSize().height - 20));
    imageDownloader->setPosition(baseLayer->getContentSize() / 2);
    baseLayer->addChild(imageDownloader);
}

void HQSceneElement::addGradientToBottom(std::string category)
{
    Color3B gradientColour;
    gradientColour.r = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(category).r;
    gradientColour.g = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(category).g;
    gradientColour.b = ConfigStorage::getInstance()->getBaseColourForContentItemInCategory(category).b;
    
    float set;
    if(baseLayer->getContentSize().height > 500)
         set = baseLayer->getContentSize().height;
    else
         set = baseLayer->getContentSize().height;
    
    auto gradient = Sprite::create("res/hqscene/gradient_overlay.png");
    gradient->setPosition(baseLayer->getContentSize().width / 2, gradient->getContentSize().height / 2);
    gradient->setScaleX(baseLayer->getContentSize().width / gradient->getContentSize().width);
    gradient->setColor(gradientColour);
    baseLayer->addChild(gradient);
}

Sprite* HQSceneElement::addIconToImage(std::string category)
{
    if(ConfigStorage::getInstance()->getIconImagesForContentItemInCategory(category) == "") return nullptr; //there is chance that there is no icon given for the given category.
        
    auto icon = Sprite::create(ConfigStorage::getInstance()->getIconImagesForContentItemInCategory(category));
    icon->setAnchorPoint(Vec2(0.5, 0.5));
    icon->setScale(1.2);
    icon->setPosition(icon->getContentSize().height*icon->getScale() ,icon->getContentSize().height*0.85*icon->getScale());
    baseLayer->addChild(icon);
    
    return icon;
}

void HQSceneElement::addLabelsToImage(std::map<std::string, std::string>itemData, Sprite* nextToIcon)
{
    float labelsXPosition = nextToIcon->getPositionX() + (nextToIcon->getContentSize().height);
    
    auto descriptionLabel = createLabelHubElementDescription(itemData["description"]);
    descriptionLabel->setAnchorPoint(Vec2(0.0f, 0.2f));
    descriptionLabel->setPosition(labelsXPosition,nextToIcon->getPositionY() - nextToIcon->getContentSize().height/2 * nextToIcon->getScale());
    reduceLabelTextToFitWidth(descriptionLabel,baseLayer->getContentSize().width - labelsXPosition - (nextToIcon->getContentSize().height/2));
    baseLayer->addChild(descriptionLabel);
    
    auto titleLabel = createLabelHubElementTitle(itemData["title"]);
    titleLabel->setAnchorPoint(Vec2(0.0f, 0.8f));
    titleLabel->setPosition(labelsXPosition,nextToIcon->getPositionY() + nextToIcon->getContentSize().height/2* nextToIcon->getScale());
    reduceLabelTextToFitWidth(titleLabel,baseLayer->getContentSize().width - labelsXPosition - (nextToIcon->getContentSize().height/2));
    baseLayer->addChild(titleLabel);
}

void HQSceneElement::addTouchOverlayToElement()
{
    overlayWhenTouched = LayerColor::create(Color4B(baseLayer->getColor().r, baseLayer->getColor().g, baseLayer->getColor().b, 0), baseLayer->getContentSize().width, baseLayer->getContentSize().height);
    baseLayer->addChild(overlayWhenTouched);
}

void HQSceneElement::resizeSceneElement(cocos2d::Vec2 shape, std::string category)
{
    Size defaultSize = ConfigStorage::getInstance()->getSizeForContentItemInCategory(category);
    Size layerSize = Size(defaultSize.width * shape.x, defaultSize.height * shape.y);
    
    this->setContentSize(layerSize);
}

void HQSceneElement::createColourLayer(std::string category, float delay)
{
    ConfigStorage* configStorage = ConfigStorage::getInstance();
    Color4B colour = configStorage->getBaseColourForContentItemInCategory(category);
    Size size = Size(this->getContentSize().width - 20, this->getContentSize().height - 20);
    
    baseLayer = LayerColor::create(colour, size.width, size.height);
    baseLayer->setPosition(10, 10);
    baseLayer->setOpacity(0);
    
    this->addChild(baseLayer);
    
    baseLayer->runAction(Sequence::create(DelayTime::create(delay), FadeTo::create(0.1, colour.a), NULL));
}

void HQSceneElement::addListenerToElement(std::string uri, std::string contentId, std::string category, std::string title, std::string description, std::string type, bool preview)
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
            overlayWhenTouched->setOpacity(150);
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
            overlayWhenTouched->setOpacity(0);
        }
        
        return true;
    };
    
    listener->onTouchEnded = [=](Touch *touch, Event *event)
    {
        if(iamtouched)
        {
            overlayWhenTouched->setOpacity(0);
            
            if(Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("contentLayer")->getNumberOfRunningActions() > 0) return false;
            
            AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
            iamtouched = false;
            CCLOG("Action to come: %s", uri.c_str());
            
            if(!preview)
            {
                AnalyticsSingleton::getInstance()->openContentEvent(title, description, type, contentId);
                startUpElementDependingOnType(uri, contentId, category);
            }
            else
            {
                CCLOG("MixPanel: %s, %s, %s", title.c_str(),description.c_str(),category.c_str());
                AnalyticsSingleton::getInstance()->previewContentClickedEvent(title,description,type);
                MessageBox::createPreviewLoginSignupMessageBox();
                return true;
            }
            
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), baseLayer);
}

void HQSceneElement::startUpElementDependingOnType(std::string uri, std::string contentId, std::string category)
{
    
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

void HQSceneElement::onExitTransitionDidStart()
{
    aboutToExit = true;
    this->stopAllActions();
    this->cleanup();
}
