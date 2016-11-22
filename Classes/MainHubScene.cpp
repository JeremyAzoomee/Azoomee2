#include "MainHubScene.h"
#include "SimpleAudioEngine.h"
#include "MainHubBgElements.h"
#include "ImageContainer.h"
#include "OomeeLayer.h"

USING_NS_CC;

Scene* MainHubScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainHubScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void MainHubScene::addBackgroundCircles()
{
    auto blueCircle = Sprite::create("res/mainhub/blue_circle.png");
    blueCircle->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    blueCircle->setScale(0.125);
    blueCircle->setOpacity(0);
    this->addChild(blueCircle);
    
    blueCircle->runAction(Sequence::create(DelayTime::create(1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), FadeOut::create(3), NULL));
    
    for(int i = 0; i < 5; i++)
    {
        auto circle = Sprite::create(StringUtils::format("res/mainhub/circle_%d.png", i));
        circle->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
        circle->setScale(0);
        this->addChild(circle);
        
        circle->runAction(Sequence::create(DelayTime::create(2 + (i * 0.2)), EaseElasticOut::create(ScaleTo::create(0.5, 1.0)), NULL));
        
        int turnDirection = 1;
        if(i % 2 == 0) turnDirection = -1;
        circle->runAction(RepeatForever::create(RotateBy::create(30 + CCRANDOM_0_1() * 30, 360 * turnDirection)));
    }

}

void MainHubScene::addImageContainers()
{
    auto imageIcon = ImageContainer::create();
    imageIcon->setPosition(visibleSize / 2);
    this->addChild(imageIcon);
    
    imageIcon->createContainer("res/previewimg/video_birds.png", Color4B(248,71,89,150), 1, 5 + CCRANDOM_0_1(), Point(-1050,75));
    imageIcon->createContainer("res/previewimg/video_jamie.png", Color4B(248,71,89,150), 0.7, 5 + CCRANDOM_0_1(), Point(-700,400));
    
    imageIcon->createContainer("res/previewimg/video_lassie.png", Color4B(58,188,152,150), 0.7, 5 + CCRANDOM_0_1(), Point(-700,-700));
    imageIcon->createContainer("res/previewimg/video_moe.png", Color4B(58,188,152,150), 1, 5 + CCRANDOM_0_1(), Point(-1050,-475));
    
    imageIcon->createContainer("res/previewimg/video_birds.png", Color4B(86,177,255,150), 1, 5 + CCRANDOM_0_1(), Point(600,75));
    imageIcon->createContainer("res/previewimg/video_moe.png", Color4B(86,177,255,150), 0.7, 5 + CCRANDOM_0_1(), Point(400,400));
    
    imageIcon->createContainer("res/previewimg/video_lassie.png", Color4B(246,185,66,150), 0.7, 5 + CCRANDOM_0_1(), Point(400,-700));
    imageIcon->createContainer("res/previewimg/video_jamie.png", Color4B(246,185,66,150), 1, 5 + CCRANDOM_0_1(), Point(600,-475));
}

// on "init" you need to initialize your instance
bool MainHubScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    auto bgElements = MainHubBgElements::create();
    this->addChild(bgElements);
    
    addBackgroundCircles();
    addImageContainers();
    
    auto oomeeLayer = OomeeLayer::create();
    this->addChild(oomeeLayer);
    
    return true;
}
