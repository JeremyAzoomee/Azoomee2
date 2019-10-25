//
//  RecentMessages.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 25/10/2019.
//

#include "RecentMessages.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float RecentMessages::kListViewPadding = 45.0f;
const float RecentMessages::kHeaderHeight = 300.0f;
const float RecentMessages::kMessageBarSpacing = 30.0f;
const float RecentMessages::kMessageBarHeightPortrait = 300.0f;
const float RecentMessages::kMessageBarHeightLandscape = 240.0f;

bool RecentMessages::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0f, 1.0f));
    
    _messageBarHeight = kMessageBarHeightLandscape;
    
    _background = RoundedRectSprite::create();
    _background->setTexture("res/decoration/white_1px.png");
    _background->setCornerRadius(20);
    _background->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _background->setColor(Style::Color::darkIndigoThree);
    _background->setStretchImageEnabled(true);
    addChild(_background, -1);
    
    _contentLayout = ui::Layout::create();
    _contentLayout->setSizeType(SizeType::PERCENT);
    _contentLayout->setSizePercent(Vec2(1.0f,1.0f));
    _contentLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _contentLayout->setLayoutType(Type::VERTICAL);
    addChild(_contentLayout);
    
    _headerLayout = ui::Layout::create();
    _headerLayout->setContentSize(Size(getContentSize().width, kHeaderHeight));
    _headerLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _contentLayout->addChild(_headerLayout);
    
    _headerText = DynamicText::create(_("Recent Messages"), Style::Font::PoppinsBold(), 80);
    _headerText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _headerText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _headerText->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _headerText->setTextVerticalAlignment(TextVAlignment::CENTER);
    _headerLayout->addChild(_headerText);
    
    _divider = cocos2d::ui::Layout::create();
    _divider->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _divider->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _divider->setBackGroundColorType(BackGroundColorType::SOLID);
    _divider->setBackGroundColor(Style::Color::macaroniAndCheese);
    _headerLayout->addChild(_divider);
    
    _messageListView = ui::ListView::create();
    _messageListView->setContentSize(Size(getContentSize().width, getContentSize().height - kHeaderHeight));
    _messageListView->setDirection(ui::ListView::Direction::VERTICAL);
    _messageListView->setBounceEnabled(true);
    _messageListView->setItemsMargin(kMessageBarSpacing);
    _messageListView->setPadding(kListViewPadding, kListViewPadding, kListViewPadding, kListViewPadding);
    _messageListView->setGravity(ui::ListView::Gravity::LEFT);
    _messageListView->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _contentLayout->addChild(_messageListView);
    
    _bottomGradient = RoundedRectSprite::create();
    _bottomGradient->setTexture("res/hqscene/gradient_overlay_large.png");
    _bottomGradient->setStretchImageEnabled(true);
    _bottomGradient->setCornerRadius(20);
    _bottomGradient->setRoundedCorners(true, true, false, false);
    _bottomGradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _bottomGradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _bottomGradient->setColor(Style::Color::darkIndigoThree);
    addChild(_bottomGradient);
    
    return true;
}

void RecentMessages::onEnter()
{
    Super::onEnter();
}

void RecentMessages::onExit()
{
    Super::onExit();
}

void RecentMessages::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    
    _background->setContentSize(contentSize);
    _contentLayout->updateSizeAndPosition();
    _divider->setContentSize(Size(contentSize.width, 6));
    _headerLayout->setContentSize(Size(contentSize.width, kHeaderHeight));
    _messageListView->setContentSize(Size(contentSize.width, contentSize.height - kHeaderHeight));
    const Size& messageBarSize = Size(contentSize.width - (2 * kListViewPadding), _messageBarHeight);
    for(auto bar : _messageBars)
    {
        bar->setContentSize(messageBarSize);
    }
    _bottomGradient->setContentSize(Size(contentSize.width, _messageBarHeight));
    _headerLayout->updateSizeAndPosition();
}

void RecentMessages::setMessageData(const std::vector<RecentMessage>& messageData)
{
    _messageData = messageData;
    setupMessageBars();
}

void RecentMessages::setMessageSelectedCallback(const MessageSelectedCallback& callback)
{
    _callback = callback;
}

void RecentMessages::toggleBottomGradient(bool enabled)
{
    _bottomGradient->setVisible(enabled);
}

void RecentMessages::setMessageBarHeight(float height)
{
    _messageBarHeight = height;
    const Size& messageBarSize = Size(getContentSize().width - (2 * kListViewPadding), _messageBarHeight);
    for(auto bar : _messageBars)
    {
        bar->setContentSize(messageBarSize);
    }
    
}

void RecentMessages::setupMessageBars()
{
    _messageListView->removeAllItems();
    _messageBars.clear();
    if(_messageData.size() > 0)
    {
        const Size& messageBarSize = Size(getContentSize().width - (2 * kListViewPadding), _messageBarHeight);
        for(auto message : _messageData)
        {
            ui::Layout* bar = ui::Layout::create();
            bar->setBackGroundColor(Color3B::RED);
            bar->setBackGroundColorType(BackGroundColorType::SOLID);
            bar->setContentSize(messageBarSize);
            _messageListView->pushBackCustomItem(bar);
            _messageBars.pushBack(bar);
        }
    }
    _messageListView->scrollToTop(0, false);
}

NS_AZOOMEE_END
