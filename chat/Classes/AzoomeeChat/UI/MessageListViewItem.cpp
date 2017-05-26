#include "MessageListViewItem.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>


using namespace cocos2d;



NS_AZOOMEE_CHAT_BEGIN

bool MessageListViewItem::init()
{
    if(!Super::init())
    {
        return false;
    }

    setLayoutType(ui::Layout::Type::RELATIVE);
    
    // Content layer, holds the content and positions itself inside the item based
    // on whether the message is to or from the current user.
    _contentLayout = ui::Layout::create();
    _contentLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _contentLayout->setLayoutParameter(CreateLeftCenterRelativeLayoutParam());
    addChild(_contentLayout);
    
    
    // Text type
    _textLayout = ui::Layout::create();
    _textLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _textLayout->setBackGroundImage("res/chat/ui/message/speach_bubble.png");
    _textLayout->setBackGroundImageCapInsets(Rect(67, 67, 329, 2));
    _textLayout->setBackGroundImageScale9Enabled(true);
    _textLayout->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _contentLayout->addChild(_textLayout);
    
    _textLabel = ui::Text::create();
    _textLabel->setFontName(Style::Font::Regular);
    _textLabel->setFontSize(65.0f);
    _textLabel->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _textLabel->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _textLayout->addChild(_textLabel);
    
    
    // By default setup content as blank
    setAlignment(Alignment::Left);
    setData(nullptr);
  
    return true;
}

void MessageListViewItem::onEnter()
{
    Super::onEnter();
}

void MessageListViewItem::onExit()
{
    Super::onExit();
}

void MessageListViewItem::doLayout()
{
    Super::doLayout();
}

void MessageListViewItem::setContentSize(const cocos2d::Size& contentSize)
{
    const float oldWidth = getContentSize().width;
    Super::setContentSize(contentSize);
    
    // Do we need to resize the item contents?
    if(contentSize.width != oldWidth)
    {
        // Must be called after Super::setContentSize
        resizeItemContents();
    }
}

void MessageListViewItem::resizeItemContents()
{
    // Ensure we always have 1 character, so the height is calculated correctly
    if(_textLabel->getString().size() == 0)
    {
        _textLabel->setString(" ");
    }
    
    // Wordwrap the label and get the height we need for the text
    // Note we always use the landscape resolution width for item calculation
    // The items will overlap slightly on portrait which is what we want
    // TODO: Grab sizes from config
    const float maxWidth = getContentSize().width * 0.5f;
    const float bubblePadding = 40.0f;
    const Vec2& messageContentPadding = Vec2(50.0f, 20.0f);
    const float bubbleMaxWidth = maxWidth - (messageContentPadding.x * 2.0f);
    const float labelMaxWidth = bubbleMaxWidth - (bubblePadding * 2.0f);
    
    _textLabel->ignoreContentAdaptWithSize(false); // means fixed size (don't resize with text)
    Label* labelRenderer = dynamic_cast<Label*>(_textLabel->getVirtualRenderer());
    
    // First see if we need to do any word wrap
    labelRenderer->setDimensions(0, 0);
    Size labelSize = labelRenderer->getContentSize();
    if(labelSize.width > labelMaxWidth)
    {
        // Set fixed width on the label
        labelRenderer->setDimensions(labelMaxWidth, 0);
        
        // Now get the height of the label
        labelSize = labelRenderer->getContentSize();
    }
    
    // Resize the elements
    _textLabel->setContentSize(labelSize);
    const Size& bubbleSize = Size(labelSize.width + (bubblePadding * 2.0f), labelSize.height + (bubblePadding * 2.0f));
    _textLayout->setContentSize(bubbleSize);
    const Size& contentSize = Size(bubbleSize.width + (messageContentPadding.x * 2), bubbleSize.height + (messageContentPadding.y * 2));
    _contentLayout->setContentSize(contentSize);
    
    // Ensure the item height is big enough
    Size itemSize = getContentSize();
    itemSize.height = contentSize.height;
    setContentSize(itemSize);
    
    // Force a layout to make sure elements are aligned correctly inside
    // This is especially needed for when an existing item has been updated
    forceDoLayout();
}

#pragma mark - Public

void MessageListViewItem::setData(const MessageRef& message)
{
    _messageData = message;
    
    if(_messageData)
    {
        // Update content elements
        const std::string& messageType = message->messageType();
        const bool moderated = message->moderated();
        if(messageType == "TEXT" || moderated)
        {
            // TODO: Get moderated text from Strings
            _textLabel->setString((moderated ? "Message deleted" : message->messageText()));
        }
        else
        {
            // Message type not supported yet
            _textLabel->setString("Message type not supported");
        }
        
        // Color depends also on current user
        const std::string& senderId = message->senderId();
        const bool isCurrentUser = (senderId == ChildDataProvider::getInstance()->getLoggedInChildId());
        const Color3B& fontColor = (isCurrentUser) ? Style::Color::kermitGreen : Style::Color::barney;
        _textLabel->setTextColor(Color4B(fontColor));
    }
    else
    {
        _textLabel->setString("");
        _textLabel->setTextColor(Color4B(Style::Color::black));
    }
    
    if(getContentSize().width > 0)
    {
        resizeItemContents();
    }
}

MessageRef MessageListViewItem::getData() const
{
    return _messageData;
}

void MessageListViewItem::setAlignment(const Alignment& alignment)
{
    _alignment = alignment;
    
    ui::RelativeLayoutParameter* contentLayoutParam = (ui::RelativeLayoutParameter*) _contentLayout->getLayoutParameter();
    if(_alignment == Alignment::Right)
    {
        contentLayoutParam->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    }
    else
    {
        contentLayoutParam->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    }
    
    // Update margin
    setEdgeMargin(_margin);
}

void MessageListViewItem::setEdgeMargin(float marginFloat)
{
    _margin = marginFloat;
    ui::Margin margin;
    if(_alignment == Alignment::Right)
    {
        margin.right = marginFloat;
    }
    else
    {
        margin.left = marginFloat;
    }
    
    ui::RelativeLayoutParameter* contentLayoutParam = (ui::RelativeLayoutParameter*) _contentLayout->getLayoutParameter();
    contentLayoutParam->setMargin(margin);
    
    forceDoLayout();
}

NS_AZOOMEE_CHAT_END
