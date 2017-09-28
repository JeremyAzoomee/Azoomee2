#include "ImageContainer.h"
#include "WebViewSelector.h"
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>
#include "HQDataParser.h"
#include "HQDataProvider.h"
#include "GameDataManager.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "NavigationLayer.h"
#include "PreviewLoginSignupMessageBox.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "HQScene.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include "RoutePaymentSingleton.h"
#include "IAPUpsaleLayer.h"
#include "VideoPlaylistManager.h"
#include "DynamicNodeHandler.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN


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
    
    std::string type = elementProperties["type"];
    
    Color4B colour4 = ConfigStorage::getInstance()->getColourForElementType(type);
    Color3B colour3 = Color3B(colour4.r, colour4.g, colour4.b);
    
    createBgLayer(elementProperties, scale, startDelay, position);
    
    addImageToLayer(HQDataProvider::getInstance()->getImageUrlForItem(elementProperties["id"], Vec2(1,1)), type, startDelay);
    addGradientToBottom(colour3, startDelay);
    addIconToImage(elementProperties["type"], startDelay);
    addLabelToImage(elementProperties["title"], startDelay);
    

    if(elementProperties["entitled"] == "false")
        addLockToImageContainer(elementProperties["type"], startDelay);
        
    addReponseLayerToImage(elementProperties, scale);
    addListenerToContainer(bgLayer, colour4.a, elementProperties, RoutePaymentSingleton::getInstance()->showIAPContent());
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
    bgLayer->setPosition(origin.x+position.x-bgLayer->getContentSize().width/2*bgLayer->getScale(),origin.y+position.y-bgLayer->getContentSize().height/2*bgLayer->getScale());
    bgLayer->setScale(0.1);
    bgLayer->setOpacity(0);
    this->addChild(bgLayer);
    
    scaleFactor = bgLayer->getContentSize().width / 445;
    
    if(startDelay == 0)
        bgLayer->runAction(Sequence::create(DelayTime::create(startDelay), FadeTo::create(0, colour.a), DelayTime::create(appearPause), FadeOut::create(0), DelayTime::create(appearPause), FadeTo::create(0, colour.a), DelayTime::create(interTime), EaseElasticOut::create(ScaleTo::create(scaleTime, 1.0)), NULL));
    else
    {
        auto audioCallback = CallFunc::create(CC_CALLBACK_0(ImageContainer::startAudio, this, HQ_BIG_SQUARES_APPEAR_AUDIO_EFFECT));
        
        bgLayer->runAction(Sequence::create(DelayTime::create(startDelay), FadeTo::create(0, colour.a), DelayTime::create(appearPause), FadeOut::create(0), DelayTime::create(appearPause), FadeTo::create(0, colour.a), DelayTime::create(interTime), audioCallback, EaseElasticOut::create(ScaleTo::create(scaleTime, 1.0)), NULL));
        
    }
}

