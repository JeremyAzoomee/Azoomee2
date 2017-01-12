#include "IntroVideoScene.h"
#include "ui/UIVideoPlayer.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocos2d::experimental::ui;

Scene* IntroVideoScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = IntroVideoScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool IntroVideoScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
    videoPlayer->setContentSize(visibleSize);
    videoPlayer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    videoPlayer->setPosition(visibleSize / 2);
    videoPlayer->setFileName("res/TestVideo.mp4");
    addChild(videoPlayer);
    videoPlayer->play();
    
    return true;
}


