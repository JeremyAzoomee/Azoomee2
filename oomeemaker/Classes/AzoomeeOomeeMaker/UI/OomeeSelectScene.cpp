//
//  OomeeSelectScene.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 21/02/2018.
//

#include "OomeeSelectScene.h"
#include "OomeeMakerScene.h"
#include "../DataObjects/OomeeMakerDataHandler.h"
#include <AzoomeeCommon/Utils/DirectorySearcher.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/UI/ElectricDreamsDecoration.h>

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

void OomeeSelectScene::newOomee()
{
    OomeeMakerScene* makerScene = OomeeMakerScene::create();
    
    // use func in timefunctions.h when merge in master
    std::string saveFileName;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    
    std::ostringstream oss;
    oss << tm.tm_mday << tm.tm_mon << tm.tm_year << tm.tm_hour << tm.tm_min << tm.tm_sec;
    auto fileNameStr = oss.str();
    
    makerScene->setFilename(fileNameStr);
    Director::getInstance()->replaceScene(makerScene);
}

bool OomeeSelectScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    addSideWiresToScreen(this, 0.2, 1.0f);
    
    _contentLayer = Layer::create();
    _contentLayer->setContentSize(Director::getInstance()->getVisibleSize());
    _contentLayer->setPosition(Director::getInstance()->getVisibleOrigin());
    this->addChild(_contentLayer);
   
    _oomeeCarousel = OomeeCarousel::create();
    _oomeeCarousel->setButtonDelegate(this);
    _oomeeCarousel->setContentSize(_contentLayer->getContentSize());
    _oomeeCarousel->setPosition(_contentLayer->getContentSize() / 2);
    setCarouselData();
    _contentLayer->addChild(_oomeeCarousel);
    
    ui::Button* exitButton = ui::Button::create();
    exitButton->loadTextureNormal("res/oomeeMaker/close_button.png");
    exitButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    exitButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    exitButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(delegate)
        {
            delegate->onOomeeMakerNavigationBack();
        }
        else
        {
            exit(EXIT_SUCCESS);
        }
    });
    _contentLayer->addChild(exitButton);
    
    _newOomeeButton = ui::Button::create();
    _newOomeeButton->loadTextureNormal("res/oomeeMaker/new_oomee_button.png");
    _newOomeeButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _newOomeeButton->setNormalizedPosition(Vec2(0.5,0.15));
    _newOomeeButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        newOomee();
    });
    _contentLayer->addChild(_newOomeeButton);
    
    return true;
}

void OomeeSelectScene::onEnter()
{
    Super::onEnter();
}

void OomeeSelectScene::onEnterTransitionDidFinish()
{
    Super::onEnterTransitionDidFinish();
    this->getScheduler()->schedule([](float deltaT){
        OomeeMakerDataHandler::getInstance()->getConfigFilesIfNeeded();
    }, this, 0, 0, 0.5, false, "getFiles");
    
}

void OomeeSelectScene::setCarouselData()
{
    if(!_oomeeCarousel)
    {
        return;
    }
    
    const std::string& fileExtention = ".png";
    const std::vector<std::string>& createdOomeeFiles = DirectorySearcher::getInstance()->getFilesInDirectoryWithExtention(OomeeMakerDataHandler::getInstance()->getFullSaveDir(), fileExtention);
    std::vector<std::string> trimmedFilenames;
    for(std::string filename : createdOomeeFiles)
    {
        trimmedFilenames.push_back(filename.substr(0,filename.length() - fileExtention.size()));
    }
    _oomeeCarousel->setVisibleRange(MIN(MAX(1,ceil(trimmedFilenames.size() * 0.75f)), 6));
    _oomeeCarousel->setOomeeData(trimmedFilenames);
}

// delegate functions

void OomeeSelectScene::editOomee(const std::string& oomeeFileName)
{
    OomeeMakerScene* makerScene = OomeeMakerScene::create();
    makerScene->setFilename(oomeeFileName);
    Director::getInstance()->replaceScene(makerScene);
}

void OomeeSelectScene::deleteOomee(const std::string &oomeeFilename)
{
    if(OomeeMakerDataHandler::getInstance()->deleteOomee(oomeeFilename))
    {
        setCarouselData();
        this->getScheduler()->schedule([this](float deltaT){
            _oomeeCarousel->centerButtons();
        }, this, 0, 0, 1.0, 0, "centerButtons");
    }
}

void OomeeSelectScene::makeAvatar(const std::string &oomeeFilename)
{
    if(delegate)
    {
        delegate->onOomeeMakerUpdateAvatar(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + oomeeFilename + ".png");
    }
}

void OomeeSelectScene::shareOomee(const std::string &oomeeFilename)
{
    if(delegate)
    {
        delegate->onOomeeMakerShareOomee(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + oomeeFilename + ".png");
    }
}

NS_AZOOMEE_OM_END
