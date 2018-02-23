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
    std::string saveFileName;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    
    std::ostringstream oss;
    oss << tm.tm_mday << tm.tm_mon << tm.tm_year << tm.tm_hour << tm.tm_min << tm.tm_sec;
    auto fileNameStr = oss.str();
        
    saveFileName = "oomeeFiles/" + fileNameStr;
    
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
    
    OomeeMakerDataHandler::getInstance()->getConfigFilesIfNeeded();
    
    _carousel = ui::ListView::create();
    _carousel->setContentSize(_contentLayer->getContentSize() * 0.7f);
    _carousel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _carousel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _carousel->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    _carousel->setBounceEnabled(true);
    _carousel->setGravity(ui::ListView::Gravity::CENTER_VERTICAL);
    _carousel->setItemsMargin(50.0f);
    
    _contentLayer->addChild(_carousel);
    
    const std::string& searchDir = FileUtils::getInstance()->getWritablePath() + "oomeeFiles/";
    
    if(!FileUtils::getInstance()->isDirectoryExist(searchDir))
    {
        FileUtils::getInstance()->createDirectory(searchDir);
    }
    
    const std::vector<std::string>& createdOomeeFiles = DirectorySearcher::getInstance()->getFilesInDirectoryWithExtention(searchDir, ".png");
    
    for(std::string filename : createdOomeeFiles)
    {
        std::string trimmedFilename = filename.substr(0,filename.size() - 4);
        ui::Button* button = ui::Button::create(searchDir + filename);
        button->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType)
        {
            if(eType == ui::Widget::TouchEventType::ENDED)
            {
                this->editOomee("oomeeFiles/" + trimmedFilename);
            }
        });
        _carousel->pushBackCustomItem(button);
    }
    
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
        delegate->onOomeeMakerNavigationBack();
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
}

NS_AZOOMEE_OM_END
