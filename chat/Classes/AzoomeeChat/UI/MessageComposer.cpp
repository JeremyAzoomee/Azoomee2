#include "MessageComposer.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Strings.h>


using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN

const char* const kEstimatedKeyboardHeightPortrait = "Azoomee::MessageComposer::EstimatedKeyboardHeight/Portrait";
const char* const kEstimatedKeyboardHeightLandscape = "Azoomee::MessageComposer::EstimatedKeyboardHeight/Landscape";
const int kResizeAnimationTag = 10001;

bool MessageComposer::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Style::Color::dark);
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    setSizeType(ui::Widget::SizeType::PERCENT);
    setSizePercent(Vec2(1.0f, 0.0f));
    
    // Top layout holds cancel button, text field and other tabs
    _topLayout = SplitLayout::create();
    addChild(_topLayout);
    
    // Calculate the height of the top bar
    Texture2D* textEntryTex = _director->getTextureCache()->addImage("res/chat/ui/textfield/message_field.png");
    if(textEntryTex)
    {
        // TODO: Get from config
        const float textEntryHeight = textEntryTex->getContentSize().height;
        const float textEntryMargin = 40.0f;
        const float totalHeight = textEntryHeight + (textEntryMargin * 2.0f);
        // Width will get updated later, so just use 0 for now
        _topLayout->setContentSize(Size(0.0f, totalHeight));
        _currentHeight = totalHeight;
    }
    
    createTopUIContent(_topLayout);
    
    // Sticker selector sits under the top
    _stickerSelector = StickerSelector::create();
    _stickerSelector->setTabBarHeight(_topLayout->getContentSize().height);
    _stickerSelector->setLayoutParameter(CreateTopLinearLayoutParam());
    _stickerSelector->addStickerSelectedEventListener([this](const StickerRef& sticker) {
        // Sticker selected
        sendMessage(sticker);
    });
    addChild(_stickerSelector);
    
    // Default to Idle
    setMode(MessageComposer::Mode::Idle);
    return true;
}

void MessageComposer::onEnter()
{
    Super::onEnter();
}

void MessageComposer::onExit()
{
    Super::onExit();
    
    setMode(MessageComposer::Mode::Idle);
}

#pragma mark - Size changes

void MessageComposer::onSizeChanged()
{
    Super::onSizeChanged();
    
    // Resize absolute sized layouts
    // TODO: Can we build an Adaptor of some kind to automate this?
    std::vector<ui::Widget*> resizeItemsMatchParentWidth = {
        _topLayout,
        _messageEntryLayout
    };
    
    for(auto widget : resizeItemsMatchParentWidth)
    {
        const Size& parentContentSize = widget->getParent()->getContentSize();
        Size itemContentSize = widget->getContentSize();
        if(itemContentSize.width != parentContentSize.width)
        {
            itemContentSize.width = parentContentSize.width;
            widget->setContentSize(itemContentSize);
        }
    }
    
    // Update the sticker selector to use the latest keyboard height we have
    // At the same time, resize to fill the width of it's parent
    const float keyboardHeight = getEstimatedKeyboardHeight();
    _stickerSelector->setContentSize(Size(_stickerSelector->getParent()->getContentSize().width, keyboardHeight));
    
    // Size the text entry field
    const Size& textEntryMaxSize = _messageEntryField->getParent()->getContentSize();
    const auto& textEntryMargin = _messageEntryField->getLayoutParameter()->getMargin();
    const float textEntryWidth = textEntryMaxSize.width - textEntryMargin.left - textEntryMargin.right;
    _messageEntryField->setContentSize(Size(textEntryWidth, textEntryMaxSize.height));
}

void MessageComposer::setContentSize(const cocos2d::Size& contentSize)
{
    if(_topLayout)
    {
        // Check if the keyboard height will be different
        const float keyboardHeight = getEstimatedKeyboardHeight();
        if(_lastKeyboardHeight > 0.0f && keyboardHeight != _lastKeyboardHeight)
        {
            _lastKeyboardHeight = keyboardHeight;
            _currentHeight = keyboardHeight + _topLayout->getContentSize().height - Director::getInstance()->getVisibleOrigin().y;
        }
        
        cocos2d::Size correctedSize = contentSize;
        correctedSize.height = _currentHeight;
        Super::setContentSize(correctedSize);
    }
    else
    {
        Super::setContentSize(contentSize);
    }
}

