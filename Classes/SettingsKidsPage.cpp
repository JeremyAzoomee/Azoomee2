//
//  SettingsKidsPage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 06/09/2018.
//

#include "SettingsKidsPage.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/API/API.h>
#include "KidDetailsLayer.h"
#include "SceneManagerScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsKidsPage::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    return true;
}

void SettingsKidsPage::onEnter()
{
    _headerBanner = SettingsPageHeader::create();
    _headerBanner->setContentSize(Size(this->getContentSize().width, 316));
    _headerBanner->setText(_("Every child has a unique Kid Code. Share it with your friends and family to invite them to safely connect on Azoomee Chat."));
    _headerBanner->setLayoutParameter(CreateTopLinearLayoutParam());
    this->addChild(_headerBanner);
    
    _kidList = ui::ListView::create();
    _kidList->setDirection(ui::ScrollView::Direction::VERTICAL);
    _kidList->setBounceEnabled(true);
	_kidList->setContentSize(Size(this->getContentSize().width - 100, this->getContentSize().height - 316 - (ConfigStorage::getInstance()->isDeviceIphoneX() ? 200 : 150)));
    _kidList->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _kidList->setItemsMargin(50);
    _kidList->setTopPadding(50);
    _kidList->setBottomPadding(50);
    this->addChild(_kidList);
    
    addKidsToScrollView();
    
    _footerBanner = ui::Layout::create();
    _footerBanner->setContentSize(Size(this->getContentSize().width, 300));
    _footerBanner->setBackGroundColor(Style::Color::skyBlue);
    _footerBanner->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _footerBanner->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
			ChildDataParser::getInstance()->setChildLoggedIn(false);
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(AddChild));
        }
    });
    _footerBanner->setTouchEnabled(true);
    this->addChild(_footerBanner);
    
    ui::ImageView* newProfileImg = ui::ImageView::create("res/settings/plus.png");
    newProfileImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	newProfileImg->setNormalizedPosition(Vec2(0,0.75f));
    
    Label* newProfileText = Label::createWithTTF(_("Add New Profile"), Style::Font::Medium(), 75);
    newProfileText->setTextColor(Color4B::WHITE);
    newProfileText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    newProfileText->setNormalizedPosition(Vec2(1.0f,0.75f));
    
    ui::Layout* centerLayout = ui::Layout::create();
    centerLayout->setContentSize(Size(newProfileImg->getContentSize().width + newProfileText->getContentSize().width + 30,_footerBanner->getContentSize().height));
    centerLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    centerLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    centerLayout->addChild(newProfileText);
    centerLayout->addChild(newProfileImg);
    
    _footerBanner->addChild(centerLayout);
    
    Super::onEnter();
}

void SettingsKidsPage::addKidsToScrollView()
{
    for(int i = 0; i < ParentDataProvider::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        KidDetailsLayer* kidLayer = KidDetailsLayer::create();
        kidLayer->setContentSize(Size(_kidList->getContentSize().width, 1900));
        kidLayer->setChildNum(i);
        kidLayer->setDeleteChildCallback([&](){
            ModalMessages::getInstance()->startLoading();
            HttpRequestCreator* request = API::GetAvailableChildrenRequest(this);
            request->execute();
        });
        _kidList->pushBackCustomItem(kidLayer);
    }
}

// Delegate Functions

void SettingsKidsPage::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    ModalMessages::getInstance()->stopLoading();
    if(ParentDataParser::getInstance()->parseAvailableChildren(body))
    {
        _kidList->removeAllItems();
        addKidsToScrollView();
    }
}

void SettingsKidsPage::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
}

NS_AZOOMEE_END
