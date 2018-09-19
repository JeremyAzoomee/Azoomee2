//
//  OomeeMakerLayer.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 21/02/2018.
//

#include "OomeeMakerScene.h"
#include "SimpleAudioEngine.h"
#include "../DataObjects/OomeeMakerDataHandler.h"
#include "OomeeItemList.h"
#include "ItemCategoryList.h"
#include "OomeeFigure.h"
#include "DragAndDropController.h"
#include "OomeeSelectScene.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

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
    
    const Size& contentSize = _contentLayer->getContentSize();
    
    LayerColor* base = LayerColor::create(Color4B(Style::Color::white));
    _contentLayer->addChild(base);
    LayerColor* bg = LayerColor::create(Color4B(Style::Color::greyBlue));
    bg->setOpacity(50);
    _contentLayer->addChild(bg);
    
    Sprite* wiresLeft = Sprite::create("res/oomeeMaker/wire_left.png");
    wiresLeft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    wiresLeft->setPosition(Vec2(0, contentSize.height));
    wiresLeft->setOpacity(0);
    wiresLeft->runAction(Sequence::create(DelayTime::create(2.0),FadeIn::create(1.5), NULL));
    _contentLayer->addChild(wiresLeft);
    
    Sprite* wiresRight = Sprite::create("res/oomeeMaker/wire_right.png");
    wiresRight->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    wiresRight->setPosition(Vec2(contentSize.width * 0.75, contentSize.height));
    wiresRight->setOpacity(0);
    wiresRight->runAction(Sequence::create(DelayTime::create(2.0),FadeIn::create(1.5), NULL));
    _contentLayer->addChild(wiresRight);
    
    Sprite* bgCircle1 = Sprite::create("res/oomeeMaker/circle_0.png");
    bgCircle1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bgCircle1->setNormalizedPosition(Vec2(0.42,0.5));
    bgCircle1->setScale(0);
    bgCircle1->setOpacity(25);
    bgCircle1->setRotation(RandomHelper::random_real(0.0,M_PI));
    bgCircle1->setColor(Style::Color::darkTeal);
    _contentLayer->addChild(bgCircle1);
    
    auto popIn1 = EaseBackOut::create(ScaleTo::create(0.5, ((contentSize.width * 0.35) / bgCircle1->getContentSize().height)));
    auto rotate1 = RepeatForever::create(RotateBy::create(30 + CCRANDOM_0_1() * 30, 360));
    
    bgCircle1->runAction(popIn1);
    bgCircle1->runAction(rotate1);
    
    Sprite* bgCircle2 = Sprite::create("res/oomeeMaker/circle_1.png");
    bgCircle2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bgCircle2->setNormalizedPosition(Vec2( 0.42, 0.5));
    bgCircle2->setScale(0);
    bgCircle2->setOpacity(25);
    bgCircle2->setRotation(RandomHelper::random_real(0.0,M_PI));
    bgCircle2->setColor(Style::Color::darkTeal);
    _contentLayer->addChild(bgCircle2);
    
    auto popIn2 = EaseBackOut::create(ScaleTo::create(0.5, ((contentSize.width * 0.45) / bgCircle2->getContentSize().height)));
    auto rotate2 = RepeatForever::create(RotateBy::create(30 +  CCRANDOM_0_1() * 30, -360));
    
    bgCircle2->runAction(popIn2);
    bgCircle2->runAction(rotate2);
}