void MessageComposer::resizeUIForKeyboard(float keyboardHeight, float duration)
{
    cocos2d::log("MessageComposer::resizeUIForKeyboard: %f, duration=%f", keyboardHeight, duration);
    
    // Resize the composer so the TextField stays in view
    // Stop any current action so we don't get overlap/conflicts
    stopActionByTag(kResizeAnimationTag);
    
    _lastKeyboardHeight = keyboardHeight;
    
    // Calculate what height we need
    // Take into account screen cropping
    if(keyboardHeight > 0)
    {
        keyboardHeight -= Director::getInstance()->getVisibleOrigin().y;
    }
    const float wantHeight = keyboardHeight + _topLayout->getContentSize().height;
    const Size& contentSize = getContentSize();
    
    // Animate or not?
    if(duration > 0.0f)
    {
        // Animate
        ActionFloat* animation = ActionFloat::create(duration, contentSize.height, wantHeight, [this](float height) {
            _currentHeight = height;
            // Note - getContentSize() is important, it could change during animation
            // Don't use contentSize from outside the action
            setContentSize(Size(getContentSize().width, height));
        });
        animation->setTag(kResizeAnimationTag);
        runAction(animation);
    }
    else
    {
        // Immediate
        _currentHeight = wantHeight;
        setContentSize(Size(contentSize.width, _currentHeight));
    }
}

float MessageComposer::getEstimatedKeyboardHeight() const
{
    const Size& nativeScreenSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    const bool currentOrientationPortrait = (nativeScreenSize.height > nativeScreenSize.width);
    
    // Do we have a value in user defaults?
    float height = UserDefault::getInstance()->getFloatForKey(currentOrientationPortrait ? kEstimatedKeyboardHeightPortrait : kEstimatedKeyboardHeightLandscape);
    if(height == 0)
    {
        // If no value stored, calculate a sensible default based on the native device's
        // screen aspect ratio.
        // It's worth noting we override these defaults later when we get an actual
        // keyboard height from the IME, so we're just getting a value as close as we can.
        
        //Keyboard height based on width
        //
        //iPad (4:3):
        //    width = 2732, height = 2048
        //    keyboard height landscape = 1061.333374
        //    - landscape = 1061.33 / 2732 = 0.388 < using this
        //    - landscape = 1061.33 / 2048 = 0.518
        //    width = 2048, height = 2732
        //    keyboard height portrait = 835.333374
        //    - portrait = 835.33 / 2048 = 0.407 < using this
        //    - portrait = 835.33 / 2732 = 0.305
        //    
        //iPhone 6 (16:9):
        //    width = 2732, height = 1792
        //    keyboard height landscape = 1050.623657
        //    - landscape = 1050.62 / 2732 = 0.3845 < using this
        //    - landscape = 1050.62 / 1792 = 0.586
        //    width = 1792, height = 2732
        //    keyboard height portrait = 1056.755615
        //    - portrait = 1056.75 / 1792 = 0.589 < using this
        //    - portrait = 1056.75 / 2732 = 0.386

        
        // Make sure we have portrait for aspect ratio calc
        Size nativeScreenSizePortait = nativeScreenSize;
        if(nativeScreenSize.width > nativeScreenSize.height)
        {
            nativeScreenSizePortait.width = nativeScreenSize.height;
            nativeScreenSizePortait.height = nativeScreenSize.width;
        }
        const float aspectRatio = nativeScreenSizePortait.height / nativeScreenSizePortait.width;
        
        // x = landscape % of width, y = portrait % of width
        // Remember in landscape width = portrait height
        Vec2 keyboardHeightPct;
        keyboardHeightPct.x = 0.388; // landscape
        
        if(aspectRatio >= 1.32 && aspectRatio <= 1.34) // 4:3 (1.33) approx
        {
            // Values taken from iPad keyboard height
            keyboardHeightPct.y = 0.407; // portrait
        }
        else if(aspectRatio >= 1.76 && aspectRatio <= 1.79) // 16:9 (1.77) approx
        {
            // Values taken from iPhone keyboard height
            keyboardHeightPct.y = 0.589; // portrait
        }
        else if(aspectRatio >= 1.65 && aspectRatio <= 1.68) // 5:3 (1.6667) approx
        {
            // Same as 16:9
            keyboardHeightPct.y = 0.589; // portrait
        }
        else if(aspectRatio >= 1.59 && aspectRatio <= 1.61) // 16:10/8:5 (1.6) approx
        {
            // TODO: Check some Fire devices (usually 8:5) for some good default values
            // Same as 16:9 for now
            keyboardHeightPct.y = 0.589; // portrait
        }
        else
        {
            // Anything else
            // Use the 4:3 height and transform it by the aspectRatio
            keyboardHeightPct.y = 0.407 / 1.33 * aspectRatio; // portrait
        }
        
        const Vec2& visibleOrigin = Director::getInstance()->getVisibleOrigin();
        Size portraitSize = Director::getInstance()->getVisibleSize();
        portraitSize.width += visibleOrigin.x;
        portraitSize.height += visibleOrigin.y;
        // Make sure we've got portrait values
        if(portraitSize.width > portraitSize.height)
        {
            const float width = portraitSize.height;
            portraitSize.height = portraitSize.width;
            portraitSize.width = width;
        }
        
        // Save it so we don't need to calc again
        const float heightPortrait = portraitSize.width * keyboardHeightPct.y;
        const float heightLandscape = portraitSize.height * keyboardHeightPct.x;
        // Check before we save, we don't overwrite any legit values
        if(UserDefault::getInstance()->getFloatForKey(kEstimatedKeyboardHeightPortrait) == 0)
        {
            UserDefault::getInstance()->setFloatForKey(kEstimatedKeyboardHeightPortrait, heightPortrait);
        }
        if(UserDefault::getInstance()->getFloatForKey(kEstimatedKeyboardHeightLandscape) == 0)
        {
            UserDefault::getInstance()->setFloatForKey(kEstimatedKeyboardHeightLandscape, heightLandscape);
        }
        
        return (currentOrientationPortrait) ? heightPortrait : heightLandscape;
    }
    return height;
}

