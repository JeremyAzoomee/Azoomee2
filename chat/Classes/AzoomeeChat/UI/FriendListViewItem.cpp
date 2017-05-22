#include "FriendListViewItem.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>


using namespace cocos2d;



NS_AZOOMEE_CHAT_BEGIN

bool FriendListViewItem::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Style::Color::dark);
    setLayoutType(ui::Layout::Type::VERTICAL);
    // Friend items are selectable in the ListView
    setTouchEnabled(true);
    
    // Content first
    const float borderHeightPct = 0.03f;
    _contentLayout = ui::Layout::create();
    _contentLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _contentLayout->setSizePercent(Vec2(1.0f, 1.0f - borderHeightPct));
    _contentLayout->setLayoutType(ui::Layout::Type::HORIZONTAL);
    addChild(_contentLayout);
             
    // Border at the bottom
    ui::Layout* bottomBorder = ui::Layout::create();
    bottomBorder->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    bottomBorder->setBackGroundColor(Style::Color::darkNeonPurple);
    bottomBorder->setSizeType(ui::Widget::SizeType::PERCENT);
    bottomBorder->setSizePercent(Vec2(1.0f, borderHeightPct));
    addChild(bottomBorder);
    
    
    // Add content elements
    _oomeeBackground = ui::ImageView::create();
    _oomeeBackground->loadTexture("res/chat/ui/avatar/avatar_empty.png");
    _oomeeBackground->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(20.0f, 0, 0, 0)));
    _contentLayout->addChild(_oomeeBackground);
    
    _nameLabel = ui::Text::create();
    _nameLabel->setFontName(Style::Font::Regular);
    _nameLabel->setFontSize(75.0f);
    _nameLabel->setTextColor(Color4B(Style::Color::white));
    _nameLabel->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(75.0f, 0, 0, 0)));
    _contentLayout->addChild(_nameLabel);
    
    
    // By default setup content as blank
    setData(nullptr);
  
    return true;
}

void FriendListViewItem::onEnter()
{
    Super::onEnter();
}

void FriendListViewItem::onExit()
{
    Super::onExit();
}

void FriendListViewItem::doLayout()
{
    Super::doLayout();
}

void FriendListViewItem::setData(const FriendRef& friendData)
{
    _friendData = friendData;
    
    // Update content elements
    _nameLabel->setString(_friendData ? _friendData->friendName() : "");
}

FriendRef FriendListViewItem::getData() const
{
    return _friendData;
}

#pragma mark - Interactions

void FriendListViewItem::onPressStateChangedToNormal()
{
    if(_nameLabel)
    {
        _nameLabel->setTextColor(Color4B(Style::Color::white));
    }
}

void FriendListViewItem::onPressStateChangedToPressed()
{
    if(_nameLabel)
    {
        _nameLabel->setTextColor(Color4B(Style::Color::neonPurple));
    }
}

void FriendListViewItem::onPressStateChangedToDisabled()
{
    if(_nameLabel)
    {
        _nameLabel->setTextColor(Color4B(Style::Color::white));
    }
}


NS_AZOOMEE_CHAT_END
