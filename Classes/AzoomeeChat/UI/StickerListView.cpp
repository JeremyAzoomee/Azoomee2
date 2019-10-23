#include "StickerListView.h"
#include "StickerListViewColumn.h"
#include <AzoomeeCommon/UI/Style.h>


using namespace cocos2d;



NS_AZOOMEE_CHAT_BEGIN

bool StickerListView::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Style::Color::black);
    
    setDirection(ui::ScrollView::Direction::HORIZONTAL);
    setBounceEnabled(true);
    setScrollBarEnabled(false);
    setGravity(ui::ListView::Gravity::CENTER_VERTICAL);
  
    return true;
}

void StickerListView::onEnter()
{
    Super::onEnter();
}

void StickerListView::onExit()
{
    Super::onExit();
}

void StickerListView::doLayout()
{
    Super::doLayout();
}

void StickerListView::onSizeChanged()
{
    Super::onSizeChanged();
    
    // Resize all items, since they are using absolute sizing
    const auto& items = getItems();
    const cocos2d::Size& contentSize = getContentSize();
    for(auto item : items)
    {
        StickerListViewColumn* column = (StickerListViewColumn*)item;
        
        Size columnSize = column->getContentSize();
        columnSize.height = contentSize.height;
        column->setContentSize(columnSize);
    }
}

void StickerListView::setContentSize(const cocos2d::Size& contentSize)
{
    Super::setContentSize(contentSize);
}

#pragma mark - Public

void StickerListView::setItems(const StickerList& data)
{
    _listData = data;
    
    removeAllItems();
    
    const Size& contentSize = getContentSize();
    const float itemWidth = 2732 / 8.0f; // widest design resolution size by 8 items
    for(int i = 0; i < _listData.size(); i += 2)
    {
        StickerListViewColumn* item = StickerListViewColumn::create();
        item->setContentSize(Size(itemWidth, contentSize.height));
        item->setData(data, i);
        item->addItemSelectedEventListener([this](const StickerRef& sticker) {
            if(_selectedEventCallback)
            {
                _selectedEventCallback(sticker);
            }
        });
        pushBackCustomItem(item);
    }
    
    forceDoLayout();
}

void StickerListView::addItemSelectedEventListener(const ItemSelectedCallback& callback)
{
    _selectedEventCallback = callback;
}

NS_AZOOMEE_CHAT_END
