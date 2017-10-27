//
//  ArtListView.cpp
//  AzoomeeChat
//
//  Created by Macauley on 24/10/2017.
//
//

#include "ArtListView.h"
#include "ArtListItem.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_CHAT_BEGIN

void ArtListView::doLayout()
{
    Super::doLayout();
}
void ArtListView::onSizeChanged()
{
    Super::onSizeChanged();
    
    // Resize all items, since they are using absolute sizing
    const auto& items = getItems();
    const cocos2d::Size& contentSize = getContentSize();
    for(auto item : items)
    {
        ArtListItem* alItem = (ArtListItem*)item;
        
        Size columnSize = alItem->getContentSize();
        columnSize.height = contentSize.height;
        alItem->setContentSize(columnSize);
    }
}

void ArtListView::setItems(const std::vector<std::string>& data)
{
    _artData = data;
    
    removeAllItems();
    
    const Size& contentSize = getContentSize();
    for(int i = 0; i < _artData.size(); i++)
    {
        ArtListItem* item = ArtListItem::create();
        item->setContentSize(Size(0.0f, contentSize.height));
        item->setData(data[i]);
        pushBackCustomItem(item);
    }
    
    forceDoLayout();
}

void ArtListView::addItemSelectedEventListener(const ItemSelectedCallback& callback)
{
    _selectedEventCallback = callback;
}

bool ArtListView::init()
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
    
    addEventListener([this](Ref* sender, ui::ListView::EventType type) {
        if(type == ui::ListView::EventType::ON_SELECTED_ITEM_END)
        {
            const int selectedIndex = (int)getCurSelectedIndex();
            if(_selectedEventCallback)
            {
                _selectedEventCallback(_artData[selectedIndex]);
            }
        }
    });

    
    return true;
}

void ArtListView::onEnter()
{
    Super::onEnter();
}

void ArtListView::onExit()
{
    Super::onExit();
}

void ArtListView::setContentSize(const cocos2d::Size& contentSize)
{
    Super::setContentSize(contentSize);
}


NS_AZOOMEE_CHAT_END
