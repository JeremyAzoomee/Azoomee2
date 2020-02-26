#include "StickerListViewColumn.h"
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/UI/LayoutParams.h>


using namespace cocos2d;



USING_NS_TZ

NS_AZ_CHAT_BEGIN

bool StickerListViewColumn::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setLayoutType(ui::Layout::Type::RELATIVE);
    
    // Content layer fills the height minus some margin
    const Vec2& contentMargin = Vec2(0.1f, 0.1f);
    _contentLayout = ui::Layout::create();
    _contentLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _contentLayout->setSizePercent(Vec2(1.0f - contentMargin.x, 1.0f - contentMargin.y));
    _contentLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _contentLayout->setLayoutParameter(CreateCenterRelativeLayoutParam());
    addChild(_contentLayout);
    
    const float rowMargin = 0.08f;
    const float rowHeight = (1.0f - rowMargin) * 0.5f;
    
    // Top row
    _topRowLayout = ui::Layout::create();
    _topRowLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _topRowLayout->setSizePercent(Vec2(1.0f, rowHeight));
    _topRowLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _topRowLayout->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    _contentLayout->addChild(_topRowLayout);
    
    _topRowButton = ui::Button::create();
    _topRowButton->ignoreContentAdaptWithSize(false);
    _topRowButton->getRendererNormal()->setStretchEnabled(true);
    _topRowButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    _topRowButton->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _topRowButton->addClickEventListener([this](Ref*){
       if(_selectedEventCallback)
       {
           _selectedEventCallback(_stickers[0]);
       }
    });
    _topRowLayout->addChild(_topRowButton);
    
    // Bottom row
    _bottomRowLayout = ui::Layout::create();
    _bottomRowLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _bottomRowLayout->setSizePercent(_topRowLayout->getSizePercent());
    _bottomRowLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _bottomRowLayout->setLayoutParameter(CreateBottomCenterRelativeLayoutParam());
    _contentLayout->addChild(_bottomRowLayout);
    
    _bottomRowButton = ui::Button::create();
    _bottomRowButton->ignoreContentAdaptWithSize(false);
    _bottomRowButton->getRendererNormal()->setStretchEnabled(true);
    _bottomRowButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    _bottomRowButton->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _bottomRowButton->addClickEventListener([this](Ref*){
        if(_selectedEventCallback && _stickers[1] != nullptr)
        {
            _selectedEventCallback(_stickers[1]);
        }
    });
    _bottomRowLayout->addChild(_bottomRowButton);
  
    return true;
}

void StickerListViewColumn::onEnter()
{
    Super::onEnter();
}

void StickerListViewColumn::onExit()
{
    Super::onExit();
}

void StickerListViewColumn::doLayout()
{
    // Size the buttons
    std::vector<ui::Button*> resizeItemsFillParent = {
        _topRowButton,
        _bottomRowButton
    };
    
    for(auto button : resizeItemsFillParent)
    {
        const Size& parentContentSize = button->getParent()->getContentSize() * 0.8f;
        const Size& buttonTextureSize = button->getRendererNormal()->getTexture()->getContentSize();
        Size buttonContentSize = parentContentSize;
        if(buttonTextureSize.width > buttonTextureSize.height)
        {
            buttonContentSize.height = buttonTextureSize.height / buttonTextureSize.width * buttonContentSize.width;
        }
        else
        {
            buttonContentSize.width = buttonTextureSize.width / buttonTextureSize.height * buttonContentSize.height;
        }
        button->setContentSize(buttonContentSize);
    }
    
    Super::doLayout();
}

#pragma mark - Size changes

void StickerListViewColumn::setContentSize(const cocos2d::Size& contentSize)
{
    Super::setContentSize(contentSize);
}

void StickerListViewColumn::onSizeChanged()
{
    Super::onSizeChanged();
}

#pragma mark - Public

void StickerListViewColumn::setData(const StickerList& stickers, int startIndex)
{
    // Grab first sticker
    _stickers[0] = stickers[startIndex];
    // Grab next sticker if available
    int nextIndex = startIndex + 1;
    _stickers[1] = (nextIndex < stickers.size()) ? stickers[nextIndex] : nullptr;
    
    _topRowButton->loadTextureNormal(_stickers[0]->imageLocalPath());
    _bottomRowButton->loadTextureNormal(_stickers[1] ? _stickers[1]->imageLocalPath() : "");
    _bottomRowLayout->setVisible(_stickers[1] != nullptr);
    
    // Force elements to update their sizes
    forceDoLayout();
}

void StickerListViewColumn::addItemSelectedEventListener(const ItemSelectedCallback& callback)
{
    _selectedEventCallback = callback;
}

NS_AZ_CHAT_END
