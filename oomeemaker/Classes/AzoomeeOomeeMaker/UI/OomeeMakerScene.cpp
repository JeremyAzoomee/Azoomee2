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
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/ModalMessages.h>

using namespace cocos2d;

NS_AZOOMEE_OM_BEGIN

const std::string OomeeMakerScene::kDefaultOomeeId = "oomeeNew";
const std::string OomeeMakerScene::kColourCategoryId = "colours";

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
    
    LayerColor* bg = LayerColor::create(Color4B(Style::Color::white));
    _contentLayer->addChild(bg);
    this->addChild(_contentLayer);
    
    return true;
}

void OomeeMakerScene::onEnter()
{
    const OomeeRef& oomeeData = OomeeMakerDataStorage::getInstance()->getOomeeForKey(kDefaultOomeeId);
    
    const std::vector<ItemCategoryRef>& categoryData = OomeeMakerDataStorage::getInstance()->getItemCategoryList();
    
    _oomee = OomeeFigure::create();
    _oomee->setContentSize(_contentLayer->getContentSize());
    _oomee->setOomeeData(oomeeData);
    _oomee->setColour(OomeeMakerDataStorage::getInstance()->getColourForKey("yellow"));
    _oomee->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _oomee->setEditable(true);
    if(FileUtils::getInstance()->isFileExist(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + _filename + ".oomee"))
    {
        rapidjson::Document data;
        data.Parse(FileUtils::getInstance()->getStringFromFile(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + _filename + ".oomee").c_str());
        _oomee->initWithOomeeFigureData(data);
    }
    else
    {
        _oomee->addDefaultAccessories();
    }
    _contentLayer->addChild(_oomee);
    
    ItemCategoryList* categories = ItemCategoryList::create();
    categories->setContentSize(Size(_contentLayer->getContentSize().width / 6.0f, _contentLayer->getContentSize().height * 0.85f));
    categories->setNormalizedPosition(Vec2(0,0.425f));
    categories->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    categories->setItemSelectedCallback([this, categories](const ItemCategoryRef& data) {
        categories->setSelectedButton(data);
        this->setItemsListForCategory(data);
    });
    categories->setCategories(categoryData);
    
    _contentLayer->addChild(categories);
    
    _itemList = OomeeItemList::create();
    _itemList->setContentSize(Size(_contentLayer->getContentSize().width * 0.25f, _contentLayer->getContentSize().height));
    _itemList->setPosition(Vec2(_contentLayer->getContentSize().width + _itemList->getContentSize().width, _contentLayer->getContentSize().height / 2.0f));
    _itemList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _itemList->setItemSelectedCallback([this](const OomeeItemRef& data) {
        this->addAccessoryToOomee(data);
    });
    //_itemList->setColourSelectedCallback([this](float hue){
    //    _oomee->setHue(hue);
    //});
    _itemList->setColourSelectedCallback([this](const OomeeColourRef& colour){
        _oomee->setColour(colour);
    });
    _itemList->runAction(Sequence::create(DelayTime::create(0.5),MoveBy::create(1.5, Vec2(-_itemList->getContentSize().width, 0)), NULL));
    categories->setSelectedButton(categoryData.at(0));
    setItemsListForCategory(categoryData.at(0));
    
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
        if(data->getId() == kColourCategoryId)
        {
            _itemList->SetColourItems();
        }
        else
        {
            _itemList->setItems(OomeeMakerDataStorage::getInstance()->getItemsForCategory(data->getId()));
        }
    }
}

void OomeeMakerScene::setFilename(const std::string &filename)
{
    _filename = filename;
}

void OomeeMakerScene::saveAndExit()
{
    ModalMessages::getInstance()->startSaving();
    
    const std::string scheduleKey = "saveAndExit";
    Director::getInstance()->getScheduler()->schedule([&](float dt){
        saveOomeeFiles();
        Director::getInstance()->replaceScene(OomeeSelectScene::create());
    }, this, 0.5, 0, 0, false, scheduleKey);
    
}

void OomeeMakerScene::saveOomeeFiles()
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
    savedFileContent += StringUtils::format("], \"colourHue\": %f }", _oomee->getHue());
    
    FileUtils::getInstance()->writeStringToFile(savedFileContent, OomeeMakerDataHandler::getInstance()->getFullSaveDir() + _filename + ".oomee");
    
    _oomee->saveSnapshotImage(OomeeMakerDataHandler::getInstance()->getLocalSaveDir() + _filename + ".png");
}

NS_AZOOMEE_OM_END
