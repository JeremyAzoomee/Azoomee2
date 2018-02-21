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
        
    saveFileName = FileUtils::getInstance()->getWritablePath() + "oomeeFiles/" + fileNameStr + ".oomee";
    
    makerScene->setFilename(saveFileName);
    Director::getInstance()->replaceScene(makerScene);
}

bool OomeeSelectScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setContentSize(Director::getInstance()->getVisibleSize());
    setPosition(Director::getInstance()->getVisibleOrigin());
    
    OomeeMakerDataHandler::getInstance()->getConfigFilesIfNeeded();
    
    _carousel = ui::ListView::create();
    _carousel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _carousel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    this->addChild(_carousel);
    
    const std::string& searchDir = FileUtils::getInstance()->getWritablePath() + "oomeeFiles/";
    const std::vector<std::string>& createdOomeeFiles = DirectorySearcher::getInstance()->getFilesInDirectoryWithExtention(searchDir, ".oomee");
    
    for(const std::string& filename : createdOomeeFiles)
    {
        const std::string fileString = FileUtils::getInstance()->getStringFromFile(searchDir + filename);
        rapidjson::Document file;
        file.Parse(fileString.c_str());
        if(!file.HasParseError())
        {
            OomeeFigure* oomeeFigure = OomeeFigure::create();
            oomeeFigure->setEditable(false);
            if(oomeeFigure->initWithOomeeFigureData(file))
            {
                _carousel->addChild(oomeeFigure);
                _createdOomees[searchDir + filename] = oomeeFigure;
            }
        }
    }
    
    ui::Button* exitButton = ui::Button::create();
    exitButton->loadTextureNormal("CloseNormal.png");
    exitButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    exitButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    exitButton->setContentSize(Size(250,250));
    exitButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        this->newOomee();
    });
    this->addChild(exitButton);
    
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
