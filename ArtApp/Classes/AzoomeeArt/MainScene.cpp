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
    layer->addShareButton();
    layer->_fileName = "";
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
    layer->addShareButton();
    layer->_fileName = fileName;
    layer->_drawingCanvas->setBaseImage(fileName);
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

    _drawingCanvas = DrawingCanvas::create();

    this->addChild(_drawingCanvas);
    
    _uiLayer = DrawingCanvasUILayer::create();
    _uiLayer->setDrawingCanvas(_drawingCanvas);
    
    this->addChild(_uiLayer);
    
    return true;
}

void MainScene::addBackButton()
{
    _backButton = ui::Button::create();
    _backButton->setPosition(Vec2(Point(Director::getInstance()->getVisibleSize().width*0.045, Director::getInstance()->getVisibleOrigin().y + Director::getInstance()->getVisibleSize().height * 0.975)));
    _backButton->setAnchorPoint(Vec2(0.5,1));
    _backButton->loadTextures(kArtAppAssetLoc + "back_button.png", kArtAppAssetLoc + "back_button.png");
    _backButton->addClickEventListener([this](Ref* but){saveFileAndExit();});
    this->addChild(_backButton,1);
}

void MainScene::addShareButton()
{
    _shareButton = ui::Button::create();
    _shareButton->setPosition(_backButton->getPosition() - Vec2(0, _backButton->getContentSize().height * 1.33f));
    _shareButton->setAnchorPoint(Vec2(0.5,1));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    _shareButton->loadTextures(kArtAppAssetLoc + "share_android.png", kArtAppAssetLoc + "share_android.png");
#else
    _shareButton->loadTextures(kArtAppAssetLoc + "share_ios.png", kArtAppAssetLoc + "share_ios.png");
#endif
    _shareButton->addClickEventListener([this](Ref* but){saveAndSendFile();});
    this->addChild(_shareButton,1);
}

void MainScene::saveFileAndExit()
{
    saveFile();
    delegate->onArtAppNavigationBack();
}

void MainScene::saveAndSendFile()
{
    saveFile();
    delegate->setFileName(_fileName);
    delegate->onArtAppShareImage();
}

void MainScene::saveFile()
{
    std::string saveFileName;
    if(this->_fileName == "")
    {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        
        
        std::ostringstream oss;
        oss << tm.tm_mday << tm.tm_mon << tm.tm_year << tm.tm_hour << tm.tm_min << tm.tm_sec;
        auto fileNameStr = oss.str();
        
        saveFileName = kArtCacheFolder + Azoomee::ChildDataProvider::getInstance()->getParentOrChildId() + "/" + fileNameStr + ".png";
        this->_fileName = FileUtils::getInstance()->getWritablePath() + "/" + saveFileName;
    }
    else
    {
        const std::string& truncatedPath = this->_fileName.substr(this->_fileName.find(kArtCacheFolder));
        saveFileName = truncatedPath;
        
    }
    
    _drawingCanvas->saveImage(saveFileName);
}

NS_AZOOMEE_AA_END
