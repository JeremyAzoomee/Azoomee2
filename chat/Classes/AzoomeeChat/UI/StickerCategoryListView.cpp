#include "StickerCategoryListView.h"
#include "StickerCategoryListViewItem.h"
#include <AzoomeeCommon/UI/Style.h>


using namespace cocos2d;



NS_AZOOMEE_CHAT_BEGIN

bool StickerCategoryListView::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Style::Color::dark);
    
    setDirection(ui::ScrollView::Direction::HORIZONTAL);
    setBounceEnabled(true);
    setGravity(ui::ListView::Gravity::CENTER_VERTICAL);
    
    addEventListener([this](Ref* sender, ui::ListView::EventType type) {
        if(type == ui::ListView::EventType::ON_SELECTED_ITEM_END)
        {
            const int selectedIndex = (int)getCurSelectedIndex();
            
            // Release the selected state of previous item
            const auto& items = getItems();
            for(int i = 0; i < items.size(); ++i)
            {
                if(i == selectedIndex)
                {
                    continue;
                }
                StickerCategoryListViewItem* item = (StickerCategoryListViewItem*) items.at(i);
                item->releaseSelectedState();
            }
            
            if(_selectedEventCallback)
            {
                _selectedEventCallback(_listData[selectedIndex]);
            }
        }
    });
  
    return true;
}

void StickerCategoryListView::onEnter()
{
    Super::onEnter();
}

void StickerCategoryListView::onExit()
{
    Super::onExit();
}

void StickerCategoryListView::doLayout()
{
    Super::doLayout();
}

void StickerCategoryListView::onSizeChanged()
{
    Super::onSizeChanged();
    
    // Resize all items, since they are using absolute sizing
    const auto& items = getItems();
    const cocos2d::Size& contentSize = getContentSize();
    for(auto item : items)
    {
        StickerCategoryListViewItem* listItem = (StickerCategoryListViewItem*)item;
        
        Size itemSize = listItem->getContentSize();
        itemSize.height = contentSize.height;
        listItem->setContentSize(itemSize);
    }
}

void StickerCategoryListView::setContentSize(const cocos2d::Size& contentSize)
{
    Super::setContentSize(contentSize);
}

#pragma mark - Public

void StickerCategoryListView::setItems(const StickerCategoryList& data)
{
    _listData = data;
    
    removeAllItems();
    const Size& contentSize = getContentSize();
    for(const auto& category : _listData)
    {
        StickerCategoryListViewItem* item = StickerCategoryListViewItem::create();
        item->setContentSize(Size(0.0f, contentSize.height));
        item->setData(category);
        pushBackCustomItem(item);
    }
    
    forceDoLayout();
}

void StickerCategoryListView::addItemSelectedEventListener(const ItemSelectedCallback& callback)
{
    _selectedEventCallback = callback;
}

NS_AZOOMEE_CHAT_END
