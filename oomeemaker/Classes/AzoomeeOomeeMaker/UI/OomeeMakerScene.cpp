//
//  OomeeMakerLayer.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 21/02/2018.
//

#include "OomeeMakerScene.h"
#include "../DataObjects/OomeeMakerDataHandler.h"
#include "OomeeItemList.h"
#include "ItemCategoryList.h"
#include "OomeeFigure.h"
#include "DragAndDropController.h"
#include "OomeeSelectScene.h"

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

// on "init" you need to initialize your instance
bool OomeeMakerScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Super::init() )
    {
        return false;
    }
    
    _contentLayer = Layer::create();
    _contentLayer->setContentSize(Director::getInstance()->getVisibleSize());
    _contentLayer->setPosition(Director::getInstance()->getVisibleOrigin());
    this->addChild(_contentLayer);
    
    return true;
}

void OomeeMakerScene::onEnter()
{
    OomeeRef oomeeData = OomeeMakerDataStorage::getInstance()->getOomeeForKey("oomee01");
    
    std::vector<ItemCategoryRef> categoryData = OomeeMakerDataStorage::getInstance()->getItemCategoryList();
    
    std::vector<OomeeItemRef> itemsData = OomeeMakerDataStorage::getInstance()->getItemsInCategoryData()[categoryData[0]->getId()];
    
    _oomee = OomeeFigure::create();
    _oomee->setOomeeData(oomeeData);
    _oomee->setContentSize(_contentLayer->getContentSize());
    _oomee->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _oomee->setEditable(true);
    if(FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + _filename + ".oomee"))
    {
        rapidjson::Document data;
        data.Parse(FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->getWritablePath() + _filename + ".oomee").c_str());
        _oomee->initWithOomeeFigureData(data);
        
    }
    
    _contentLayer->addChild(_oomee);
    
    ItemCategoryList* categories = ItemCategoryList::create();
    categories->setContentSize(Size(_contentLayer->getContentSize().width / 6.0f, _contentLayer->getContentSize().height * 0.7f));
    categories->setNormalizedPosition(Vec2(0,0.35f));
    categories->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    categories->setItemSelectedCallback([this](const ItemCategoryRef& data) {
        this->setItemsListForCategory(data);
    });
    categories->setCategories(categoryData);
    
    _contentLayer->addChild(categories);
    
    _itemList = OomeeItemList::create();
    _itemList->setContentSize(Size(_contentLayer->getContentSize().width / 6.0f, _contentLayer->getContentSize().height * 0.8f));
    _itemList->setNormalizedPosition(Vec2(1.0f,0.5f));
    _itemList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _itemList->setItemSelectedCallback([this](const OomeeItemRef& data) {
        this->addAccessoryToOomee(data);
    });
    _itemList->setItems(itemsData);
    
    _contentLayer->addChild(_itemList);
    
    ui::Button* exitButton = ui::Button::create();
    exitButton->loadTextureNormal("res/oomeeMaker/back_new.png");
    exitButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    exitButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    exitButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        this->saveAndExit();
    });
    _contentLayer->addChild(exitButton);
    
    Super::onEnter();
}

void OomeeMakerScene::onEnterTransitionDidFinish()
{
    DragAndDropController::getInstance()->setTargetOomee(_oomee);
    DragAndDropController::getInstance()->attachToScene(this);
    DragAndDropController::getInstance()->setDebugModeEnabled(true);
    Super::onEnterTransitionDidFinish();
}

void OomeeMakerScene::addAccessoryToOomee(const OomeeItemRef &data)
{
    if(_oomee)
    {
        _oomee->addAccessory(data);
    }
}

void OomeeMakerScene::setItemsListForCategory(const ItemCategoryRef& data)
{
    if(_itemList)
    {
        _itemList->setItems(OomeeMakerDataStorage::getInstance()->getItemsInCategoryData().at(data->getId()));
    }
}

void OomeeMakerScene::setFilename(const std::string &filename)
{
    _filename = filename;
}

void OomeeMakerScene::saveAndExit()
{
    std::string savedFileContent = "{";
    savedFileContent += StringUtils::format("\"oomee\":\"%s\",", _oomee->getOomeeData()->getId().c_str());
    savedFileContent += "\"oomeeItems\":[";
    const std::vector<std::string>& accIds = _oomee->getAccessoryIds();
    for(int i = 0; i < accIds.size(); i++)
    {
        savedFileContent += StringUtils::format("\"%s\"",accIds.at(i).c_str());
        if(i < accIds.size() - 1)
        {
            savedFileContent += ",";
        }
    }
    savedFileContent += "]}";
    
    FileUtils::getInstance()->writeStringToFile(savedFileContent, FileUtils::getInstance()->getWritablePath() + _filename + ".oomee");
    
    _oomee->saveSnapshotImage(_filename + ".png");
    
    Director::getInstance()->replaceScene(OomeeSelectScene::create());
}

NS_AZOOMEE_OM_END
