#include "MessageListView.h"
#include "MessageListViewItem.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>


using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN

#pragma mark - Constants

const char* const MessageListView::kEventListenerFlag = "MessageListView_reached_top";

#pragma mark - Methods

bool MessageListView::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Style::Color::black);
    
#ifdef AVATARS_IN_LISTVIEW
    // Setup foreground with the Oomee bar
    _foreground = ui::Layout::create();
    _foreground->setLayoutType(ui::Layout::Type::RELATIVE);
    _foreground->setSizePercent(Vec2(1.0f, 1.0f));
    _foreground->setSizeType(ui::Widget::SizeType::PERCENT);
    
    const float avatarBaseHeight = 140.0f;
    
    // Avatar bar
    _avatarBase = ui::ImageView::create("res/chat/ui/avatar/avatar_base.png");
    _avatarBase->setAnchorPoint(Vec2::ZERO);
    _avatarBase->ignoreContentAdaptWithSize(false); // stretch
    _avatarBase->setContentSize(Size(0.0f, avatarBaseHeight));
    _avatarBase->setLayoutParameter(CreateBottomLeftRelativeLayoutParam());
    _foreground->addChild(_avatarBase);
    
    // Avatars
    const float avatarSize = 300.0f;
    const float avatarShadowWidth = avatarSize * 0.5f;
    const Vec2 avatarMargin = Vec2(30.0f, 60.0f);
    
    // Shadows first
    _avatarShadows[0] = ui::ImageView::create("res/chat/ui/avatar/avatar_shadow.png");
    const Size& shadowTextureSize = _avatarShadows[0]->cocos2d::ui::Widget::getVirtualRendererSize();
    const float widthToHeightAspectRatio = shadowTextureSize.height / shadowTextureSize.width;
    _avatarShadows[0]->setContentSize(Size(avatarShadowWidth, avatarShadowWidth * widthToHeightAspectRatio));
    _avatarShadows[0]->setLayoutParameter(CreateBottomRightRelativeLayoutParam(ui::Margin(0, 0, (avatarMargin.x * 0.75f) + (avatarSize * 0.5f) - (avatarShadowWidth * 0.5f), avatarMargin.y * 0.65f)));
    _foreground->addChild(_avatarShadows[0]);
    
    _avatarShadows[1] = ui::ImageView::create("res/chat/ui/avatar/avatar_shadow.png");
    _avatarShadows[1]->setContentSize(_avatarShadows[0]->getContentSize());
    _avatarShadows[1]->setLayoutParameter(CreateBottomLeftRelativeLayoutParam(ui::Margin((avatarMargin.x * 0.75f) + (avatarSize * 0.5f) - (avatarShadowWidth * 0.5f), 0, 0, avatarMargin.y * 0.65f)));
    _foreground->addChild(_avatarShadows[1]);
    
    // Now the avatars themselves
    _avatars[0] = AvatarWidget::create();
    // First participant (usually current user) is on the RIGHT
    _avatars[0]->setLayoutParameter(CreateBottomRightRelativeLayoutParam(ui::Margin(0, 0, avatarMargin.x, avatarMargin.y)));
    _avatars[0]->setContentSize(Size(avatarSize, avatarSize));
    _foreground->addChild(_avatars[0]);
    
    _avatars[1] = AvatarWidget::create();
    // Second participant is on the LEFT
    _avatars[1]->setLayoutParameter(CreateBottomLeftRelativeLayoutParam(ui::Margin(avatarMargin.x, 0, 0, avatarMargin.y)));
    _avatars[1]->setContentSize(Size(avatarSize, avatarSize));
    _foreground->addChild(_avatars[1]);
#endif
    
    
    // List view for messages
    _listView = ui::ListView::create();
    _listView->setSizePercent(Vec2(1.0f, 1.0f));
    _listView->setSizeType(ui::Widget::SizeType::PERCENT);
    _listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _listView->setBounceEnabled(true);
    _listView->setScrollBarEnabled(false);
    _listView->addEventListener(CC_CALLBACK_2(MessageListView::onScrollEvent, this));
    addChild(_listView);
    
#ifdef AVATARS_IN_LISTVIEW
    // Create the special list view item to allow space for Oomees
    _blankListItem = ui::Layout::create();
    _blankListItem->setContentSize(Size(10, avatarSize + avatarMargin.y));
    _listView->pushBackCustomItem(_blankListItem);
    
    // Add the foreground last
    addChild(_foreground);
