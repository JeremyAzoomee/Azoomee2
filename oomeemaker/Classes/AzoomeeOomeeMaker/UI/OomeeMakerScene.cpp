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
    
    LayerColor* bg = LayerColor::create(Color4B(Style::Color::barney));
    _contentLayer->addChild(bg);
    this->addChild(_contentLayer);
    
    return true;
}

void OomeeMakerScene::onEnter()
{
    const OomeeRef& oomeeData = OomeeMakerDataStorage::getInstance()->getOomeeForKey("oomee01");
    
    const std::vector<ItemCategoryRef>& categoryData = OomeeMakerDataStorage::getInstance()->getItemCategoryList();
    
    const std::vector<OomeeItemRef>& itemsData = OomeeMakerDataStorage::getInstance()->getItemsForCategory(categoryData.at(0)->getId());
    
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
    _itemList->setContentSize(Size(_contentLayer->getContentSize().width * 0.25f, _contentLayer->getContentSize().height * 0.9f));
    _itemList->setNormalizedPosition(Vec2(1.0f,0.5f));
    _itemList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _itemList->setItemSelectedCallback([this](const OomeeItemRef& data) {
        this->addAccessoryToOomee(data);
    });
    _itemList->setItems(itemsData);
    
    _contentLayer->addChild(_itemList);
    
    for(int i = 0; i < 10; i++)
    {
        ui::Button* colourButton = ui::Button::create();
        colourButton->loadTextureNormal("res/oomeeMaker/back_new.png");
        colourButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        colourButton->setNormalizedPosition(Vec2(0.15 + i * 0.075, 0.1));
        colourButton->addTouchEventListener([this, i](Ref* pSender, ui::Widget::TouchEventType eType){
            _oomee->setHue(2 * M_PI * (i/10.0f));
        });
        SpriteWithHue* visibleSprite = SpriteWithHue::create("res/oomeeMaker/body_00.png");
        visibleSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        visibleSprite->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        visibleSprite->setScale(colourButton->getContentSize().width / visibleSprite->getContentSize().width);
        visibleSprite->setHue(2 * M_PI * (i/10.0f));
        colourButton->addChild(visibleSprite);
        
        _contentLayer->addChild(colourButton);
    }
    
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
        _itemList->setItems(OomeeMakerDataStorage::getInstance()->getItemsForCategory(data->getId()));
    }
}

void OomeeMakerScene::setFilename(const std::string &filename)
{
    _filename = filename;
}

void OomeeMakerScene::saveAndExit()
{
    auto overlay = LayerColor::create(Style::Color_4B::semiTransparentOverlay, Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height);
    overlay->setPosition(Director::getInstance()->getVisibleOrigin());
    overlay->setName("savingOverlay");
    this->addChild(overlay,2);
    
    auto savingLabel = Label::createWithTTF("Saving...", Style::Font::Regular, 128);
    savingLabel->setColor(Style::Color::white);
    savingLabel->setNormalizedPosition(Vec2(0.5,0.5));
    savingLabel->setAnchorPoint(Vec2(0.5,0.5));
    overlay->addChild(savingLabel);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), overlay);
    const std::string scheduleKey = "saveAndShare";
    Director::getInstance()->getScheduler()->schedule([&](float dt){
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
        
        FileUtils::getInstance()->writeStringToFile(savedFileContent, FileUtils::getInstance()->getWritablePath() + _filename + ".oomee");
        
        _oomee->saveSnapshotImage(_filename + ".png");
        
        Director::getInstance()->replaceScene(OomeeSelectScene::create());
    }, this, 0.5, 0, 0, false, scheduleKey);
    
}

NS_AZOOMEE_OM_END
