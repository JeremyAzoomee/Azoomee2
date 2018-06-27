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

const std::string OomeeMakerScene::kDefaultOomeeId = "yellow";
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
    
    this->addChild(_contentLayer);
    
    addBGLayer();
    
    return true;
}

void OomeeMakerScene::addBGLayer()
{
    LayerColor* base = LayerColor::create(Color4B(Style::Color::white));
    _contentLayer->addChild(base);
    LayerColor* bg = LayerColor::create(Color4B(Style::Color::greyBlue));
    bg->setOpacity(50);
    _contentLayer->addChild(bg);
    
    Sprite* wiresLeft = Sprite::create("res/oomeeMaker/wire_left.png");
    wiresLeft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    wiresLeft->setPosition(Vec2(0, _contentLayer->getContentSize().height));
    wiresLeft->setOpacity(0);
    wiresLeft->runAction(Sequence::create(DelayTime::create(2.0),FadeIn::create(1.5), NULL));
    _contentLayer->addChild(wiresLeft);
    
    Sprite* wiresRight = Sprite::create("res/oomeeMaker/wire_right.png");
    wiresRight->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    wiresRight->setPosition(Vec2(_contentLayer->getContentSize().width * 0.75, _contentLayer->getContentSize().height));
    wiresRight->setOpacity(0);
    wiresRight->runAction(Sequence::create(DelayTime::create(2.0),FadeIn::create(1.5), NULL));
    _contentLayer->addChild(wiresRight);
    
    Sprite* bgCircle1 = Sprite::create("res/oomeeMaker/circle_0.png");
    bgCircle1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bgCircle1->setNormalizedPosition(Vec2(0.42,0.5));
    bgCircle1->setScale(0);
    bgCircle1->setOpacity(25);
    bgCircle1->setRotation(RandomHelper::random_real(0.0,M_PI));
    _contentLayer->addChild(bgCircle1);
    
    auto popIn1 = EaseBackOut::create(ScaleTo::create(0.5, ((_contentLayer->getContentSize().width * 0.35) / bgCircle1->getContentSize().height)));
    auto rotate1 = RepeatForever::create(RotateBy::create(30 + CCRANDOM_0_1() * 30, 360));
    
    bgCircle1->runAction(popIn1);
    bgCircle1->runAction(rotate1);
    
    Sprite* bgCircle2 = Sprite::create("res/oomeeMaker/circle_1.png");
    bgCircle2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bgCircle2->setNormalizedPosition(Vec2( 0.42, 0.5));
    bgCircle2->setScale(0);
    bgCircle2->setOpacity(25);
    bgCircle2->setRotation(RandomHelper::random_real(0.0,M_PI));
    _contentLayer->addChild(bgCircle2);
    
    auto popIn2 = EaseBackOut::create(ScaleTo::create(0.5, ((_contentLayer->getContentSize().width * 0.45) / bgCircle2->getContentSize().height)));
    auto rotate2 = RepeatForever::create(RotateBy::create(30 +  CCRANDOM_0_1() * 30, -360));
    
    bgCircle2->runAction(popIn2);
    bgCircle2->runAction(rotate2);
}