void MessageComposer::setEstimatedKeyboardHeight(float height)
{
    // Ignore tiny values
    // Sometimes device reports a 0 or small height (e.g Android devices can report the size of the
    // black bottom bar which sometimes appears with or without the keyboard).
    if(height < 100.0f)
    {
        return;
    }
    
    const Size& nativeScreenSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    const bool currentOrientationPortrait = (nativeScreenSize.height > nativeScreenSize.width);
    
    if(currentOrientationPortrait)
    {
        UserDefault::getInstance()->setFloatForKey(kEstimatedKeyboardHeightPortrait, height);
    }
    else
    {
        UserDefault::getInstance()->setFloatForKey(kEstimatedKeyboardHeightLandscape, height);
    }
}

#pragma mark - Public

void MessageComposer::setDelegate(MessageComposer::Delegate* delegate)
{
    _delegate = delegate;
}

#pragma mark - Send message

void MessageComposer::sendMessage(const std::string& message)
{
    if(_delegate)
    {
        const std::string& trimmedMessage = Azoomee::trim(message);
        if(trimmedMessage.length() > 0)
        {
            const MessageRef& messageObj = Message::createTextMessage(trimmedMessage);
            _delegate->onMessageComposerSendMessage(messageObj);
        }
    }
    
    // Clear the message field and go back to idle
    _messageEntryField->setString("");
    setMode(MessageComposer::Mode::Idle);
}

void MessageComposer::sendMessage(const StickerRef& sticker)
{
    if(_delegate)
    {
        const MessageRef& messageObj = Message::createStickerMessage(sticker);
        _delegate->onMessageComposerSendMessage(messageObj);
    }
    
    // Clear the message field and go back to idle
    _messageEntryField->setString("");
    setMode(MessageComposer::Mode::Idle);
}

#pragma mark - Mode

MessageComposer::Mode MessageComposer::currentMode() const
{
    return _currentMode;
}

