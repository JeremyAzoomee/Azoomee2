#include "ImageContainer.h"
#include "SimpleAudioEngine.h"
#include "WebViewSelector.h"
#include "imageDownloader.h"
#include "HQDataParser.h"
#include "HQDataProvider.h"
#include "GameDataManager.h"
#include "ConfigStorage.h"
#include "NavigationLayer.h"
#include "ModalMessages.h"
#include "ChildDataProvider.h"
#include "HQHistoryManager.h"

USING_NS_CC;

Scene* ImageContainer::createScene()
{
    auto scene = Scene::create();
    auto layer = ImageContainer::create();
    scene->addChild(layer);

    return scene;
}

bool ImageContainer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void ImageContainer::createContainer(std::map<std::string, std::string> elementProperties, float scale, float startDelay, Point position)
{
    if(HQHistoryManager::getInstance()->noHistory())
    {
        appearPause = 0.1;
        interTime = 1.0;
        scaleTime = 0.5;
    }
    else
    {
        appearPause = 0;
        interTime = 0;
        scaleTime = 0;
    }
    
    std::string displayType = elementProperties["type"];
    if(elementProperties["type"] == "GROUP") displayType = "VIDEO";
    if(elementProperties["type"] == "AUDIOGROUP") displayType = "LISTEN";
    
    Color4B colour4 = ConfigStorage::getInstance()->getColourForElementType(displayType);
    Color3B colour3 = Color3B(colour4.r, colour4.g, colour4.b);
    
    createBgLayer(elementProperties, scale, startDelay, position);
    addImageToLayer(HQDataProvider::getInstance()->getImageUrlForItem(elementProperties["id"], Vec2(1,1)), startDelay);
    addGradientToBottom(colour3, startDelay);
    addIconToImage(displayType, startDelay);
    addLabelToImage(elementProperties["title"], startDelay);
    
    if(elementProperties["entitled"] == "false")
    {
        addLockToImageContainer(startDelay);
        
        if(!ChildDataProvider::getInstance()->getIsChildLoggedIn())
        {
            addPreviewListenerToContainer(bgLayer);
        }
    }
    else
    {
        addReponseLayerToImage(elementProperties, scale);
        addListenerToContainer(bgLayer, colour4.a, elementProperties["uri"], elementProperties["id"]);
    }
}

//-----------------------------------------------------All methods below are called internally.---------------------------------------------------

void ImageContainer::createBgLayer(std::map<std::string, std::string> elementProperties, float scale, float startDelay, Point position)
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size baseContentSize = Size(445, 339);
    Size containerSize = baseContentSize * scale;
    
    Color4B colour = ConfigStorage::getInstance()->getColourForElementType(elementProperties["type"]);
    
    bgLayer = LayerColor::create(colour, containerSize.width + 20, containerSize.height + 20);
    bgLayer->setAnchorPoint(Vec2(0.5, 0.5));
    bgLayer->setPosition(Point(position.x + origin.x, position.y + origin.y));
    bgLayer->setScale(0.1);
    bgLayer->setOpacity(0);
    this->addChild(bgLayer);
    
    bgLayer->runAction(Sequence::create(DelayTime::create(startDelay), FadeTo::create(0, colour.a), DelayTime::create(appearPause), FadeOut::create(0), DelayTime::create(appearPause), FadeTo::create(0, colour.a), DelayTime::create(interTime), EaseElasticOut::create(ScaleTo::create(scaleTime, 1.0)), NULL));
}

void ImageContainer::addReponseLayerToImage(std::map<std::string, std::string> elementProperties, float scale)
{
    Size baseContentSize = Size(445, 339);
    Size containerSize = baseContentSize * scale;
    
    Color4B colour = ConfigStorage::getInstance()->getColourForElementType(elementProperties["type"]);
    
    auto responseLayer = LayerColor::create(colour, containerSize.width, containerSize.height);
    responseLayer->setAnchorPoint(Point(0.5, 0.5));
    responseLayer->setPosition(10,10);
    responseLayer->setOpacity(0);
    responseLayer->setName("responseLayer");
    bgLayer->addChild(responseLayer);
}

