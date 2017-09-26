#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "AzoomeeArtApp.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

USING_NS_CC;

NS_AZOOMEE_AA_BEGIN

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
    layer->addBackButton();
    layer->fileName = "";
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

Scene* MainScene::createSceneWithDrawing(const std::string& fileName)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
    layer->addBackButton();
    layer->fileName = fileName;
    layer->drawingCanvas->setBaseImage(fileName);
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Super::init() )
    {
        return false;
    }
    
    Director::getInstance()->purgeCachedData();

    drawingCanvas = DrawingCanvas::create();

    this->addChild(drawingCanvas);
    
    return true;
}

void MainScene::addBackButton()
{
    backButton = ui::Button::create();
    backButton->setPosition(Vec2(Point(100, Director::getInstance()->getVisibleOrigin().y + Director::getInstance()->getVisibleSize().height - 250)));
    backButton->setAnchorPoint(Vec2(0,0));
    backButton->loadTextures("res/navigation/back_new.png", "res/navigation/back_new.png");
    backButton->addClickEventListener([this](Ref* but){backButtonCallBack();});
    this->addChild(backButton,1);
}

void MainScene::backButtonCallBack()
{
    AnalyticsSingleton::getInstance()->contentItemClosedEvent();
    
    if(drawingCanvas->actionCounter > 0)
    {
        auto savingLabel = Label::createWithTTF("Saving...", Style::Font::Regular, 128);
        savingLabel->setColor(Style::Color::white);
        savingLabel->setPosition(Director::getInstance()->getVisibleOrigin() + Director::getInstance()->getVisibleSize()/2);
        savingLabel->setAnchorPoint(Vec2(0.5,0.5));
        this->addChild(savingLabel,3);
        
        auto overlay = LayerColor::create(Style::Color_4B::semiTransparentOverlay, Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height);
        overlay->setPosition(Director::getInstance()->getVisibleOrigin());
        this->addChild(overlay,2);
    
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](Touch *touch, Event *event) //Lambda callback, which is a C++ 11 feature.
        {
            return true;
        };
    
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), overlay);
        const std::string scheduleKey = "saveAndExit";
        Director::getInstance()->getScheduler()->schedule([&](float dt){
            this->saveFileAndExit();
        }, this, 0.5, 0, 0, false, scheduleKey);
    }
    else
    {
        delegate->onArtAppNavigationBack();
    }
}

void MainScene::saveFileAndExit()
{
    std::string saveFileName;
    if(this->fileName == "")
    {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        
        
        std::ostringstream oss;
        //oss << std::put_time(&tm, "%d%m%Y%H%M%S");
        oss << tm.tm_mday << tm.tm_mon << tm.tm_year << tm.tm_hour << tm.tm_min << tm.tm_sec;
        auto fileNameStr = oss.str();
        
        saveFileName = artCacheFolder + Azoomee::ChildDataProvider::getInstance()->getParentOrChildId() + "/" + fileNameStr + ".png";
    }
    else
    {
        std::string truncatedPath = this->fileName.substr(this->fileName.find(artCacheFolder));
        saveFileName = truncatedPath;
        
    }
    
    drawingCanvas->saveImage(saveFileName);
    delegate->onArtAppNavigationBack();
}

NS_AZOOMEE_AA_END