void MessageComposer::setMode(MessageComposer::Mode mode)
{
    Mode oldMode = _currentMode;
    _currentMode = mode;
    
    // Update UI elements
    _cancelButton->setVisible(_currentMode != MessageComposer::Mode::Idle);
    _sendButton->setVisible(_currentMode != MessageComposer::Mode::Idle);
    
    
    // Stickers enter/exit
    if(_currentMode == MessageComposer::Mode::StickersEntry)
    {
        // Stickers button requires some resizing when selected, since the aspect ratio of the
        // selected and normal images is different
        _stickersTab->loadTextureNormal("res/chat/ui/buttons/stickers_active.png");
        const Size& textureSize = _stickersTab->getRendererNormal()->getTexture()->getContentSize();
        const float buttonWidth = _stickersTab->getContentSize().width;
        const float buttonHeight = textureSize.height / textureSize.width * buttonWidth;
        _stickersTab->setContentSize(Size(buttonWidth, buttonHeight));
        
        // Update the sticker selector to use the latest keyboard height we have
        const float keyboardHeight = getEstimatedKeyboardHeight();
        _stickerSelector->setContentSize(Size(_stickerSelector->getContentSize().width, keyboardHeight));
    }
    else
    {
        // Restore the stickers tab
        _stickersTab->loadTextureNormal("res/chat/ui/buttons/stickers_btn.png");
        const Size& textureSize = _stickersTab->getRendererNormal()->getTexture()->getContentSize();
        const float buttonWidth = _stickersTab->getContentSize().width;
        const float buttonHeight = textureSize.height / textureSize.width * buttonWidth;
        _stickersTab->setContentSize(Size(buttonWidth, buttonHeight));
    }
    
    
    // TextEntry enter/exit
    if(_currentMode == MessageComposer::Mode::TextEntry)
    {
        _messageEntryLayout->setBackGroundImage("res/chat/ui/textfield/message_field_active.png");
        
        // Make sure the close buttons sits above the message entry layer in terms of Z
        // If we don't do this, the button won't get the click event because it's intercepted
        // by a modal layer from the text entry which fills the screen
        // We don't have to worry about other buttons because they are added after in the hierarchy
        _cancelButton->setGlobalZOrder(_messageEntryLayout->getGlobalZOrder() + 1);
    }
    else
    {
        _messageEntryLayout->setBackGroundImage("res/chat/ui/textfield/message_field.png");
        
        // Make sure keyboard dismissed
        if(_imeOpen)
        {
            _messageEntryField->didNotSelectSelf();
            ui::UICCTextField* textFieldRenderer = dynamic_cast<ui::UICCTextField*>(_messageEntryField->getVirtualRenderer());
            textFieldRenderer->closeIME();
        }
    }
    
    
    // Resize the view appropriately
    
    // Stickers enter state
    if(_currentMode == MessageComposer::Mode::StickersEntry && oldMode != MessageComposer::Mode::StickersEntry)
    {
        // Match the normal keyboard height or if that isn't available, an estimated height
        const float keyboardHeight = getEstimatedKeyboardHeight();
        resizeUIForKeyboard(keyboardHeight, 0.25f);
    }
    // Idle enter from Stickers or Gallery
    // We ignore if old mode was TextEntry because the view is resized based on keyboard IME events
    else if(_currentMode == MessageComposer::Mode::Idle && oldMode != MessageComposer::Mode::TextEntry)
    {
        resizeUIForKeyboard(0.0f, 0.25f);
    }
}

#pragma mark - TextField

void MessageComposer::onTextFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type)
{
    switch(type)
    {
        case ui::TextField::EventType::ATTACH_WITH_IME:
        {
            // Override the textField delegate so we can capture return key
            ui::UICCTextField* textFieldRenderer = (ui::UICCTextField*)_messageEntryField->getVirtualRenderer();
            textFieldRenderer->setDelegate(this);
            break;
        }
        case ui::TextField::EventType::DETACH_WITH_IME:
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            // On Android we must deal with hiding the keyboard here, because sometimes
            // IMEKeyboardNotificationInfo methods do not get called (sigh)
            // TODO: Find a more generic way to fix this
            if(_imeOpen)
            {
                cocos2d::IMEKeyboardNotificationInfo imeNotification;
                imeNotification.end = cocos2d::Rect(0, 0, 0, height);
                imeNotification.duration = 0.25f;
                keyboardWillHide(imeNotification);
            }
#endif
            break;
        }
        case ui::TextField::EventType::INSERT_TEXT:
        {
            // TextField changed
            break;
        }
        case ui::TextField::EventType::DELETE_BACKWARD:
        default:
        {
            // Nothing to do
            break;
        }
    }
}

