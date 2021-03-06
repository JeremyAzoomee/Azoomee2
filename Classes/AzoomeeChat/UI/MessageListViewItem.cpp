#include "MessageListViewItem.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/HQDataObject/ContentItemManager.h>
#include "../Data/StickerCache.h"


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
    
    
    // Speech bubble to hold the text message
    _bubbleLayout = ui::Layout::create();
    _bubbleLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _bubbleLayout->setBackGroundImage("res/chat/ui/message/speach_bubble.png");
    const Size& bgImageSize = _bubbleLayout->getBackGroundImageTextureSize();
    // Calc correct cap insets
    Rect bgInsets;
    bgInsets.origin = Vec2((bgImageSize.height / 2) - 2, (bgImageSize.height / 2) - 2);
    bgInsets.size = Size(bgImageSize.width - (bgInsets.origin.x * 2.0f), MIN(2.0f, bgImageSize.height - (bgInsets.origin.y * 2.0f)));
    _bubbleLayout->setBackGroundImageCapInsets(bgInsets);
    _bubbleLayout->setBackGroundImageScale9Enabled(true);
    _bubbleLayout->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _contentLayout->addChild(_bubbleLayout);
    
    // Text label
    _textLabel = ui::Text::create();
    _textLabel->setFontName(Style::Font::RegularSystemName);
    _textLabel->setFontSize(65.0f);
    _textLabel->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _textLabel->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _bubbleLayout->addChild(_textLabel);
    
    
    // Sticker layout to hold the image
    _stickerLayout = ui::Layout::create();
    _stickerLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _stickerLayout->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _contentLayout->addChild(_stickerLayout);
    
    // Sticker image
    _stickerImage = ui::ImageView::create();
    _stickerImage->ignoreContentAdaptWithSize(false); // stretch the image
    _stickerImage->setAnchorPoint(Vec2(0.5f, 0.5f));
    _stickerImage->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _stickerLayout->addChild(_stickerImage);
    
    // Art layout to hold art image
    _artLayout = ui::Layout::create();
    _artLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _artLayout->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _contentLayout->addChild(_artLayout);
    
    // Art image
    _artImage = RemoteImageSprite::create();
    _artImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _artImage->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _artImage->setKeepAspectRatio(true);
    _artLayout->addChild(_artImage);
    
    // Art Stencil
    //_imageStencil = ui::Scale9Sprite::create("res/artapp/popup_bg.png");
    //_imageStencil->setContentSize(_artImage->getContentSize());
    //_imageStencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    // Art clipping node
    //_imageMask = ClippingNode::create(_imageStencil);
    //_imageMask->setAlphaThreshold(0.5f);
    //_imageMask->addChild(_artImage);
    //_artLayout->addChild(_imageMask);
    
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
    const float maxWidth = getContentSize().width * 0.5f;
    const Vec2& contentPadding = Vec2(50.0f, 20.0f);
    
    if(_bubbleLayout->isVisible())
    {
        // Wordwrap the label and get the height we need for the text
        // Note we always use the landscape resolution width for item calculation
        // The items will overlap slightly on portrait which is what we want
        // TODO: Grab sizes from config
        const float bubblePadding = 70.0f;
        const float bubbleMaxWidth = maxWidth - (contentPadding.x * 2.0f);
        const float labelMaxWidth = bubbleMaxWidth - (bubblePadding * 2.5f);
        
        _textLabel->ignoreContentAdaptWithSize(false); // means fixed size (don't resize with text)
        Label* labelRenderer = dynamic_cast<Label*>(_textLabel->getVirtualRenderer());
        
        // First see if we need to do any word wrap
		labelRenderer->setDimensions(labelMaxWidth, 0);
        Size labelSize = labelRenderer->getContentSize();
        if(labelSize.height <= labelRenderer->getRenderingFontSize())
        {
            // Set fixed width on the label
            labelRenderer->setDimensions(0, 0);
            
            // Now get the height of the label
            labelSize = labelRenderer->getContentSize();
        }
        
        // Resize the elements
        _textLabel->setContentSize(labelSize);
        const Size& bubbleSize = Size(labelSize.width + (bubblePadding * 2.5f), labelSize.height + (bubblePadding * 2.0f));
        _bubbleLayout->setContentSize(bubbleSize);
        const Size& contentSize = Size(bubbleSize.width + (contentPadding.x * 2), bubbleSize.height + (contentPadding.y * 2));
        _contentLayout->setContentSize(contentSize);
    }
    else if(_stickerLayout->isVisible())
    {
        // Resize the sticker, ensuring it keeps it's aspect ratio
        const float maxHeight = 800.0f; // TODO: Get from config
        const Size& textureSize = _stickerImage->getVirtualRendererSize();
        Size imageSize = Size(maxWidth - (contentPadding.x * 2.0f), maxHeight - (contentPadding.y * 2.0f));
        if(textureSize.width > textureSize.height)
        {
            imageSize.height = textureSize.height / textureSize.width * imageSize.width;
        }
        else
        {
            imageSize.width = textureSize.width / textureSize.height * imageSize.height;
        }
        _stickerImage->setContentSize(imageSize);
        _stickerLayout->setContentSize(imageSize);
        const Size& contentSize = Size(imageSize.width + (contentPadding.x * 2), imageSize.height + (contentPadding.y * 2));
        _contentLayout->setContentSize(contentSize);
    }
    else if(_artLayout->isVisible())
    {
        // Resize the sticker, ensuring it keeps it's aspect ratio
        const float maxHeight = 800.0f; // TODO: Get from config
        const Size& textureSize = _artImage->getContentSize();
        Size imageSize = Size(maxWidth - (contentPadding.x * 2.0f), maxHeight - (contentPadding.y * 2.0f));
        if(textureSize.width > textureSize.height)
        {
            imageSize.height = textureSize.height / textureSize.width * imageSize.width;
        }
        else
        {
            imageSize.width = textureSize.width / textureSize.height * imageSize.height;
        }
        const Size& contentSize = Size(imageSize.width + (contentPadding.x * 2), imageSize.height + (contentPadding.y * 2));
        _artImage->setContentSize(imageSize);
        
        //_imageStencil->setContentSize(imageSize);
        //_imageMask->setContentSize(imageSize);
        _artLayout->setContentSize(imageSize);
        _artImage->resizeImage();
        _contentLayout->setContentSize(contentSize);
    }
    
    // Ensure the item height is big enough
    Size itemSize = getContentSize();
    itemSize.height = _contentLayout->getContentSize().height;
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
        
        std::string messageText = "";
        
        if(messageType == Message::MessageTypeText || moderated)
        {
            // TODO: Get moderated text from Strings
            messageText = moderated ? "Message deleted" : message->messageText();
        }
        else if(messageType == Message::MessageTypeSticker)
        {
            // Get the sticker
            const StickerRef& sticker = StickerCache::getInstance()->findStickerByURL(message->stickerURL());
            if(sticker)
            {
                _stickerImage->loadTexture(sticker->imageLocalPath());
            }
            else
            {
                // If we don't recognise the sticker, display a message saying so instead
                // TODO: Get sticker not recognised text from Strings
                messageText = "Sticker not recognised";
            }
        }
        else if(messageType == Message::MessageTypeArt)
        {
            const Size& contentSize = getContentSize();
            _artImage->initWithUrlAndSizeWithoutPlaceholder(message->artURL(), Size(contentSize.width/2,contentSize.width/2 * 10.0f/16.0f));
            _artLayout->setTouchEnabled(false);
        }
        else if(messageType == Message::MessageTypeContent)
        {
            HQContentItemObjectRef contentItem = ContentItemManager::getInstance()->getContentItemForId(message->contentId());
            if(contentItem)
            {
                const std::string& imgUrl = contentItem->getBaseImageThumbUrl();
                const Size& contentSize = getContentSize();
                _artImage->initWithUrlAndSizeWithoutPlaceholder(imgUrl, Size(contentSize.width/2,contentSize.width/2 * 10.0f/16.0f));
                if(!_userIsParent)
                {
                    _artLayout->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType event){
                        if(event == ui::Widget::TouchEventType::ENDED)
                        {
                            Chat::delegate->onChatNavigateToContent(message->contentId());
                        }
                    });
                    _artLayout->setTouchEnabled(true);
                }
            }
        }
        else
        {
            // TODO: Get not supported text from Strings
            messageText = "Message type not supported";
        }
        
        // Update the visiblity of elements depending on the message content
        _textLabel->setString(messageText);
        _bubbleLayout->setVisible(messageText.size() > 0);
        if(!_bubbleLayout->isVisible())
        {
            if(_messageData->messageType() == Message::MessageTypeArt || _messageData->messageType() == Message::MessageTypeContent)
            {
                _artLayout->setVisible(true);
            }
            else
            {
                _artLayout->setVisible(false);
            }
        
            if(_messageData->messageType() == Message::MessageTypeSticker)
            {
                _stickerLayout->setVisible(true);
            }
            else
            {
                _stickerLayout->setVisible(false);
            }
        }
        else
        {
            _stickerLayout->setVisible(false);
            _artLayout->setVisible(false);
        }
        
        // Color depends also on current user
        const std::string& senderId = message->senderId();
        const bool isCurrentUser = (senderId == ChildManager::getInstance()->getParentOrChildId());
        const Color3B& fontColor = (isCurrentUser) ? Style::Color::darkIndigoTwo : Style::Color::white;
        _textLabel->setTextColor(Color4B(fontColor));
        _bubbleLayout->setBackGroundImageColor((isCurrentUser) ? Style::Color::white : Style::Color::darkIndigo);
    }
    else
    {
        _textLabel->setString("");
        _textLabel->setTextColor(Color4B(Style::Color::black));
        _stickerLayout->setVisible(false);
        _bubbleLayout->setVisible(false);
        _artLayout->setVisible(false);
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

void MessageListViewItem::setUserIsParent(bool isParent)
{
    _userIsParent = isParent;
}

bool MessageListViewItem::getUserIsParent() const
{
    return _userIsParent;
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
