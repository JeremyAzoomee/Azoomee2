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
    
    setDirection(ui::ScrollView::Direction::VERTICAL);
    setBounceEnabled(true);
    setGravity(ui::ListView::Gravity::LEFT);
    setItemsMargin(0.0f);
    
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
    
    const float itemWidth = contentSize.width * 1.0f / _columns;
    const float widthToHeightRatio = (_columns == 1) ? 0.175f : 0.25f;
    const float itemHeight = itemWidth * widthToHeightRatio;
    return cocos2d::Size(itemWidth, itemHeight);
}

void FriendListView::interceptTouchEvent(cocos2d::ui::Widget::TouchEventType event, cocos2d::ui::Widget *sender, cocos2d::Touch* touch)
{
    // Override ListView logic so we can deal with items from other columns
    
    // Passing to ScrollView and not Super/ListView is intentional
    ScrollView::interceptTouchEvent(event, sender, touch);
    if(!_touchEnabled)
    {
        return;
    }
    
    if(event != TouchEventType::MOVED)
    {
        // Find the index of the selected item
        Widget* parent = sender;
        FriendListViewItem* item = nullptr;
        while(parent)
        {
            item = dynamic_cast<FriendListViewItem*>(parent);
            if(item)
            {
                const FriendRef& friendData = item->getData();
                if(friendData == nullptr)
                {
                    // If data is null, it's an empty cell and we don't need to continue
                    item = nullptr;
                    break;
                }
                
                for(int i = 0; i < _listData.size(); ++i)
                {
                    if(_listData[i] == friendData)
                    {
                        _curSelectedIndex = i;
                        break;
                    }
                }
                break;
            }
            
            parent = dynamic_cast<Widget*>(parent->getParent());
        }
        
        if(item != nullptr && item->isHighlighted())
        {
            selectedItemEvent(event);
        }
    }
}

#pragma mark - Public

void FriendListView::setColumns(int columns)
{
    if(_columns != columns)
    {
        _columns = columns;
        
        // Recreate the data
        if(_listData.size() > 0)
        {
            setItems(_listData);
        }
    }
}

void FriendListView::setItems(const FriendList& friendList)
{
    removeAllItems();
    
    // Break down list into columns
    std::vector<FriendRef> itemsByColumn[_columns];
    int column = 0;
    for(const auto& friendData : friendList)
    {
        itemsByColumn[column].push_back(friendData);
        if(++column >= _columns)
        {
            column = 0;
        }
    }
    
    // Add items to the list in columns
    const cocos2d::Size& itemSize = calculateItemSize();
    for(int row = 0; row < itemsByColumn[0].size(); ++row)
    {
        // Column 0
        FriendListViewItem* item = FriendListViewItem::create();
        item->setContentSize(itemSize);
        item->setData(itemsByColumn[0][row]);
        pushBackCustomItem(item);
        
        // Add addition columns to the end of the first column
        for(int col = 1; col < _columns; ++col)
        {
            FriendListViewItem* additionalItem = FriendListViewItem::create();
            additionalItem->setContentSize(itemSize);
            
            if(row < itemsByColumn[col].size())
            {
                additionalItem->setData(itemsByColumn[col][row]);
            }
            
            item->addChild(additionalItem);
            additionalItem->setPosition(Vec2(itemSize.width * col, 0.0f));
        }
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
            
            // Add addition columns to the end of the first column
            for(int col = 1; col < _columns; ++col)
            {
                FriendListViewItem* additionalItem = FriendListViewItem::create();
                additionalItem->setContentSize(itemSize);
                item->addChild(additionalItem);
                additionalItem->setPosition(Vec2(itemSize.width * col, 0.0f));
            }
            
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
