#include "MessageComposer.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>


using namespace cocos2d;



NS_AZOOMEE_CHAT_BEGIN

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
    }
    
    createTopUIContent(_topLayout);
  
    return true;
}

void MessageComposer::onEnter()
{
    Super::onEnter();
}

void MessageComposer::onExit()
{
    Super::onExit();
}

void MessageComposer::onSizeChanged()
{
    Super::onSizeChanged();
    
    // Resize absolute sized layouts
    // TODO: Can we build an Adaptor of some kind to automate this?
    std::vector<ui::Widget*> resizeItems = {
        _topLayout,
        _messageEntryLayout
    };
    
    for(auto widget : resizeItems)
    {
        const Size& parentContentSize = widget->getParent()->getContentSize();
        Size itemContentSize = widget->getContentSize();
        if(itemContentSize.width != parentContentSize.width)
        {
            itemContentSize.width = parentContentSize.width;
            widget->setContentSize(itemContentSize);
        }
    }
    
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
        cocos2d::Size correctedSize = contentSize;
        correctedSize.height = _topLayout->getContentSize().height;
        Super::setContentSize(correctedSize);
    }
    else
    {
        Super::setContentSize(contentSize);
    }
}

#pragma mark - UI Creation

void MessageComposer::createTopUIContent(SplitLayout* parent)
{
    // Top layout has the right side fixed, the left fills the space
    // { [LEFT CONTENT----->][TABS] }
    parent->setMode(SplitLayout::Mode::Horizontal);
    parent->setSplitBehaviour(0.0f, SplitLayout::FixedSize);
    
    // Left side has another split, which holds the cancel button, and then the message entry
    // { [CLOSE BUTTON][<------MESSAGE ENTRY UI] }
    ui::Layout* firstLayout = parent->firstLayout();
    SplitLayout* firstLayoutContent = SplitLayout::create();
    // Fill the size of the first column
    firstLayoutContent->setSizeType(ui::Widget::SizeType::PERCENT);
    firstLayoutContent->setSizePercent(Vec2(1.0f, 1.0f));
    firstLayoutContent->setMode(SplitLayout::Mode::Horizontal);
    firstLayoutContent->setSplitBehaviour(SplitLayout::FixedSize, 0.0f);
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
    _stickersTab->getRendererClicked()->setStrechEnabled(true);
    _stickersTab->getRendererDisabled()->setStrechEnabled(true);
    _stickersTab->setContentSize(Size(buttonHeight, buttonHeight));
    _stickersTab->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(leftMarginX, 0, 0, 0)));
    parent->addChild(_stickersTab);
    
    _galleryTab = ui::Button::create("res/chat/ui/buttons/art_btn.png");
    // Enable content adaption - otherwise % size doesn't work
    _galleryTab->ignoreContentAdaptWithSize(false);
    _galleryTab->getRendererNormal()->setStrechEnabled(true);
    _galleryTab->getRendererClicked()->setStrechEnabled(true);
    _galleryTab->getRendererDisabled()->setStrechEnabled(true);
    _galleryTab->setContentSize(Size(buttonHeight, buttonHeight));
    _galleryTab->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(tabMarginX, 0, 0, 0)));
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
    _messageEntryLayout->setSplitBehaviour(0.0f, SplitLayout::FixedSize);
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
    _messageEntryField = ui::TextField::create("Type a message here...", Style::Font::Regular, 65.0f);
    _messageEntryField->setCursorEnabled(true);
    _messageEntryField->ignoreContentAdaptWithSize(false);
    _messageEntryField->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _messageEntryField->setTextVerticalAlignment(TextVAlignment::CENTER);
    _messageEntryField->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(textEntryLeftMargin, 0, 0, 0)));
    _messageEntryField->setPlaceHolderColor(Style::Color::windowsBlue);
    _messageEntryField->setTextColor(Color4B(Style::Color::darkBlueGrey));
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
    secondLayout->addChild(_sendButton);
    
    // Size parent to fit the content
    Size secondLayoutContentSize = _sendButton->getContentSize();
    secondLayoutContentSize.width += sendButtonRightMargin;
    secondLayout->setContentSize(secondLayoutContentSize);
}

NS_AZOOMEE_CHAT_END
