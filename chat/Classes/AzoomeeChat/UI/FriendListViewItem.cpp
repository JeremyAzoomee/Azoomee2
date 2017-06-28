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
    
    
    // Avatar
    _avatarLayout = ui::Layout::create();
    _avatarLayout->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(20.0f, 0, 0, 0)));
    _contentLayout->addChild(_avatarLayout);
    
    _avatarPlaceholder = ui::ImageView::create();
    _avatarPlaceholder->ignoreContentAdaptWithSize(false); // stretch the image
    _avatarPlaceholder->setAnchorPoint(Vec2(0.5f, 0.5f));
    _avatarPlaceholder->setSizeType(ui::Widget::SizeType::PERCENT);
    _avatarPlaceholder->setSizePercent(Vec2(1.0f, 1.0f));
    _avatarPlaceholder->setPositionType(ui::Widget::PositionType::PERCENT);
    _avatarPlaceholder->setPositionPercent(Vec2(0.5f, 0.5f));
    _avatarPlaceholder->loadTexture("res/chat/ui/avatar/avatar_empty.png");
    _avatarLayout->addChild(_avatarPlaceholder);
    
    _avatarWidget = AvatarWidget::create();
    _avatarWidget->setAnchorPoint(Vec2(0.5f, 0.5f));
    _avatarWidget->setSizeType(ui::Widget::SizeType::PERCENT);
    _avatarWidget->setSizePercent(Vec2(1.0f, 1.0f));
    _avatarWidget->setPositionType(ui::Widget::PositionType::PERCENT);
    _avatarWidget->setPositionPercent(Vec2(0.5f, 0.5f));
    _avatarLayout->addChild(_avatarWidget);
    
    // Name
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
    
    if(_friendData)
    {
        _avatarWidget->setAvatarForFriend(_friendData);
        _avatarPlaceholder->setVisible(false);
        _avatarWidget->setVisible(true);
    }
    else
    {
        _avatarWidget->setAvatarForFriend(nullptr);
        _avatarPlaceholder->setVisible(true);
        _avatarWidget->setVisible(false);
    }
}

FriendRef FriendListViewItem::getData() const
{
    return _friendData;
}

#pragma mark - Size changes

void FriendListViewItem::onSizeChanged()
{
    Super::onSizeChanged();
    
    // Resize the avatar appropriately
    const float itemHeight = _contentLayout->getContentSize().height;
    const float avatarHeightPct = 0.8f;
    const float avatarSize = itemHeight * avatarHeightPct;
    _avatarLayout->setContentSize(Size(avatarSize, avatarSize));
    
    // Re-position
    doLayout();
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
    onPressStateChangedToNormal();
}


NS_AZOOMEE_CHAT_END
