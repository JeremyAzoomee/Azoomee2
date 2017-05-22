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
    
//    addEventListener([this](Ref*, ui::ScrollView::EventType event){
//        if(event == ui::ScrollView::EventType::SCROLLING)
//        {
//            // Get current scroll position as %
//            const Size& prevScrollSize = getInnerContainerSize();
//            const Vec2& scrollPosition = getInnerContainerPosition();
//            
//            float scrollAreaHeight = getContentSize().height - prevScrollSize.height;
//            float yPct = scrollPosition.y / scrollAreaHeight;
//            cocos2d::log("MessageListView yPct=%f, scrollPosition=%f", yPct, scrollPosition.y);
//        }
//    });
  
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
    const float scrollPos = getScrollPosition();
    
    Super::setContentSize(contentSize);
    
    // Restore the scroll position
    setScrollPosition(scrollPos);
}

float MessageListView::getScrollPosition() const
{
    const float scrollAreaHeight = getContentSize().height - getInnerContainerSize().height;
    return 1.0f - (getInnerContainerPosition().y / scrollAreaHeight);
}

void MessageListView::setScrollPosition(float pos)
{
    // Ensure a layout is done first, as it can change the innerContainerSize
    doLayout();
    
    const float scrollAreaHeight = getContentSize().height - getInnerContainerSize().height;
    const float scrollY = scrollAreaHeight - (scrollAreaHeight * pos);
    setInnerContainerPosition(Vec2(0.0f, scrollY));
}

#pragma mark - Public

void MessageListView::setData(const MessageList& messageList)
{
    const float scrollPos = getScrollPosition();
    const float prevScrollHeight = getInnerContainerSize().height;
    
    // If messages are zero, we can just remove
    if(messageList.size() == 0)
    {
        removeAllItems();
    }
    else
    {
        // Update message list
        // Do this inline to avoid a flicker of the UI
        // We just overwrite the content of all UI items here
        const Size& contentSize = getContentSize();
        const cocos2d::Vector<ui::Widget*> items = getItems();
        
        for(int i = 0; i < items.size() || i < messageList.size(); ++i)
        {
            MessageListViewItem* item = (i < items.size()) ? (MessageListViewItem*)items.at(i) : nullptr;
            const MessageRef& message = (i < messageList.size()) ? messageList[i] : nullptr;
            
            if(item && message)
            {
                item->setData(message);
            }
            else if(item)
            {
                // If we have UI elements but ran out of messages, we can stop now and delete any remaining UI elements
                break;
            }
            else if(message)
            {
                // Ran out of UI elements, so we need to add a new one now
                item = MessageListViewItem::create();
                // Must width before setting data since we pass in 0 height
                item->setContentSize(Size(contentSize.width, 0.0f));
                item->setData(message);
                pushBackCustomItem(item);
            }
            else
            {
                // We shouldn't get here, but just incase
                break;
            }
        }
        
        // Trim message list
        int64_t numToDelete = getItems().size() - messageList.size();
        while(numToDelete > 0)
        {
            removeLastItem();
            --numToDelete;
        }
    }
    
    _listData = messageList;
    doLayout();
    
    // Scroll to bottom if we have different item size to before
    if(prevScrollHeight != getInnerContainerSize().height)
    {
        setScrollPosition(1.0f);
    }
    else
    {
        // Otherwise restore scroll position
        setScrollPosition(scrollPos);
    }
}

NS_AZOOMEE_CHAT_END
