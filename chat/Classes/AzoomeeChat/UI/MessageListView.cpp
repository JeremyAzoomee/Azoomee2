#include "MessageListView.h"
#include "MessageListViewItem.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>


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
    
    // Setup background with the Oomee bar
    _background = ui::Layout::create();
    _background->setLayoutType(ui::Layout::Type::RELATIVE);
    _background->setSizePercent(Vec2(1.0f, 1.0f));
    _background->setSizeType(ui::Widget::SizeType::PERCENT);
    addChild(_background);
    
    // Avatar bar
    _avatarBase = ui::ImageView::create("res/chat/ui/avatar/avatar_base.png");
    _avatarBase->setAnchorPoint(Vec2::ZERO);
    _avatarBase->ignoreContentAdaptWithSize(false); // stretch
//    _avatarBase->setScale9Enabled(true);
//    _avatarBase->setCapInsets(Rect(0, 0, _avatarBase->getVirtualRendererSize().width, 0));
    _avatarBase->setLayoutParameter(CreateBottomLeftRelativeLayoutParam());
    _background->addChild(_avatarBase);
    
    // List view for messages
    _listView = ui::ListView::create();
    _listView->setSizePercent(Vec2(1.0f, 1.0f));
    _listView->setSizeType(ui::Widget::SizeType::PERCENT);
    _listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _listView->setBounceEnabled(true);
    addChild(_listView);
    
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
    if(_listView)
    {
        const auto& items = _listView->getItems();
        const cocos2d::Size& contentSize = _listView->getContentSize();
        for(auto item : items)
        {
            MessageListViewItem* messageItem = (MessageListViewItem*)item;
            
            Size itemSize = messageItem->getContentSize();
            itemSize.width = contentSize.width;
            messageItem->setContentSize(itemSize);
        }
        
        // TODO: Get from config
        const float avatarBaseHeight = 140.0f;
        _avatarBase->setContentSize(Size(contentSize.width, avatarBaseHeight));
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
    if(!_listView)
    {
        return 0.0f;
    }
    const float scrollAreaHeight = _listView->getContentSize().height - _listView->getInnerContainerSize().height;
    return 1.0f - (_listView->getInnerContainerPosition().y / scrollAreaHeight);
}

void MessageListView::setScrollPosition(float pos)
{
    if(!_listView)
    {
        return;
    }
    // Ensure a layout is done first, as it can change the innerContainerSize
    _listView->doLayout();
    
    const float scrollAreaHeight = _listView->getContentSize().height - _listView->getInnerContainerSize().height;
    const float scrollY = scrollAreaHeight - (scrollAreaHeight * pos);
    _listView->setInnerContainerPosition(Vec2(0.0f, scrollY));
}

#pragma mark - Public

void MessageListView::setData(const FriendList& participants, const MessageList& messageList)
{
    const float scrollPos = getScrollPosition();
    const float prevScrollHeight = _listView->getInnerContainerSize().height;
    
    _participants = participants;
    
    // If messages are zero, we can just remove
    if(messageList.size() == 0)
    {
        _listView->removeAllItems();
    }
    else
    {
        // Update message list
        // Do this inline to avoid a flicker of the UI
        // We just overwrite the content of all UI items here
        const Size& contentSize = _listView->getContentSize();
        const cocos2d::Vector<ui::Widget*> items = _listView->getItems();
        
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
            else
            {
                // Ran out of UI elements, so we need to add a new one now
                item = MessageListViewItem::create();
                // Must width before setting data since we pass in 0 height
                item->setContentSize(Size(contentSize.width, 0.0f));
                item->setData(message);
                _listView->pushBackCustomItem(item);
            }
            
            // Setup item alignment
            if(item)
            {
                // The participant at 0 is displayed on the RIGHT
                // This is usually expected (but not assumed) to be the current user
                if(message->senderId() == _participants[0]->friendId())
                {
                    item->setAlignment(MessageListViewItem::Alignment::Right);
                }
                else
                {
                    item->setAlignment(MessageListViewItem::Alignment::Left);
                }
            }
        }
        
        // Trim message list
        int64_t numToDelete = _listView->getItems().size() - messageList.size();
        while(numToDelete > 0)
        {
            _listView->removeLastItem();
            --numToDelete;
        }
    }
    
    _listData = messageList;
    _listView->doLayout();
    
    // Scroll to bottom if we have different item size to before
    if(prevScrollHeight != _listView->getInnerContainerSize().height)
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
