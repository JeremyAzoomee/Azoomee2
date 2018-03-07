//
//  OomeeSelectScene.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 21/02/2018.
//

#include "OomeeSelectScene.h"
#include "OomeeMakerScene.h"
#include "OomeeCarousel.h"
#include "../DataObjects/OomeeMakerDataHandler.h"
#include <AzoomeeCommon/Utils/DirectorySearcher.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

void OomeeSelectScene::editOomee(const std::string& oomeeFileName)
{
    OomeeMakerScene* makerScene = OomeeMakerScene::create();
    makerScene->setFilename(oomeeFileName);
    Director::getInstance()->replaceScene(makerScene);
}

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
        
    saveFileName = OomeeMakerDataHandler::getInstance()->getLocalSaveDir() + fileNameStr;
    
    makerScene->setFilename(saveFileName);
    Director::getInstance()->replaceScene(makerScene);
}

bool OomeeSelectScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    _contentLayer = Layer::create();
    _contentLayer->setContentSize(Director::getInstance()->getVisibleSize());
    _contentLayer->setPosition(Director::getInstance()->getVisibleOrigin());
    this->addChild(_contentLayer);
   
    const std::vector<std::string>& createdOomeeFiles = DirectorySearcher::getInstance()->getFilesInDirectoryWithExtention(OomeeMakerDataHandler::getInstance()->getFullSaveDir(), ".png");
    std::vector<std::string> trimmedFilenames;
    for(std::string filename : createdOomeeFiles)
    {
        trimmedFilenames.push_back(filename.substr(0,filename.length() - 4));
    }
    
    OomeeCarousel* oomeeCarousel = OomeeCarousel::create();
    oomeeCarousel->setContentSize(_contentLayer->getContentSize() * 0.95);
    oomeeCarousel->setVisibleRange(MIN(MAX(1,ceil(trimmedFilenames.size() * 0.75f)), 6));
    oomeeCarousel->setPosition(_contentLayer->getContentSize() / 2);
    oomeeCarousel->setOomeeData(trimmedFilenames);
    _contentLayer->addChild(oomeeCarousel);
    
    ui::Button* newOomeeButton = ui::Button::create();
    newOomeeButton->loadTextureNormal("res/oomeeMaker/menu6.png");
    newOomeeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    newOomeeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    newOomeeButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        this->newOomee();
    });
    _contentLayer->addChild(newOomeeButton);
    
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

NS_AZOOMEE_OM_END
