#include "StickerCategoryListViewItem.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>


using namespace cocos2d;



NS_AZOOMEE_CHAT_BEGIN

bool StickerCategoryListViewItem::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Style::Color::black);
    setLayoutType(ui::Layout::Type::HORIZONTAL);
    // List items are selectable in the ListView
    setTouchEnabled(true);
    
    // Content first
    const float borderWidthPct = 0.02f;
    _contentLayout = ui::Layout::create();
    _contentLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _contentLayout->setSizePercent(Vec2(1.0f - borderWidthPct, 1.0f));
    _contentLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _contentLayout->setLayoutParameter(CreateCenterVerticalLinearLayoutParam());
    addChild(_contentLayout);
    
    // ImageView
    _imageView = ui::ImageView::create();
    _imageView->ignoreContentAdaptWithSize(false); // stretch the image
    _imageView->setAnchorPoint(Vec2(0.5f, 0.5f));
    _imageView->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _contentLayout->addChild(_imageView);
             
    // Border on the right
    _border = ui::Layout::create();
    _border->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _border->setBackGroundColor(Style::Color::grapePurple);
    _border->setSizeType(ui::Widget::SizeType::PERCENT);
    _border->setSizePercent(Vec2(borderWidthPct, 0.7f));
    _border->setLayoutParameter(CreateCenterVerticalLinearLayoutParam());
    addChild(_border);
    
    
    // By default setup content as blank
    setData(nullptr);
  
    return true;
}

void StickerCategoryListViewItem::onEnter()
{
    Super::onEnter();
}

void StickerCategoryListViewItem::onExit()
{
    Super::onExit();
}

void StickerCategoryListViewItem::doLayout()
{
    // Update the size of the imageview to maintain aspect ratio
    const Size& displaySize = getContentSize() * 0.8f;
    const Size& imageSize = _imageView->getVirtualRendererSize();
    if(imageSize.height > imageSize.width)
    {
        float imageAspectRatio = imageSize.width / imageSize.height;
        _imageView->setContentSize(Size(displaySize.height * imageAspectRatio, displaySize.height));
    }
    else
    {
        float imageAspectRatio = imageSize.height / imageSize.width;
        _imageView->setContentSize(Size(displaySize.width, displaySize.width * imageAspectRatio));
    }
    
    Super::doLayout();
}

#pragma mark - Size changes

void StickerCategoryListViewItem::setContentSize(const cocos2d::Size& contentSize)
{
    // Ignore the width and set it based on the height
    Size correctedSize = contentSize;
    correctedSize.width = contentSize.height;
    Super::setContentSize(correctedSize);
}

void StickerCategoryListViewItem::onSizeChanged()
{
    Super::onSizeChanged();
}

#pragma mark - Public

void StickerCategoryListViewItem::setData(const StickerCategoryRef& category)
{
    _stickerCategory = category;
    
    // Update content elements
    if(_stickerCategory)
    {
        _imageView->loadTexture(_stickerCategory->imageLocalPath());
    }
    else
    {
        _imageView->loadTexture("");
    }
    
    // Force elements to update their sizes
    forceDoLayout();
}

StickerCategoryRef StickerCategoryListViewItem::getData() const
{
    return _stickerCategory;
}

void StickerCategoryListViewItem::setSelected(bool selected)
{
    _selected = selected;
    setBackGroundColor(selected ? Style::Color::grapePurpleTwo : Style::Color::black);
    _border->setVisible(!selected);
}


NS_AZOOMEE_CHAT_END