void OomeeMakerScene::onEnter()
{
    const Size& contentSize = _contentLayer->getContentSize();
    
    const OomeeRef& oomeeData = OomeeMakerDataStorage::getInstance()->getOomeeForKey(kDefaultOomeeId);
    
    const std::vector<ItemCategoryRef>& categoryData = OomeeMakerDataStorage::getInstance()->getItemCategoryList();
    
    _oomee = OomeeFigure::create();
    _oomee->setContentSize(Size(contentSize.width * 0.585, contentSize.height));
    _oomee->setPosition(Vec2(contentSize.width * 0.165, 0));
    _oomee->setColour(OomeeMakerDataStorage::getInstance()->getColourForKey(kDefaultOomeeId));
    _oomee->setOomeeData(oomeeData);
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
    
    _categoryList = ItemCategoryList::create();
    _categoryList->setContentSize(Size(contentSize.width * 0.165, contentSize.height * 0.65f));
    _categoryList->setPosition(Vec2(0,contentSize.height * 0.425f));
    _categoryList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _categoryList->setItemSelectedCallback([this](const ItemCategoryRef& data) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/CategorySelection_Button.mp3");
        _categoryList->setSelectedButton(data);
        this->setItemsListForCategory(data);
    });
    _categoryList->setCategories(categoryData);
    
    _contentLayer->addChild(_categoryList);
    
    LayerColor* itemListBG = LayerColor::create(Color4B::WHITE, contentSize.width * 0.25f, contentSize.height);
    itemListBG->setPosition(Vec2(contentSize.width, 0));
    itemListBG->runAction(Sequence::create(DelayTime::create(0.5),MoveBy::create(1.5, Vec2(-contentSize.width * 0.25f, 0)), NULL));
    _contentLayer->addChild(itemListBG);
    
    _itemList = OomeeItemList::create();
    _itemList->setContentSize(Size(contentSize.width * 0.25f, contentSize.height * 0.8f));
    _itemList->setPosition(Vec2(contentSize.width + _itemList->getContentSize().width, contentSize.height / 2.0f));
    _itemList->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _itemList->setItemSelectedCallback([this](const OomeeItemRef& data) {
        this->addAccessoryToOomee(data);
    });
    _itemList->setColourSelectedCallback([this](const OomeeColourRef& colour){
        _oomee->setColour(colour);
        const OomeeRef& oomee = OomeeMakerDataStorage::getInstance()->getOomeeForKey(colour->getId());
        if(oomee)
        {
            _oomee->setOomeeData(oomee);
        }
    });
    _itemList->runAction(Sequence::create(DelayTime::create(0.5),MoveBy::create(1.5, Vec2(-_itemList->getContentSize().width, 0)), NULL));
    
    _contentLayer->addChild(_itemList);
    
    _topScrollButton = ui::Button::create("res/oomeeMaker/arrow_carousel_up.png");
    _topScrollButton->setAnchorPoint(Vec2(0.5,0.0));
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
    _bottomScrollButton->setAnchorPoint(Vec2(0.5, 1.0));
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
    
    ui::Button* categoryTopScrollButton = ui::Button::create("res/oomeeMaker/arrow_carousel_up.png");
    categoryTopScrollButton->setAnchorPoint(Vec2(0.5,0.0));
    categoryTopScrollButton->setVisible(false);
    categoryTopScrollButton->setPosition(_categoryList->getPosition() + (_categoryList->getContentSize() / 2));
    
    ui::Button* categoryBottomScrollButton = ui::Button::create("res/oomeeMaker/arrow_carousel_down.png");
    categoryBottomScrollButton->setAnchorPoint(Vec2(0.5, 1.0));
    categoryBottomScrollButton->setPosition(_categoryList->getPosition() + Vec2(_categoryList->getContentSize().width / 2, -_categoryList->getContentSize().height / 2));
    
    categoryTopScrollButton->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            _categoryList->scrollToTop(0.1, false);
            categoryTopScrollButton->setVisible(false);
            categoryBottomScrollButton->setVisible(true);
        }
    });
    _contentLayer->addChild(categoryTopScrollButton);
    
    categoryBottomScrollButton->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType)
    {
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            _categoryList->scrollToBottom(0.1, false);
            categoryTopScrollButton->setVisible(true);
            categoryBottomScrollButton->setVisible(false);
        }
    });
    _contentLayer->addChild(categoryBottomScrollButton);
    
    ui::Button* exitButton = ui::Button::create();
    exitButton->loadTextureNormal("res/oomeeMaker/back.png");
    exitButton->setAnchorPoint(Vec2(-0.25, 1.25));
    exitButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    exitButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->saveAndExit();
        }
    });
    _contentLayer->addChild(exitButton);
    
    ui::Button* makeAvatarButon = ui::Button::create();
    makeAvatarButon->loadTextureNormal("res/oomeeMaker/make_oomee_button.png");
    makeAvatarButon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    makeAvatarButon->setPosition(Vec2(contentSize.width * 0.42, makeAvatarButon->getContentSize().height));
    makeAvatarButon->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->makeAvatar();
        }
    });
    _contentLayer->addChild(makeAvatarButon);
    
    _undoButton = ui::Button::create();
    _undoButton->loadTextureNormal("res/oomeeMaker/undo.png");
    _undoButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _undoButton->setPosition(makeAvatarButon->getPosition() - Vec2(makeAvatarButon->getContentSize().width,0));
    _undoButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->undo();
        }
    });
    _contentLayer->addChild(_undoButton);
    
    ui::Button* shareOomeeButon = ui::Button::create();
    shareOomeeButon->loadTextureNormal("res/oomeeMaker/share_button.png");
    shareOomeeButon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    shareOomeeButon->setPosition(makeAvatarButon->getPosition() + Vec2(makeAvatarButon->getContentSize().width, 0));
    shareOomeeButon->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            this->shareOomee();
        }
    });
    _contentLayer->addChild(shareOomeeButon);
    this->scheduleUpdate();
    Super::onEnter();
}

void OomeeMakerScene::update(float deltaT)
{
    if(_itemList && _itemList->getItems().size() > 0)
    {
        _bottomScrollButton->setVisible(_itemList->getBottommostItemInCurrentView() != _itemList->getItems().back());
        _topScrollButton->setVisible(_itemList->getTopmostItemInCurrentView() != _itemList->getItems().front());

    }
    Super::update(deltaT);
}

