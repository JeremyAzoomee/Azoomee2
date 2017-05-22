#include "MessageListView.h"
#include "MessageListViewItem.h"
#include <AzoomeeCommon/UI/Style.h>


using namespace cocos2d;



NS_AZOOMEE_CHAT_BEGIN

bool MessageListView::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Style::Color::dustyLavender);
    
    setDirection(ui::ScrollView::Direction::VERTICAL);
    setBounceEnabled(true);
  
    return true;
}

void MessageListView::onEnter()
{
    Super::onEnter();
}

void MessageListView::onExit()
{
    Super::onExit();
}

void MessageListView::doLayout()
{
    Super::doLayout();
}

void MessageListView::onSizeChanged()
{
    Super::onSizeChanged();
    
    // Resize all items, since they are using absolute sizing
    const auto& items = getItems();
    const cocos2d::Size& contentSize = getContentSize();
    for(auto item : items)
    {
        MessageListViewItem* messageItem = (MessageListViewItem*)item;
        
        Size itemSize = messageItem->getContentSize();
        itemSize.width = contentSize.width;
        messageItem->setContentSize(itemSize);
    }
}

void MessageListView::setContentSize(const cocos2d::Size& contentSize)
{
    Super::setContentSize(contentSize);
    
    // TODO: Retain the scroll position relative to the bottom
}

#pragma mark - Public

void MessageListView::setItems(const MessageList& messageList)
{
    const uint64_t prevItemCount = _listData.size();
    removeAllItems();
    
    const Size& contentSize = getContentSize();
    for(const auto& messageData : messageList)
    {
        MessageListViewItem* item = MessageListViewItem::create();
        item->setContentSize(Size(contentSize.width, 0.0f));
        item->setData(messageData);
        pushBackCustomItem(item);
    }
    _listData = messageList;
    
    forceDoLayout();
    
    // Scroll to bottom if we have more items than before
    //if(_listData.size() > prevItemCount)
    {
        jumpToBottom();
    }
}

NS_AZOOMEE_CHAT_END
