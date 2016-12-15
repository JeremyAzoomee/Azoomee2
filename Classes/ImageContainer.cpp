#include "ImageContainer.h"
#include "SimpleAudioEngine.h"
#include "WebViewSelector.h"
#include "imageDownloader.h"
#include "HQDataProvider.h"
#include "GameDataManager.h"

USING_NS_CC;

Scene* ImageContainer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ImageContainer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ImageContainer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}

void ImageContainer::addListenerToContainer(cocos2d::Node *addTo, int maxOpacity, std::string uri, std::string itemId)
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
            target->getChildByName("responseLayer")->runAction(Sequence::create(FadeTo::create(0, maxOpacity), DelayTime::create(0.1), FadeTo::create(0, 0), DelayTime::create(0.1), FadeTo::create(0, maxOpacity), FadeTo::create(2, 0), NULL));
            CCLOG("uri to open: %s", uri.c_str());
            
            if(HQDataProvider::getInstance()->getExtensionFromUri(uri) == "json")
            {
                GameDataManager::getInstance()->startProcessingGame(uri, itemId);
            }
            else
            {
                auto webViewSelector = WebViewSelector::create();
                webViewSelector->loadWebView(uri.c_str());
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

cocos2d::Color4B ImageContainer::getColourByType(std::string type)
{
    if(type == "GAME") return Color4B(58,188,152,150);
    if(type == "AUDIO") return Color4B(86,177,255,150);
    
    return Color4B(248,71,89,150); //Video for default
}

void ImageContainer::addLockToImageContainer(float startDelay)
{
    auto lockImage = Sprite::create("res/hqscene/locked.png");
    lockImage->setPosition(bgLayer->getContentSize() / 2);
    lockImage->setScale(bgLayer->getContentSize().width / 445);
    lockImage->setOpacity(0);
    bgLayer->addChild(lockImage);
    
    lockImage->runAction(Sequence::create(DelayTime::create(startDelay), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
}

void ImageContainer::addImageToLayer(std::string url)
{
    ImageDownloader *imageDownloader = ImageDownloader::create();
    imageDownloader->initWithURLAndSize(url, Size(bgLayer->getContentSize().width - 20, bgLayer->getContentSize().height - 20));
    imageDownloader->setPosition(bgLayer->getContentSize() / 2);
    bgLayer->addChild(imageDownloader);
}

void ImageContainer::createContainer(std::map<std::string, std::string> elementProperties, float scale, float startDelay, Point position)
{
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Size baseContentSize = Size(445, 339);
    Size containerSize = baseContentSize * scale;
    
    bgLayer = LayerColor::create(getColourByType(elementProperties["type"]), containerSize.width + 20, containerSize.height + 20);
    bgLayer->setAnchorPoint(Vec2(0.5, 0.5));
    bgLayer->setPosition(Point(position.x + origin.x, position.y + origin.y));
    bgLayer->setScale(0.1);
    bgLayer->setOpacity(0);
    this->addChild(bgLayer);
    
    bgLayer->runAction(Sequence::create(DelayTime::create(startDelay), FadeTo::create(0, getColourByType(elementProperties["type"]).a), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeTo::create(0, getColourByType(elementProperties["type"]).a), DelayTime::create(1), EaseElasticOut::create(ScaleTo::create(0.5, 1.0)), NULL));
    
    addImageToLayer(HQDataProvider::getInstance()->getImageUrlForItem(elementProperties["id"]));
    addGradientToBottom(Color3B(getColourByType(elementProperties["type"]).r, getColourByType(elementProperties["type"]).g, getColourByType(elementProperties["type"]).b), startDelay);
    addIconToImage(elementProperties["type"], startDelay);
    addLabelToImage(elementProperties["title"], startDelay);
    
    
    
    if(elementProperties["entitled"] == "false")
    {
        addLockToImageContainer(startDelay);
    }
    else
    {
        auto responseLayer = LayerColor::create(getColourByType(elementProperties["type"]), containerSize.width, containerSize.height);
        responseLayer->setAnchorPoint(Point(0.5, 0.5));
        responseLayer->setPosition(10,10);
        responseLayer->setOpacity(0);
        responseLayer->setName("responseLayer");
        bgLayer->addChild(responseLayer);
        
        addListenerToContainer(bgLayer, getColourByType(elementProperties["type"]).a, elementProperties["uri"], elementProperties["id"]);
    }
}

void ImageContainer::addGradientToBottom(Color3B colour, float startDelay)
{
    auto gradient = Sprite::create("res/hqscene/gradient_overlay.png");
    gradient->setPosition(bgLayer->getContentSize().width / 2, gradient->getContentSize().height / 2);
    gradient->setScaleX(bgLayer->getContentSize().width / gradient->getContentSize().width);
    gradient->setColor(colour);
    gradient->setOpacity(0);
    bgLayer->addChild(gradient);
    
    gradient->runAction(Sequence::create(DelayTime::create(startDelay), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
}

void ImageContainer::addIconToImage(std::string type, float startDelay)
{
    std::map<std::string, std::string> iconNames;
    iconNames["VIDEO"] = "watch";
    iconNames["AUDIO"] = "watch";
    iconNames["GAME"] = "play";
    
    auto icon = Sprite::create(StringUtils::format("res/hqscene/icon_%s.png", iconNames[type].c_str()));
    icon->setScale(bgLayer->getContentSize().width / 445);
    icon->setPosition(30 * icon->getScale() + icon->getBoundingBox().size.width / 2, 30 * icon->getScale() + icon->getBoundingBox().size.height / 2);
    icon->setOpacity(0);
    bgLayer->addChild(icon);
    
    icon->runAction(Sequence::create(DelayTime::create(startDelay), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
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
    
    label->runAction(Sequence::create(DelayTime::create(startDelay), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
}