#pragma mark - TextFieldDelegate

bool MessageComposer::onTextFieldAttachWithIME(cocos2d::TextFieldTTF* sender)
{
    ui::UICCTextField* textFieldRenderer = (ui::UICCTextField*)_messageEntryField->getVirtualRenderer();
    return textFieldRenderer->onTextFieldAttachWithIME(sender);
}

bool MessageComposer::onTextFieldDetachWithIME(cocos2d::TextFieldTTF* sender)
{
    ui::UICCTextField* textFieldRenderer = (ui::UICCTextField*)_messageEntryField->getVirtualRenderer();
    return textFieldRenderer->onTextFieldDetachWithIME(sender);
}

bool MessageComposer::onTextFieldInsertText(cocos2d::TextFieldTTF* sender, const char* text, size_t nLen)
{
    // Capture return key
    if(std::strcmp(text, "\n") == 0)
    {
        // Send the message
        sendMessage(_messageEntryField->getString());
        return true;
    }
    
    ui::UICCTextField* textFieldRenderer = (ui::UICCTextField*)_messageEntryField->getVirtualRenderer();
    return textFieldRenderer->onTextFieldInsertText(sender, text, nLen);
}

bool MessageComposer::onTextFieldDeleteBackward(cocos2d::TextFieldTTF* sender, const char* delText, size_t nLen)
{
    ui::UICCTextField* textFieldRenderer = (ui::UICCTextField*)_messageEntryField->getVirtualRenderer();
    return textFieldRenderer->onTextFieldDeleteBackward(sender, delText, nLen);
}

bool MessageComposer::onVisit(cocos2d::TextFieldTTF* sender, cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
    ui::UICCTextField* textFieldRenderer = (ui::UICCTextField*)_messageEntryField->getVirtualRenderer();
    return textFieldRenderer->onVisit(sender, renderer, transform, flags);
}

#pragma mark - IMEDelegate

void MessageComposer::keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info)
{
    // We can get events even when scene is not shown
    if(!isVisible())
    {
        return;
    }
    
    _imeOpen = true;
    setMode(MessageComposer::Mode::TextEntry);
    setEstimatedKeyboardHeight(info.end.size.height);
    resizeUIForKeyboard(info.end.size.height, info.duration);
}

void MessageComposer::keyboardDidShow(cocos2d::IMEKeyboardNotificationInfo& info)
{
    // We can get events even when scene is not shown
    if(!isVisible())
    {
        return;
    }
    
    _imeOpen = true;
    setMode(MessageComposer::Mode::TextEntry);
    setEstimatedKeyboardHeight(info.end.size.height);
    resizeUIForKeyboard(info.end.size.height, 0);
}

void MessageComposer::keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info)
{
    // We can get events even when scene is not shown
    if(!isVisible())
    {
        return;
    }
    
    _imeOpen = false;
    if(_currentMode == MessageComposer::Mode::Idle || _currentMode == MessageComposer::Mode::TextEntry)
    {
        setMode(MessageComposer::Mode::Idle);
        resizeUIForKeyboard(0, info.duration);
    }
}

void MessageComposer::keyboardDidHide(cocos2d::IMEKeyboardNotificationInfo& info)
{
    // We can get events even when scene is not shown
    if(!isVisible())
    {
        return;
    }
    
    _imeOpen = false;
    if(_currentMode == MessageComposer::Mode::Idle || _currentMode == MessageComposer::Mode::TextEntry)
    {
        setMode(MessageComposer::Mode::Idle);
        resizeUIForKeyboard(0, 0);
    }
}

#pragma mark - UI Creation