void OomeeMakerScene::onEnter()
{
    const OomeeRef& oomeeData = OomeeMakerDataStorage::getInstance()->getOomeeForKey(kDefaultOomeeId);
    
    const std::vector<ItemCategoryRef>& categoryData = OomeeMakerDataStorage::getInstance()->getItemCategoryList();
    
    _oomee = OomeeFigure::create();
    _oomee->setContentSize(Size(_contentLayer->getContentSize().width * 0.585, _contentLayer->getContentSize().height));
    _oomee->setPosition(Vec2(_contentLayer->getContentSize().width * 0.165, 0));
    _oomee->setOomeeData(oomeeData);
    _oomee->setColour(OomeeMakerDataStorage::getInstance()->getColourForKey(kDefaultOomeeId));
    _oomee->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _oomee->setEditable(true);
    if(FileUtils::getInstance()->isFileExist(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + _filename + ".oomee"))
    {
        rapidjson::Document data;
        data.Parse(FileUtils::getInstance()->getStringFromFile(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + _filename + ".oomee").c_str());
        if(!_oomee->initWithOomeeFigureData(data))
        {
            _oomee->setOomeeData(oomeeData);
            _oomee->setColour(OomeeMakerDataStorage::getInstance()->getColourForKey(kDefaultOomeeId));
        }
    }
    _contentLayer->addChild(_oomee);
    
    ItemCategoryList* categories = ItemCategoryList::create();
    categories->setContentSize(Size(_contentLayer->getContentSize().width * 0.165, _contentLayer->getContentSize().height * 0.85f));
    categories->setNormalizedPosition(Vec2(0,0.425f));
    categories->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    categories->setItemSelectedCallback([this, categories](const ItemCategoryRef& data) {
        categories->setSelectedButton(data);
        this->setItemsListForCategory(data);
    });
    categories->setCategories(categoryData);
    
    _contentLayer->addChild(categories);
    
    LayerColor* itemListBG = LayerColor::create(Color4B::WHITE, _contentLayer->getContentSize().width * 0.25f, _contentLayer->getContentSize().height);
    itemListBG->setPosition(Vec2(_contentLayer->getContentSize().width, 0));
    itemListBG->runAction(Sequence::create(DelayTime::create(0.5),MoveBy::create(1.5, Vec2(-_contentLayer->getContentSize().width * 0.25f, 0)), NULL));
    _contentLayer->addChild(itemListBG);
    
    _itemList = OomeeItemList::create();
    _itemList->setContentSize(Size(_contentLayer->getContentSize().width * 0.25f, _contentLayer->getContentSize().height * 0.8f));
    _itemList->setPosition(Vec2(_contentLayer->getContentSize().width + _itemList->getContentSize().width, _contentLayer->getContentSize().height / 2.0f));
    _itemList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _itemList->setItemSelectedCallback([this](const OomeeItemRef& data) {
        this->addAccessoryToOomee(data);
    });
    _itemList->setColourSelectedCallback([this](const OomeeColourRef& colour){
        const OomeeRef& oomee = OomeeMakerDataStorage::getInstance()->getOomeeForKey(colour->getId());
        if(oomee)
        {
            _oomee->setOomeeData(oomee);
        }
        _oomee->setColour(colour);
    });
    _itemList->runAction(Sequence::create(DelayTime::create(0.5),MoveBy::create(1.5, Vec2(-_itemList->getContentSize().width, 0)), NULL));
    categories->setSelectedButton(categoryData.at(0));
    setItemsListForCategory(categoryData.at(0));
    
    _contentLayer->addChild(_itemList);
    
    _topScrollButton = ui::Button::create("res/oomeeMaker/arrow_carousel_up.png");
    _topScrollButton->setAnchorPoint(Vec2(0.5,-1.0));
    _topScrollButton->setPosition(_itemList->getPosition() + Vec2(-_itemList->getContentSize().width / 2, _itemList->getContentSize().height / 2));
    _topScrollButton->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::BEGAN)
        {
            _itemList->scrollToTop(5, true);
        }
        else if(eType == ui::Widget::TouchEventType::ENDED || eType == ui::Widget::TouchEventType::CANCELED)
        {
            _itemList->stopAutoScroll();
        }
    });
    _topScrollButton->runAction(Sequence::create(DelayTime::create(0.5),MoveBy::create(1.5, Vec2(-_itemList->getContentSize().width, 0)), NULL));
    _contentLayer->addChild(_topScrollButton);
    
    _bottomScrollButton = ui::Button::create("res/oomeeMaker/arrow_carousel_down.png");
    _bottomScrollButton->setAnchorPoint(Vec2(0.5, 2.0));
    _bottomScrollButton->setPosition(_itemList->getPosition() - (_itemList->getContentSize() / 2));
    _bottomScrollButton->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::BEGAN)
        {
            _itemList->scrollToBottom(5, true);
        }
        else if(eType == ui::Widget::TouchEventType::ENDED || eType == ui::Widget::TouchEventType::CANCELED)
        {
            _itemList->stopAutoScroll();
        }
    });
    _bottomScrollButton->runAction(Sequence::create(DelayTime::create(0.5),MoveBy::create(1.5, Vec2(-_itemList->getContentSize().width, 0)), NULL));
    _contentLayer->addChild(_bottomScrollButton);
    
    ui::Button* exitButton = ui::Button::create();
    exitButton->loadTextureNormal("res/oomeeMaker/back.png");
    exitButton->setAnchorPoint(Vec2(-0.25, 1.25));
    exitButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    exitButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        this->saveAndExit();
    });
    _contentLayer->addChild(exitButton);
    
    ui::Button* makeAvatarButon = ui::Button::create();
    makeAvatarButon->loadTextureNormal("res/oomeeMaker/make_oomee_button_1.png");
    makeAvatarButon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    makeAvatarButon->setPosition(Vec2(_contentLayer->getContentSize().width * 0.42, makeAvatarButon->getContentSize().height));
    makeAvatarButon->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        this->makeAvatar();
    });
    _contentLayer->addChild(makeAvatarButon);
    
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
    _oomee->addAccessory(data);
}

void OomeeMakerScene::setItemsListForCategory(const ItemCategoryRef& data)
{
    if(data->getId() == kColourCategoryId)
    {
        _itemList->setColourItems();
    }
    else
    {
        _itemList->setItems(OomeeMakerDataStorage::getInstance()->getFilteredItemsForCategory(data->getId(), _oomee->getOomeeData()));
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
    savedFileContent += StringUtils::format("], \"colour\": \"%s\" }", _oomee->getColour()->getId().c_str());
    
    FileUtils::getInstance()->writeStringToFile(savedFileContent, OomeeMakerDataHandler::getInstance()->getFullSaveDir() + _filename + ".oomee");
    
    _oomee->saveSnapshotImage(OomeeMakerDataHandler::getInstance()->getLocalSaveDir() + _filename + ".png");
}

void OomeeMakerScene::makeAvatar()
{
    ModalMessages::getInstance()->startSaving();
    
    const std::string scheduleKey = "saveAndExit";
    Director::getInstance()->getScheduler()->schedule([&](float dt){
        saveOomeeFiles();
        ModalMessages::getInstance()->stopSaving();
        if(delegate)
        {
            delegate->onOomeeMakerUpdateAvatar(_filename);
        }
    }, this, 0.5, 0, 0, false, scheduleKey);
    
}

NS_AZOOMEE_OM_END