void OomeeMakerScene::onEnterTransitionDidFinish()
{
    DragAndDropController::getInstance()->setTargetOomee(_oomee);
    DragAndDropController::getInstance()->attachToScene(this);
    DragAndDropController::getInstance()->setDebugModeEnabled(true);
    
    runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=](){
        _categoryList->setSelectedButton(OomeeMakerDataStorage::getInstance()->getItemCategoryForKey(kColourCategoryId));
        setItemsListForCategory(OomeeMakerDataStorage::getInstance()->getItemCategoryForKey(kColourCategoryId));
    }), NULL));
    
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

void OomeeMakerScene::undo()
{
    if(_oomee)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Undo_Exit_Buttons.mp3");
        _oomee->undoLastAction();
    }
}

void OomeeMakerScene::saveAndExit()
{
    ModalMessages::getInstance()->startSaving();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Undo_Exit_Buttons.mp3");
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
    AnalyticsSingleton::getInstance()->saveOomee(savedFileContent);
}

void OomeeMakerScene::makeAvatar()
{
    ModalMessages::getInstance()->startSaving();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Make_Avatar_Button.mp3");
    const std::string scheduleKey = "saveAndExit";
    Director::getInstance()->getScheduler()->schedule([&](float dt){
        saveOomeeFiles();
        ModalMessages::getInstance()->stopSaving();
        if(delegate)
        {
            delegate->onOomeeMakerUpdateAvatar(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + _filename + ".png");
        }
    }, this, 0.5, 0, 0, false, scheduleKey);
    
}

void OomeeMakerScene::shareOomee()
{
    ModalMessages::getInstance()->startSaving();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Share_Button.mp3");
    const std::string scheduleKey = "saveAndExit";
    Director::getInstance()->getScheduler()->schedule([&](float dt){
        saveOomeeFiles();
        ModalMessages::getInstance()->stopSaving();
        if(delegate)
        {
            delegate->onOomeeMakerShareOomee(OomeeMakerDataHandler::getInstance()->getFullSaveDir() + _filename + ".png");
        }
    }, this, 0.5, 0, 0, false, scheduleKey);
}

void OomeeMakerScene::displayMadeAvatarNotification()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("res/oomeeMaker/Audio/Avatar_Messagedrop.mp3");
    Texture2D* particleTex = Director::getInstance()->getTextureCache()->addImage("res/oomeeMaker/confetti_particle.png");
    
    std::vector<Color4F> colours = {
        Style::Color_4F::green,
        Style::Color_4F::yellow,
        Style::Color_4F::red,
        Style::Color_4F::purple,
        Style::Color_4F::blue
    };
    
    for(auto colour : colours)
    {
        auto particles = ParticleFireworks::create();
        particles->cocos2d::ParticleSystem::setTotalParticles(100.0f/ colours.size());
        particles->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        particles->setEmissionRate(33.0f / colours.size());
        particles->setDuration(2.0f);
        particles->setSpeed(-50);
        particles->setScale(4.0f);
        particles->setStartSize(17.5f);
        particles->setStartSizeVar(0);
        particles->setEndSize(17.5f);
        particles->setEndSizeVar(0);
        particles->setGravity(Vec2(0,-400));
        particles->setPosVar(Vec2(150, 10));
        particles->setStartSpinVar(180);
        particles->setStartColor(colour);
        particles->setStartColorVar(Color4F(0.0,0.0,0.0,0.0));
        particles->setEndColor(colour);
        particles->setEndColorVar(Color4F(0.0,0.0,0.0,0.0));
        particles->cocos2d::ParticleSystem::setTexture(particleTex);
        particles->setPosition(Vec2(_contentLayer->getContentSize().width * 0.42, _contentLayer->getContentSize().height));
        particles->setAutoRemoveOnFinish(true);
        _contentLayer->addChild(particles,10);
    }
    auto banner = ui::Scale9Sprite::create("res/oomeeMaker/green_box.png");
    banner->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    banner->setPosition(Vec2(_contentLayer->getContentSize().width * 0.42, _contentLayer->getContentSize().height));
    banner->setContentSize(Size(_contentLayer->getContentSize().width * 0.43 , 400));
    banner->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0,-200)), DelayTime::create(3.0f),MoveBy::create(0.5, Vec2(0,200)),CallFunc::create([=](){
        banner->removeFromParent();
    }),NULL));
    _contentLayer->addChild(banner,10);
    
    auto bannerLabel = Label::createWithTTF(StringMgr::getInstance()->getStringForKey(OOMEE_MAKER_NEW_AVATAR), Style::Font::Regular, 83);
    bannerLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bannerLabel->setNormalizedPosition(Vec2(0.5, 0.25));
    bannerLabel->setColor(Color3B::WHITE);
    banner->addChild(bannerLabel);
    
}

NS_AZOOMEE_OM_END