void MessageComposer::createTopUIContent(SplitLayout* parent)
{
    // Top layout has the right side fixed, the left fills the space
    // { [LEFT CONTENT----->][TABS] }
    parent->setMode(SplitLayout::Mode::Horizontal);
    parent->setSplitBehaviour(SplitLayout::FillSize, SplitLayout::FixedSize);
    
    // Left side has another split, which holds the cancel button, and then the message entry
    // { [CLOSE BUTTON][<------MESSAGE ENTRY UI] }
    ui::Layout* firstLayout = parent->firstLayout();
    SplitLayout* firstLayoutContent = SplitLayout::create();
    // Fill the size of the first column
    firstLayoutContent->setSizeType(ui::Widget::SizeType::PERCENT);
    firstLayoutContent->setSizePercent(Vec2(1.0f, 1.0f));
    firstLayoutContent->setMode(SplitLayout::Mode::Horizontal);
    firstLayoutContent->setSplitBehaviour(SplitLayout::FixedSize, SplitLayout::FillSize);
    firstLayout->addChild(firstLayoutContent);
    createCancelButton(firstLayoutContent->firstLayout());
    createMessageEntryUI(firstLayoutContent->secondLayout());
    
    
    // Right side is a fixed size and holds the tabs for other message entry modes
    createTabButtonsUI(parent->secondLayout());
}

void MessageComposer::createTabButtonsUI(cocos2d::ui::Layout* parent)
{
    parent->setLayoutType(ui::Layout::Type::HORIZONTAL);
    
    const float tabMarginX = 60.0f;
    const float leftMarginX = 80.0f;
    const float buttonHeight = _topLayout->getContentSize().height * 0.75f;
    
    _stickersTab = ui::Button::create("res/chat/ui/buttons/stickers_btn.png");
    // Enable content adaption - otherwise % size doesn't work
    _stickersTab->ignoreContentAdaptWithSize(false);
    _stickersTab->getRendererNormal()->setStrechEnabled(true);
    _stickersTab->setAnchorPoint(Vec2(0.0f, 1.0f));
    _stickersTab->setPressedActionEnabled(false); // Disable pressed "zooming"
    _stickersTab->setContentSize(Size(buttonHeight, buttonHeight));
    _stickersTab->setLayoutParameter(CreateLeftLinearLayoutParam(ui::Margin(leftMarginX, (_topLayout->getContentSize().height - buttonHeight) / 2, 0, 0)));
    _stickersTab->addClickEventListener([this](Ref* button){
        setMode(MessageComposer::Mode::StickersEntry);
    });
    parent->addChild(_stickersTab);
    
    _galleryTab = ui::Button::create("res/chat/ui/buttons/art_btn.png");
    // Enable content adaption - otherwise % size doesn't work
    _galleryTab->ignoreContentAdaptWithSize(false);
    _galleryTab->getRendererNormal()->setStrechEnabled(true);
    _galleryTab->setAnchorPoint(Vec2(0.0f, 1.0f));
    _galleryTab->setContentSize(Size(buttonHeight, buttonHeight));
    _galleryTab->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(tabMarginX, 0, 0, 0)));
    _galleryTab->addClickEventListener([this](Ref* button){
        // Set to idle until we support gallery feature
        setMode(MessageComposer::Mode::Idle);
    });
    parent->addChild(_galleryTab);
    
    // Size parent to fit the content
    Size contentSize = _stickersTab->getContentSize();
    contentSize.width += _galleryTab->getContentSize().width;
    contentSize.width += leftMarginX + (tabMarginX * 2);
    parent->setContentSize(contentSize);
}

void MessageComposer::createCancelButton(cocos2d::ui::Layout* parent)
{
    parent->setLayoutType(ui::Layout::Type::HORIZONTAL);
    
    const float buttonHeight = _topLayout->getContentSize().height * 0.45f;
    const float contentMarginX = 30.0f;
    
    _cancelButton = ui::Button::create("res/chat/ui/buttons/dismiss_keyboard_btn.png");
    // Enable content adaption - otherwise % size doesn't work
    _cancelButton->ignoreContentAdaptWithSize(false);
    _cancelButton->getRendererNormal()->setStrechEnabled(true);
    _cancelButton->getRendererClicked()->setStrechEnabled(true);
    _cancelButton->getRendererDisabled()->setStrechEnabled(true);
    _cancelButton->setContentSize(Size(buttonHeight, buttonHeight));
    _cancelButton->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(contentMarginX, 0, 0, 0)));
    _cancelButton->addClickEventListener([this](Ref* button){
        // Clear the message before we set to idle
        _messageEntryField->setString("");
        setMode(MessageComposer::Mode::Idle);
    });
    parent->addChild(_cancelButton);
    
    // Size parent to fit the content
    Size contentSize = _cancelButton->getContentSize();
    contentSize.width += contentMarginX * 2;
    parent->setContentSize(contentSize);
}

