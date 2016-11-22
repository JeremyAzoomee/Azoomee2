#include "ImageContainer.h"
#include "SimpleAudioEngine.h"

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

void ImageContainer::addListenerToContainer(cocos2d::Node *addTo, int maxOpacity)
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

void ImageContainer::createContainer(std::string imageName, cocos2d::Color4B bgColour, float scale, float startDelay, Point position)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto image = Sprite::create(imageName);
    
    
    //This is a feature: if size is set to 0, then the own size of the image will decide the container. However it is possible to make it smaller by adding sizes.
    Size containerSize = image->getContentSize() * scale;
    
    auto bgLayer = LayerColor::create(bgColour, containerSize.width + 20, containerSize.height + 20);
    bgLayer->setAnchorPoint(Vec2(0.5, 0.5));
    bgLayer->setPosition(Point(position.x + origin.x, position.y + origin.y));
    bgLayer->setScale(0.1);
    bgLayer->setOpacity(0);
    this->addChild(bgLayer);
    
    bgLayer->runAction(Sequence::create(DelayTime::create(startDelay), FadeTo::create(0, bgColour.a), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeTo::create(0, bgColour.a), DelayTime::create(1), EaseElasticOut::create(ScaleTo::create(0.5, 1.0)), NULL));
    
    
    image->setOpacity(0);
    image->setScale(scale);
    image->setPosition(bgLayer->getContentSize().width / 2, bgLayer->getContentSize(). height / 2);
    bgLayer->addChild(image);
    
    image->runAction(Sequence::create(DelayTime::create(startDelay + 2), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), NULL));
    
    //We are adding another LayerColor to the object, that can be shown when the button is pressed.
    
    auto responseLayer = LayerColor::create(bgColour, containerSize.width, containerSize.height);
    responseLayer->setAnchorPoint(Point(0.5, 0.5));
    responseLayer->setPosition(10,10);
    responseLayer->setOpacity(0);
    responseLayer->setName("responseLayer");
    bgLayer->addChild(responseLayer);
    
    //Adding touch listener to the container
    
    addListenerToContainer(bgLayer, bgColour.a);
}
