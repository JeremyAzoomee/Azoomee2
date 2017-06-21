#include "FriendListView.h"
#include "FriendListViewItem.h"
#include <AzoomeeCommon/UI/Style.h>


using namespace cocos2d;



NS_AZOOMEE_CHAT_BEGIN

bool FriendListView::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Style::Color::dark);
    
    setDirection(ui::ScrollView::Direction::VERTICAL);
    setBounceEnabled(true);
    setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
    
    addEventListener([this](Ref* sender, ui::ListView::EventType type) {
        if(type == ui::ListView::EventType::ON_SELECTED_ITEM_END)
        {
            const int selectedIndex = (int)getCurSelectedIndex();
            if(_selectedEventCallback && selectedIndex < _listData.size())
            {
                _selectedEventCallback(_listData[selectedIndex]);
            }
        }
    });
  
    return true;
}

void FriendListView::onEnter()
{
    Super::onEnter();
}

void FriendListView::onExit()
{
    Super::onExit();
}

void FriendListView::doLayout()
{
//    // Enable or disable bounce scroll depending on if we need it
//    const cocos2d::Size& contentSize = getContentSize();
//    const float itemMargin = _listView->getItemsMargin();
//    const float totalHeight = ((itemMargin + itemSize.height) * numOfRealItems) - itemMargin;
//    bool enabled = totalHeight > contentSize.height;
//    _listView->setBounceEnabled(enabled);
//    _listView->setScrollBarEnabled(enabled);
    
    // Now we are done, we can continue with Layout
    Super::doLayout();
}

void FriendListView::onSizeChanged()
{
    Super::onSizeChanged();
    
    // Resize all items, since they are using absolute sizing
    const auto& items = getItems();
    const cocos2d::Size& itemSize = calculateItemSize();
    for(auto item : items)
    {
        item->setContentSize(itemSize);
    }
}

void FriendListView::setContentSize(const cocos2d::Size& contentSize)
{
    Super::setContentSize(contentSize);
}

cocos2d::Size FriendListView::calculateItemSize() const
{
    // TODO: Grab sizes from config
    // TODO: Move this into FriendListViewItem
    const cocos2d::Size& contentSize = getContentSize();
    
    const float itemWidth = contentSize.width * 0.96f;
    const float widthToHeightRatio = 0.2f;
    const float itemHeight = itemWidth * widthToHeightRatio;
    return cocos2d::Size(itemWidth, itemHeight);
}

#pragma mark - Public

void FriendListView::setItems(const FriendList& friendList)
{
    removeAllItems();
    
    const cocos2d::Size& itemSize = calculateItemSize();
    for(const auto& friendData : friendList)
    {
        FriendListViewItem* item = FriendListViewItem::create();
        item->setContentSize(itemSize);
        item->setData(friendData);
        pushBackCustomItem(item);
    }
    _listData = friendList;
    
    const ssize_t itemCount = getItems().size();
    const float itemMargin = getItemsMargin();
    const cocos2d::Size& contentSize = getContentSize();
    
    // Have we filled the size of the list? If not add enough items to fill the height
    const float totalHeight = ((itemMargin + itemSize.height) * itemCount) - itemMargin;
    if(totalHeight < contentSize.height)
    {
        const float heightDiff = contentSize.height - totalHeight;
        float itemsNeeded = heightDiff / (itemSize.height + itemMargin);
        while(itemsNeeded > 0)
        {
            FriendListViewItem* item = FriendListViewItem::create();
            item->setContentSize(itemSize);
            pushBackCustomItem(item);
            itemsNeeded -= 1;
        }
    }
    
    forceDoLayout();
}

void FriendListView::addItemSelectedEventListener(const ItemSelectedCallback& callback)
{
    _selectedEventCallback = callback;
}

NS_AZOOMEE_CHAT_END
