//
//  SettingsOnlineSafetyPage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 24/08/2018.
//

#include "SettingsOnlineSafetyPage.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include "OnlineSafetySlide.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsOnlineSafetyPage::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setLayoutType(ui::Layout::Type::VERTICAL);
    
    return true;
}

void SettingsOnlineSafetyPage::onEnter()
{
    _headerBanner = SettingsPageHeader::create();
    _headerBanner->setContentSize(Size(this->getContentSize().width, 316));
    _headerBanner->setText(StringMgr::getInstance()->getStringForKey(SETTINGS_PAGE_HEADING_ONLINE_SAFETY));
    _headerBanner->setLayoutParameter(CreateTopLinearLayoutParam());
    this->addChild(_headerBanner);
    
    _onlineSafetyPageView = ui::PageView::create();
    _onlineSafetyPageView->setContentSize(Size(this->getContentSize().width - 100, this->getContentSize().height * 0.74f));
    _onlineSafetyPageView->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
    _onlineSafetyPageView->setDirection(ui::ScrollView::Direction::VERTICAL);
    for(int i = 1; i <= 5; i++)
    {
        OnlineSafetySlide* slide = OnlineSafetySlide::create();
        slide->setContentSize(_onlineSafetyPageView->getContentSize());
        slide->setTitle(StringMgr::getInstance()->getStringForKey(StringUtils::format("%sAdult%d", ONLINE_SAFETY_SLIDE_TITLE,i)));
        slide->setImage(StringUtils::format("res/onlineSafetySlides/Oomees_SettingsPage-0%d.png",i));
        slide->setBodyText(StringMgr::getInstance()->getStringForKey(StringUtils::format("%sAdult%d", ONLINE_SAFETY_SLIDE_MAIN_TEXT,i)));
        _onlineSafetyPageView->addPage(slide);
    }
    
    this->addChild(_onlineSafetyPageView);
    
    _pageTurnButton = ui::Button::create("res/settings/page_turn_button.png");
    _pageTurnButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
    _pageTurnButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _pageTurnButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_onlineSafetyPageView->getCurrentPageIndex() < _onlineSafetyPageView->getItems().size() - 1)
            {
                _onlineSafetyPageView->scrollToPage(_onlineSafetyPageView->getCurrentPageIndex() + 1, 1.0f);
            }
            else
            {
                _onlineSafetyPageView->scrollToPage(0);
            }
        }
    });
    this->addChild(_pageTurnButton);
    
    _onlineSafetyPageView->addEventListener([&](Ref* pSender, ui::PageView::EventType eType){
        if(eType == ui::PageView::EventType::TURNING)
        {
            if(_onlineSafetyPageView->getCurrentPageIndex() == _onlineSafetyPageView->getItems().size() - 1)
            {
                _pageTurnButton->setRotation(180);
            }
            else
            {
                _pageTurnButton->setRotation(0);
            }
        }
    });
    
    _onlineSafetyPageView->setCurrentPageIndex(0);
    
    Super::onEnter();
}

NS_AZOOMEE_END


