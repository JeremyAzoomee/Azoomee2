#include "ArtAppBaseScene.h"
#include "SimpleAudioEngine.h"
#include "AzoomeeArtApp.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>

#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

USING_NS_CC;

NS_AZOOMEE_AA_BEGIN

Scene* ArtAppBase::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ArtAppBase::create();
    layer->addBackButton();
    layer->fileName = "";
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

Scene* ArtAppBase::createSceneWithDrawing(std::string fileName)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ArtAppBase::create();
    layer->addBackButton();
    layer->fileName = fileName;
    layer->drawingCanvas->setBaseImage(fileName);
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ArtAppBase::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    drawingCanvas = DrawingCanvas::create();

    this->addChild(drawingCanvas);
    
    return true;
}

void ArtAppBase::addBackButton()
{
    backButton = ui::Button::create();
    backButton->setPosition(Director::getInstance()->getVisibleOrigin());
    backButton->setAnchorPoint(Vec2(0,0));
    backButton->loadTextures("res/navigation/back_new.png", "res/navigation/back_new.png");
    backButton->addClickEventListener([this](Ref* but){backButtonCallBack();});
    this->addChild(backButton,1);
}

void ArtAppBase::backButtonCallBack()
{
    std::string saveFileName;
    if(this->fileName == "")
    {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
    
        std::ostringstream oss;
        oss << std::put_time(&tm, "%d%m%Y%H%M%S");
        auto fileNameStr = oss.str();
    
        saveFileName = "artCache/" + Azoomee::ChildDataProvider::getInstance()->getLoggedInChildId() + "/" + fileNameStr + ".png";
    }
    else
    {
        std::string truncatedPath = this->fileName.substr(this->fileName.find("artCache/"));
        saveFileName = truncatedPath;
        
    }
    
    drawingCanvas->saveImage(saveFileName);
    delegate->onArtAppNavigationBack();
}

NS_AZOOMEE_AA_END
