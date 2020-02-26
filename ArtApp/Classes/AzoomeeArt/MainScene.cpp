#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "AzoomeeArtApp.h"
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "ModalMessages.h"
#include <AzoomeeCommon/Utils/TimeUtils.h>
#include <AzoomeeCommon/Utils/DirUtil.h>
#include <AzoomeeCommon/Data/AppConfig.h>

#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

USING_NS_CC;

USING_NS_TZ

NS_AZ_ART_BEGIN

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
	
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
	layer->setName("ArtAppMainScene");
    layer->addBackButton();
    layer->addShareButton();
    const std::string& fileNameStr = TimeUtils::getTimeStringForFileName();
    const std::string& saveFileName = AppConfig::kArtCacheFolder + ChildManager::getInstance()->getLoggedInChild()->getId() + "/" + fileNameStr + ".png";
    layer->_fileName = DirUtil::getCachesPath() + "/" + saveFileName;
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
	layer->setName("ArtAppMainScene");
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
    _backButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            backButtonCallBack();
        }
    });
    this->addChild(_backButton,1);
}

void MainScene::addShareButton()
{
    _shareButton = ui::Button::create();
    _shareButton->setPosition(_backButton->getPosition() - Vec2(0, _backButton->getContentSize().height * 1.33f));
    _shareButton->setAnchorPoint(Vec2(0.5,1));
    _shareButton->loadTextureNormal(kArtAppAssetLoc + "share.png");
    _shareButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            shareButtonCallBack();
        }
    });
	_shareButton->setEnabled(!delegate->isOffline());
    this->addChild(_shareButton,1);
}

void MainScene::backButtonCallBack()
{
    AnalyticsSingleton::getInstance()->contentItemClosedEvent();
    
    if(_drawingCanvas->_actionCounter > 0)
    {
        ArtAppConfirmCancelMessageBox* messageBox = ArtAppConfirmCancelMessageBox::createWithParams(_("Save?"), "res/buttons/confirm_tick_2.png", "res/buttons/confirm_x_2.png", Color3B::BLACK, Color4B::WHITE);
        messageBox->setOnConfirmCallback([this](MessagePopupBase *pSender){
            ModalMessages::getInstance()->startSaving();
            
            const std::string scheduleKey = "saveAndExit";
            Director::getInstance()->getScheduler()->schedule([&](float dt){
                this->saveFileAndExit();
            }, this, 0.5, 0, 0, false, scheduleKey);
            pSender->removeFromParent();
        });
        messageBox->setOnCancelCallback([this](MessagePopupBase *pSender){
            delegate->onArtAppNavigationBack();
            pSender->removeFromParent();
        });
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
	if(_drawingCanvas->_actionCounter > 0 || !FileUtils::getInstance()->isFileExist(_fileName))
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
        const std::string& fileNameStr = TimeUtils::getTimeStringForFileName();
        
        saveFileName = AppConfig::kArtCacheFolder + TZ::ChildManager::getInstance()->getLoggedInChild()->getId() + "/" + fileNameStr + ".png";
        this->_fileName = DirUtil::getCachesPath() + "/" + saveFileName;
    }
    else
    {
        const std::string& truncatedPath = this->_fileName.substr(this->_fileName.find(AppConfig::kArtCacheFolder));
        saveFileName = truncatedPath;
        
    }
    
    _drawingCanvas->saveImage(saveFileName);
}

void MainScene::reloadRenderTextureObject()
{
	_drawingCanvas->reloadRenderTextureObject();
}

NS_AZ_ART_END