void ImageContainer::startAudio(std::string audioName)
{
    AudioMixer::getInstance()->playEffect(audioName);
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

void ImageContainer::addListenerToContainer(cocos2d::Node *addTo, int maxOpacity, std::map<std::string, std::string> elementProperties, bool IAPEnabled)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
    {
        auto runningScene = Director::getInstance()->getRunningScene();
        
        if(runningScene->getChildByName("baseLayer") == NULL || runningScene->getChildByName("baseLayer")->getChildByName("contentLayer") == NULL || runningScene->getChildByName("baseLayer")->getChildByName("contentLayer")->getNumberOfRunningActions() > 0)
        {
            return false;
        }
        
        auto target = static_cast<Node*>(event->getCurrentTarget());
        
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0,0,s.width, s.height);
        
        if(rect.containsPoint(locationInNode))
        {
            if(elementProperties.at("entitled") == "false")
            {
                AnalyticsSingleton::getInstance()->contentItemSelectedEvent(elementProperties.at("title"), elementProperties.at("description"), elementProperties.at("type"), elementProperties.at("id"), -1, -1, "1,1");
                AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
                
                DynamicNodeHandler::getInstance()->createDynamicNodeByGroupId("upgrade");
            }
            else
            {
                AudioMixer::getInstance()->playEffect(HQ_ELEMENT_SELECTED_AUDIO_EFFECT);
                
                AnalyticsSingleton::getInstance()->contentItemSelectedEvent(elementProperties.at("title"), elementProperties.at("description"), elementProperties.at("type"), elementProperties.at("id"), -1, -1, "1,1");
                
                target->getChildByName("responseLayer")->runAction(Sequence::create(FadeTo::create(0, maxOpacity), DelayTime::create(0.1), FadeTo::create(0, 0), DelayTime::create(0.1), FadeTo::create(0, maxOpacity), FadeTo::create(2, 0), NULL));
                
                if(HQDataProvider::getInstance()->getTypeForSpecificItem("HOME", elementProperties.at("id")) == "GAME")
                {
                    GameDataManager::getInstance()->startProcessingGame(elementProperties);
                }
                else if((HQDataProvider::getInstance()->getTypeForSpecificItem("HOME", elementProperties.at("id")) == "VIDEO")||(HQDataProvider::getInstance()->getTypeForSpecificItem("HOME", elementProperties.at("id")) == "AUDIO"))
                {
                    VideoPlaylistManager::getInstance()->clearPlaylist();
                    auto webViewSelector = WebViewSelector::create();
                    webViewSelector->loadWebView(elementProperties.at("uri").c_str(),Orientation::Landscape);
                }
                else if(HQDataProvider::getInstance()->getTypeForSpecificItem("HOME", elementProperties.at("id")) == "GROUP")
                {
                    NavigationLayer *navigationLayer = (NavigationLayer *)Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("NavigationLayer");
                    navigationLayer->startLoadingGroupHQ(elementProperties.at("uri"));
                    
                    auto funcCallAction = CallFunc::create([=](){
                        HQDataProvider::getInstance()->getDataForGroupHQ(elementProperties.at("uri"));
                        HQHistoryManager::getInstance()->setGroupHQSourceId(elementProperties.at("id"));
                    });
                    
                    this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
                }
                else if(HQDataProvider::getInstance()->getTypeForSpecificItem("HOME", elementProperties.at("id")) == "AUDIOGROUP")
                {
                    NavigationLayer *navigationLayer = (NavigationLayer *)Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("NavigationLayer");
                    navigationLayer->startLoadingGroupHQ(elementProperties.at("uri"));
                    
                    auto funcCallAction2 = CallFunc::create([=](){
                        HQDataProvider::getInstance()->getDataForGroupHQ(elementProperties.at("uri"));
                        HQHistoryManager::getInstance()->setGroupHQSourceId(elementProperties.at("id"));
                    });
                    
                    this->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction2, NULL));
                }
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

void ImageContainer::addLockToImageContainer(const std::string& elementType, float startDelay)
{
    Color4B overlayColour = Style::Color_4B::semiTransparentOverlay;
    Layer* lockedOverlay = LayerColor::create(Color4B(overlayColour.r, overlayColour.g, overlayColour.b, overlayColour.a), bgLayer->getContentSize().width, bgLayer->getContentSize().height + 10);
    lockedOverlay->setPosition(0,-10);
    bgLayer->addChild(lockedOverlay);
    
    auto lockImage = Sprite::create(HQDataProvider::kLockFiles.at(elementType));
    lockImage->setPosition(bgLayer->getContentSize().width, 0);
    lockImage->setAnchorPoint(Vec2(1,0));
    bgLayer->addChild(lockImage);
    
    lockImage->runAction(Sequence::create(DelayTime::create(startDelay), FadeIn::create(0), DelayTime::create(appearPause), FadeOut::create(0), DelayTime::create(appearPause), FadeIn::create(0), NULL));
}

void ImageContainer::addImageToLayer(std::string url,std::string type, float startDelay)
{
    RemoteImageSprite *imageDownloader = RemoteImageSprite::create();
    imageDownloader->initWithURLAndSize(url, type, Size(bgLayer->getContentSize().width - 20, bgLayer->getContentSize().height - 20), cocos2d::Vec2(1,1));
    imageDownloader->setPosition(bgLayer->getContentSize() / 2);
    imageDownloader->setOpacity(0);
    bgLayer->addChild(imageDownloader);
    
    imageDownloader->runAction(Sequence::create(DelayTime::create(startDelay), FadeIn::create(0), DelayTime::create(appearPause), FadeOut::create(0), DelayTime::create(appearPause), FadeIn::create(0), NULL));
}

void ImageContainer::addGradientToBottom(Color3B colour, float startDelay)
{
    auto gradient = Sprite::create("res/hqscene/gradient_overlay.png");
    gradient->setPosition(bgLayer->getContentSize().width / 2, gradient->getContentSize().height / 2* (scaleFactor*.7));
    gradient->setScaleX(bgLayer->getContentSize().width / gradient->getContentSize().width);
    gradient->setScaleY(scaleFactor*.85);
    gradient->setColor(colour);
    gradient->setOpacity(0);
    bgLayer->addChild(gradient);
    
    gradient->runAction(Sequence::create(DelayTime::create(startDelay), FadeIn::create(0), DelayTime::create(appearPause), FadeOut::create(0), DelayTime::create(appearPause), FadeIn::create(0), NULL));
}

void ImageContainer::addIconToImage(std::string type, float startDelay)
{
    icon = Sprite::create(StringUtils::format("res/hqscene/icon_%s.png", ConfigStorage::getInstance()->getIconNameForCategory(type).c_str()));
    icon->setScale(scaleFactor);
    icon->setPosition(icon->getContentSize().height *scaleFactor, icon->getContentSize().height*.6 *scaleFactor);
    icon->setOpacity(0);
    bgLayer->addChild(icon);
    
    icon->runAction(Sequence::create(DelayTime::create(startDelay), FadeIn::create(0), DelayTime::create(appearPause), FadeOut::create(0), DelayTime::create(appearPause), FadeIn::create(0), NULL));
}

void ImageContainer::addLabelToImage(std::string name, float startDelay)
{
    auto label = createLabelHubContentTitle(name);
    label->setAnchorPoint(Vec2(0,0.6));
    label->setOpacity(0);
    label->setPosition((icon->getContentSize().width + icon->getContentSize().height *.75)  * scaleFactor, icon->getPositionY());
    reduceLabelTextToFitWidth(label,bgLayer->getContentSize().width - ((icon->getContentSize().width + icon->getContentSize().height)  * scaleFactor));
    bgLayer->addChild(label);
    
    label->runAction(Sequence::create(DelayTime::create(startDelay), FadeIn::create(0), DelayTime::create(appearPause), FadeOut::create(0), DelayTime::create(appearPause), FadeIn::create(0), NULL));
}

NS_AZOOMEE_END