void MessageComposer::createMessageEntryUI(cocos2d::ui::Layout* parent)
{
    parent->setLayoutType(ui::Layout::Type::HORIZONTAL);
    
    // { [TEXT ENTRY----->][SEND BUTTON] }
    Texture2D* textEntryTex = _director->getTextureCache()->addImage("res/chat/ui/textfield/message_field.png");
    _messageEntryLayout = SplitLayout::create();
    _messageEntryLayout->setContentSize(textEntryTex->getContentSize());
    _messageEntryLayout->setMode(SplitLayout::Mode::Horizontal);
    _messageEntryLayout->setSplitBehaviour(SplitLayout::FillSize, SplitLayout::FixedSize);
    _messageEntryLayout->setBackGroundImage("res/chat/ui/textfield/message_field.png");
    _messageEntryLayout->setBackGroundImageScale9Enabled(true);
    _messageEntryLayout->setBackGroundImageCapInsets(Rect(79, 79, 1638, 2));
    _messageEntryLayout->setLayoutParameter(CreateCenterVerticalLinearLayoutParam());
    parent->addChild(_messageEntryLayout);
    
    
    // Text entry on the left
    ui::Layout* firstLayout = _messageEntryLayout->firstLayout();
    firstLayout->setLayoutType(ui::Layout::Type::HORIZONTAL);
    
    const float textEntryLeftMargin = 50.0f;
    
    // TODO: Get text from Strings
    _messageEntryField = ui::TextField::create("Type a message here...", Style::Font::RegularSystemName, 65.0f);
    _messageEntryField->setCursorEnabled(true);
    _messageEntryField->ignoreContentAdaptWithSize(false);
    _messageEntryField->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _messageEntryField->setTextVerticalAlignment(TextVAlignment::CENTER);
    _messageEntryField->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(textEntryLeftMargin, 0, 0, 0)));
    _messageEntryField->setPlaceHolderColor(Style::Color::windowsBlue);
    _messageEntryField->setTextColor(Color4B(Style::Color::darkBlueGrey));
    _messageEntryField->addEventListener(CC_CALLBACK_2(MessageComposer::onTextFieldEvent, this));
    firstLayout->addChild(_messageEntryField);
    
    
    // Send button on the right
    ui::Layout* secondLayout = _messageEntryLayout->secondLayout();
    secondLayout->setLayoutType(ui::Layout::Type::HORIZONTAL);
    
    const float buttonHeight = textEntryTex->getContentSize().height * 0.68f;
    const float sendButtonRightMargin = (textEntryTex->getContentSize().height - buttonHeight) / 2;
    
    _sendButton = ui::Button::create("res/chat/ui/buttons/send_btn.png");
    // Enable content adaption - otherwise % size doesn't work
    _sendButton->ignoreContentAdaptWithSize(false);
    _sendButton->getRendererNormal()->setStrechEnabled(true);
    _sendButton->getRendererClicked()->setStrechEnabled(true);
    _sendButton->getRendererDisabled()->setStrechEnabled(true);
    _sendButton->setContentSize(Size(buttonHeight, buttonHeight));
    _sendButton->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(0, 0, sendButtonRightMargin, 0)));
    _sendButton->addClickEventListener([this](Ref* button){
        const std::string& message = _messageEntryField->getString();
        sendMessage(message);
    });
    secondLayout->addChild(_sendButton);
    
    // Size parent to fit the content
    Size secondLayoutContentSize = _sendButton->getContentSize();
    secondLayoutContentSize.width += sendButtonRightMargin;
    secondLayout->setContentSize(secondLayoutContentSize);
}

NS_AZOOMEE_CHAT_END
