//
//  ArtListView.cpp
//  AzoomeeChat
//
//  Created by Macauley on 24/10/2017.
//
//

#include "ArtListView.h"
#include "ArtListColumn.h"
#include <TinizineCommon/UI/Colour.h>

using namespace cocos2d;

USING_NS_TZ

NS_AZ_CHAT_BEGIN

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
        ArtListColumn* alItem = (ArtListColumn*)item;
        
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
    const float itemWidth = 2732 / 3.5f;
    for(int i = 0; i < _artData.size(); i+=2)
    {
        ArtListColumn* item = ArtListColumn::create();
        item->setContentSize(Size(itemWidth,contentSize.height));
        item->setData(_artData, i);
        item->enableOnScreenChecker();
        item->addItemSelectedEventListener([this](const std::string& data) {
            if(_selectedEventCallback)
            {
                _selectedEventCallback(data);
            }
        });
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
    setBackGroundColor(Colours::Color_3B::darkIndigoTwo);
    
    setDirection(ui::ScrollView::Direction::HORIZONTAL);
    setBounceEnabled(true);
    setScrollBarEnabled(false);
    setGravity(ui::ListView::Gravity::CENTER_VERTICAL);
    
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


NS_AZ_CHAT_END
