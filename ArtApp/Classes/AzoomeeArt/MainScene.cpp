#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "AzoomeeArtApp.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Utils/TimeFunctions.h>

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
    const std::string& fileNameStr = getTimeStringForFileName();
    const std::string& saveFileName = ConfigStorage::kArtCacheFolder + Azoomee::ChildDataProvider::getInstance()->getParentOrChildId() + "/" + fileNameStr + ".png";
    layer->_fileName = FileUtils::getInstance()->getWritablePath() + "/" + saveFileName;
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
    
    _timeStampOnAppStart = std::time(0);

    _drawingCanvas = DrawingCanvas::create();

    this->addChild(_drawingCanvas);
    
    _uiLayer = DrawingCanvasUILayer::create();
    _uiLayer->setDrawingCanvas(_drawingCanvas);
    
    this->addChild(_uiLayer);
    
    return true;
}

void MainScene::onEnter()
{
    _uiLayer->setFilename(_fileName);
    Super::onEnter();
}

void MainScene::addBackButton()
{
    _backButton = ui::Button::create();
    _backButton->setPosition(Vec2(Point(Director::getInstance()->getVisibleSize().width*0.045, Director::getInstance()->getVisibleOrigin().y + Director::getInstance()->getVisibleSize().height * 0.975)));
    _backButton->setAnchorPoint(Vec2(0.5,1));
    _backButton->loadTextures(kArtAppAssetLoc + "back_button.png", kArtAppAssetLoc + "back_button.png");
    _backButton->addClickEventListener([this](Ref* but){backButtonCallBack();});
    this->addChild(_backButton,1);
}

void MainScene::addShareButton()
{
    _shareButton = ui::Button::create();
    _shareButton->setPosition(_backButton->getPosition() - Vec2(0, _backButton->getContentSize().height * 1.33f));
    _shareButton->setAnchorPoint(Vec2(0.5,1));
    _shareButton->loadTextureNormal(kArtAppAssetLoc + "share.png");
    _shareButton->addClickEventListener([this](Ref* but){shareButtonCallBack();});
	_shareButton->setEnabled(!delegate->isOffline());
    this->addChild(_shareButton,1);
}

void MainScene::backButtonCallBack()
{
    AnalyticsSingleton::getInstance()->contentItemClosedEvent();
    
    if(_drawingCanvas->_actionCounter > 0)
    {
        ConfirmCancelMessageBox* messageBox = ConfirmCancelMessageBox::createWithParams(_("Save?"), "res/buttons/confirm_tick_2.png", "res/buttons/confirm_x_2.png", Color3B::BLACK, Color4B::WHITE);
        messageBox->setDelegate(this);
        messageBox->setPosition(Director::getInstance()->getVisibleOrigin() + Vec2(Director::getInstance()->getVisibleSize().width * 0.09f/2.0f,Director::getInstance()->getVisibleSize().height * 0.175f/2.0f));
        this->addChild(messageBox,POPUP_UI_LAYER);
        
    }
    else
    {
        delegate->onArtAppNavigationBack();
    }

}

void MainScene::shareButtonCallBack()
{
    if(_drawingCanvas->_actionCounter > 0 || _fileName == "")
    {
        ModalMessages::getInstance()->startSaving();
        
        const std::string scheduleKey = "saveAndShare";
        Director::getInstance()->getScheduler()->schedule([&](float dt){
            this->saveAndSendFile();
        }, this, 0.5, 0, 0, false, scheduleKey);
    }
    else
    {
        delegate->setFileName(_fileName);
        delegate->onArtAppShareImage();
    }
}

void MainScene::saveFileAndExit()
{
    saveFile();
    delegate->setSecondsSpentInArtApp(std::time(0) - _timeStampOnAppStart);
    delegate->onArtAppNavigationBack();
}

void MainScene::saveAndSendFile()
{
    saveFile();
    delegate->setFileName(_fileName);
    ModalMessages::getInstance()->stopSaving();
    delegate->onArtAppShareImage();
}

void MainScene::saveFile()
{
    std::string saveFileName;
    if(this->_fileName == "")
    {
        const std::string& fileNameStr = getTimeStringForFileName();
        
        saveFileName = ConfigStorage::kArtCacheFolder + Azoomee::ChildDataProvider::getInstance()->getParentOrChildId() + "/" + fileNameStr + ".png";
        this->_fileName = FileUtils::getInstance()->getWritablePath() + "/" + saveFileName;
    }
    else
    {
        const std::string& truncatedPath = this->_fileName.substr(this->_fileName.find(ConfigStorage::kArtCacheFolder));
        saveFileName = truncatedPath;
        
    }
    
    _drawingCanvas->saveImage(saveFileName);
}

void MainScene::onConfirmPressed(Azoomee::ConfirmCancelMessageBox *pSender)
{
    ModalMessages::getInstance()->startSaving();
    
    const std::string scheduleKey = "saveAndExit";
    Director::getInstance()->getScheduler()->schedule([&](float dt){
        this->saveFileAndExit();
    }, this, 0.5, 0, 0, false, scheduleKey);
    
    pSender->removeFromParent();
}

void MainScene::onCancelPressed(Azoomee::ConfirmCancelMessageBox *pSender)
{
    pSender->removeFromParent();
    delegate->onArtAppNavigationBack();
}

NS_AZOOMEE_AA_END