#endif
  
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
    
    if(_listView)
    {
        // Resize all items, since they are using absolute sizing
        const auto& items = _listView->getItems();
        const cocos2d::Size& contentSize = _listView->getContentSize();
        for(auto item : items)
        {
            MessageListViewItem* messageItem = (MessageListViewItem*)item;
            
            Size itemSize = messageItem->getContentSize();
            itemSize.width = contentSize.width;
            messageItem->setContentSize(itemSize);
        }
        
#ifdef AVATARS_IN_LISTVIEW
        // Resize avatar base to fill width
        _avatarBase->setContentSize(Size(contentSize.width, _avatarBase->getContentSize().height));
#endif
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
    if(scrollAreaHeight == 0)
    {
        return 0.0f;
    }
    float scrollY = _listView->getInnerContainerPosition().y;
    return 1.0f - (scrollY / scrollAreaHeight);
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

void MessageListView::onScrollEvent(cocos2d::Ref* sender, cocos2d::ui::ScrollView::EventType event)
{
    if((getScrollPosition() < 0.01)&&(_listView->getChildren().size() >= MessageListView::kMessagesOnPage)) //We don't start getting history, if there are less than 20 messages in the container -> the chat has just started, and the user scrolls to the top, or on the top anyways because of not having enough messages to scroll at all.
    {
        EventCustom event(MessageListView::kEventListenerFlag);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
    
#ifdef AVATARS_IN_LISTVIEW
    // Scroll movement
    if(event == ui::ScrollView::EventType::CONTAINER_MOVED)
    {
        // Shift messages around the oomees
        const Vec2& scrollPosition = _listView->getInnerContainerPosition();
        const float oomeeHeight = _blankListItem->getContentSize().height;
        float oomeeTopY = _blankListItem->getContentSize().height - scrollPosition.y;
        // How long it takes before the item has fully transitioned
        const float transitionHeight = oomeeHeight * 0.55f;
        
        // Find any items below the visible line and re-position them
        const cocos2d::Vector<ui::Widget*> items = _listView->getItems();
        for(ssize_t i = items.size() - 1; i >= 0; --i)
        {
            ui::Widget* item = items.at(i);
            if(item == _blankListItem)
            {
                continue;
            }
            
            MessageListViewItem* messageItem = (MessageListViewItem*)item;
            const float distance = oomeeTopY - messageItem->getPositionY();
            if(distance > 0.0f)
            {
                // Scale based on the distance
                const float distancePercentOfHeight = distance / transitionHeight;
                float t = MIN(distancePercentOfHeight, 1.0f);
                // Apply curve
                t = std::sin(t * M_PI * 0.5f);
                t = std::sin(t * M_PI * 0.5f);
                
                const float maxMargin = _avatars[0]->getContentSize().width;
                messageItem->setEdgeMargin(maxMargin * t);
            }
            else
            {
                messageItem->setEdgeMargin(0.0f);
            }
        }
    }
#endif
}

#pragma mark - Public

void MessageListView::setData(const FriendList& participants, const MessageList& messageList)
{
    const float scrollPos = getScrollPosition();
    const float prevScrollHeight = _listView->getInnerContainerSize().height;
    
    _participants = participants;
#ifdef AVATARS_IN_LISTVIEW
    _avatars[0]->setAvatarForFriend(_participants[0]);
    _avatars[1]->setAvatarForFriend(_participants[1]);
#endif
    
    // If messages are zero, we can just remove
    if(messageList.size() == 0)
    {
#ifdef AVATARS_IN_LISTVIEW
        _blankListItem->retain();
        _listView->removeAllItems();
        _listView->pushBackCustomItem(_blankListItem);
        _blankListItem->release();
#else
        _listView->removeAllItems();
#endif
        
        _listData.clear();
    }
    else
    {
        // Update message list
        // Do this inline to avoid a flicker of the UI
        // We just overwrite the content of all UI items here
        const Size& contentSize = _listView->getContentSize();
        
#ifdef AVATARS_IN_LISTVIEW
        _blankListItem->retain();
        _listView->removeLastItem();
#endif
        
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
        
#ifdef AVATARS_IN_LISTVIEW
        // Re-add blank item
        _listView->pushBackCustomItem(_blankListItem);
        _blankListItem->release();
#endif
        
        _listData = messageList;
    }
    
    _listView->doLayout();
    // Force a second layout, to fix issues with overlapping items
    // This probably happens because some list items resize during layout
    _listView->forceDoLayout();
    
    // Scroll to bottom if we have different item size to before
    if(prevScrollHeight != _listView->getInnerContainerSize().height)
    {
        if(scrollPos > 0.9) //inner containerview size is bigger than scrollview size, and already scrolled at the bottom(ish), so new action requires being scrolled to the bottom
        {
            setScrollPosition(1.0f);
        }
        else if((prevScrollHeight <= _listView->getContentSize().height)&&(_listView->getInnerContainerSize().height > _listView->getContentSize().height)) //getting the message that increases inner container view size bigger than scrollview content size (will have 1.0 as scrollpos after this).
        {
            setScrollPosition(1.0f);
        }
        else
        {
            float scrollPositionInPixel = prevScrollHeight * scrollPos;
            float heightDifference = _listView->getInnerContainerSize().height - prevScrollHeight;
            float newScrollPositionInPixel = scrollPositionInPixel + heightDifference;
            float newScrollPositionInPercentage = newScrollPositionInPixel / _listView->getInnerContainerSize().height;
            
            setScrollPosition(newScrollPositionInPercentage); //restore same scroll position with the new size
        }
    }
    else
    {
        // Otherwise restore scroll position
        setScrollPosition(scrollPos);
    }
}

void MessageListView::addMessage(const MessageRef& message)
{
    // Find first item where this message is newer
    MessageList::const_reverse_iterator it = _listData.rbegin();
    size_t insertIndex = _listData.size();
    for(; it != _listData.rend(); ++it)
    {
        if(message->timestamp() > (*it)->timestamp())
        {
            break;
        }
        --insertIndex;
    }
    _listData.insert(it.base(), message);
    
    // Update UI
    MessageListViewItem* item = MessageListViewItem::create();
    const Size& contentSize = _listView->getContentSize();
    
    // Must width before setting data since we pass in 0 height
    item->setContentSize(Size(contentSize.width, 0.0f));
    item->setData(message);
    _listView->insertCustomItem(item, insertIndex);
    
    _listView->doLayout();
    // Force a second layout, to fix issues with overlapping items
    // This probably happens because some list items resize during layout
    _listView->forceDoLayout();
    
    // Scroll to bottom
    // TODO: Only scroll to bottom if the message is new
    setScrollPosition(1.0f);
}

NS_AZOOMEE_CHAT_END
