#include "InterSceneLoader.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* InterSceneLoader::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = InterSceneLoader::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void InterSceneLoader::layersStopAllActions()
{
    part1->stopAllActions();
    part2->stopAllActions();
}

void InterSceneLoader::startLoading()
{
    layersStopAllActions();
    part1->runAction(EaseInOut::create( MoveTo::create(0.5, Vec2(origin.x, origin.y)), 2));
    part2->runAction(EaseInOut::create (MoveTo::create(0.5, Vec2(visibleSize.width / 2 + origin.x, origin.y)), 2));
    
    scheduleOnce(schedule_selector(InterSceneLoader::stopLoading), 1);
}

void InterSceneLoader::stopLoading(float dt)
{
    layersStopAllActions();
    part1->runAction(EaseInOut::create( MoveTo::create(0.5, Vec2(-visibleSize.width / 2 + origin.x, origin.y)), 2));
    part2->runAction(EaseInOut::create (MoveTo::create(0.5, Vec2(visibleSize.width + origin.x, origin.y)), 2));
}

LayerColor* InterSceneLoader::addColourLayerToScreen(Color4B colour, Size size, Point position, std::string imageName)
{
    LayerColor *colourLayer = LayerColor::create(colour, size.width, size.height);
    colourLayer->setPosition(position);
    this->addChild(colourLayer);
    
    if(imageName != "")
    {
        auto imageOnLayer = Sprite::create(imageName);
        imageOnLayer->setPosition(colourLayer->getContentSize().width / 2, colourLayer->getContentSize().height / 2);
        colourLayer->addChild(imageOnLayer);
    }
    
    return colourLayer;
}

// on "init" you need to initialize your instance
bool InterSceneLoader::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->setName("interSceneLoader");
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    part1 = addColourLayerToScreen(Color4B(32,54,64,255), Size(visibleSize.width / 2, visibleSize.height), Point(-visibleSize.width / 2 + origin.x, origin.y), "res/navigation/door_left.png");
    part2 = addColourLayerToScreen(Color4B(32,54,64,255), Size(visibleSize.width / 2, visibleSize.height), Point(visibleSize.width + origin.x, origin.y), "res/navigation/door_right.png");
    
    return true;
}