void ImageContainer::addListenerToContainer(cocos2d::Node *addTo, int maxOpacity, std::string uri, std::string contentId)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/audio/boot.mp3");
            
            target->getChildByName("responseLayer")->runAction(Sequence::create(FadeTo::create(0, maxOpacity), DelayTime::create(0.1), FadeTo::create(0, 0), DelayTime::create(0.1), FadeTo::create(0, maxOpacity), FadeTo::create(2, 0), NULL));
            
            if(HQDataProvider::getInstance()->getTypeForSpecificItem("HOME", contentId) == "GAME")
            {
                GameDataManager::getInstance()->startProcessingGame(uri, contentId);
            }
            else if((HQDataProvider::getInstance()->getTypeForSpecificItem("HOME", contentId) == "VIDEO")||(HQDataProvider::getInstance()->getTypeForSpecificItem("HOME", contentId) == "AUDIO"))
            {
                auto webViewSelector = WebViewSelector::create();
                webViewSelector->loadWebView(uri.c_str());
            }
            else if(HQDataProvider::getInstance()->getTypeForSpecificItem("HOME", contentId) == "GROUP")
            {
                NavigationLayer *navigationLayer = (NavigationLayer *)Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("NavigationLayer");
                navigationLayer->startLoadingGroupHQ(uri);
                
                auto funcCallAction = CallFunc::create([=](){
                    HQDataProvider::getInstance()->getDataForGroupHQ(uri);
                });
                
                this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
            }
            else if(HQDataProvider::getInstance()->getTypeForSpecificItem("HOME", contentId) == "AUDIOGROUP")
            {
                NavigationLayer *navigationLayer = (NavigationLayer *)Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("NavigationLayer");
                navigationLayer->startLoadingGroupHQ(uri);
                
                auto funcCallAction2 = CallFunc::create([=](){
                    HQDataProvider::getInstance()->getDataForGroupHQ(uri);
                });
                
                this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction2, NULL));
            }
            
            return true;
        }
        
        return false;
    };
    
    listener->onTouchEnded = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        target->getChildByName("responseLayer")->stopAllActions();
        target->getChildByName("responseLayer")->setOpacity(0);
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), addTo);
}

void ImageContainer::addPreviewListenerToContainer(cocos2d::Node *addTo)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            ModalMessages::getInstance()->createPreviewLoginSignupMessageBox();
            return true;
        }
        
        return false;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), addTo);
}

void ImageContainer::addLockToImageContainer(float startDelay)
{
    auto lockImage = Sprite::create("res/hqscene/locked.png");
    lockImage->setPosition(bgLayer->getContentSize() / 2);
    lockImage->setScale(bgLayer->getContentSize().width / 445);
    lockImage->setOpacity(0);
    bgLayer->addChild(lockImage);
    
    lockImage->runAction(Sequence::create(DelayTime::create(startDelay), FadeIn::create(0), DelayTime::create(appearPause), FadeOut::create(0), DelayTime::create(appearPause), FadeIn::create(0), NULL));
}

void ImageContainer::addImageToLayer(std::string url, float startDelay)
{
    ImageDownloader *imageDownloader = ImageDownloader::create();
    imageDownloader->initWithURLAndSize(url, Size(bgLayer->getContentSize().width - 20, bgLayer->getContentSize().height - 20));
    imageDownloader->setPosition(bgLayer->getContentSize() / 2);
    imageDownloader->setOpacity(0);
    bgLayer->addChild(imageDownloader);
    
    imageDownloader->runAction(Sequence::create(DelayTime::create(startDelay), FadeIn::create(0), DelayTime::create(appearPause), FadeOut::create(0), DelayTime::create(appearPause), FadeIn::create(0), NULL));
}

void ImageContainer::addGradientToBottom(Color3B colour, float startDelay)
{
    auto gradient = Sprite::create("res/hqscene/gradient_overlay.png");
    gradient->setPosition(bgLayer->getContentSize().width / 2, gradient->getContentSize().height / 2);
    gradient->setScaleX(bgLayer->getContentSize().width / gradient->getContentSize().width);
    gradient->setColor(colour);
    gradient->setOpacity(0);
    bgLayer->addChild(gradient);
    
    gradient->runAction(Sequence::create(DelayTime::create(startDelay), FadeIn::create(0), DelayTime::create(appearPause), FadeOut::create(0), DelayTime::create(appearPause), FadeIn::create(0), NULL));
}

void ImageContainer::addIconToImage(std::string type, float startDelay)
{
    auto icon = Sprite::create(StringUtils::format("res/hqscene/icon_%s.png", ConfigStorage::getInstance()->getIconNameForCategory(type).c_str()));
    icon->setScale(bgLayer->getContentSize().width / 445);
    icon->setPosition(30 * icon->getScale() + icon->getBoundingBox().size.width / 2, 30 * icon->getScale() + icon->getBoundingBox().size.height / 2);
    icon->setOpacity(0);
    bgLayer->addChild(icon);
    
    icon->runAction(Sequence::create(DelayTime::create(startDelay), FadeIn::create(0), DelayTime::create(appearPause), FadeOut::create(0), DelayTime::create(appearPause), FadeIn::create(0), NULL));
}

void ImageContainer::addLabelToImage(std::string name, float startDelay)
{
    if(name.length() > 15)
    {
        name = name.substr(0, 15);
        name = name + "...";
    }
    
    auto label = Label::createWithTTF(name, "fonts/arial.ttf", 30);
    label->setColor(Color3B(255,255,255));
    label->setAnchorPoint(Vec2(0,0.5));
    label->setOpacity(0);
    label->setPosition(bgLayer->getContentSize().width * 0.25, bgLayer->getContentSize().height * 0.1 + label->getContentSize().height / 2);
    bgLayer->addChild(label);
    
    label->runAction(Sequence::create(DelayTime::create(startDelay), FadeIn::create(0), DelayTime::create(appearPause), FadeOut::create(0), DelayTime::create(appearPause), FadeIn::create(0), NULL));
}
