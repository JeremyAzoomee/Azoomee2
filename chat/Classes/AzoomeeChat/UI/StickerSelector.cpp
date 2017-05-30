#include "StickerSelector.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "../Data/StickerCache.h"


using namespace cocos2d;



NS_AZOOMEE_CHAT_BEGIN

bool StickerSelector::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _splitLayout = SplitLayout::create();
    _splitLayout->setMode(SplitLayout::Mode::Vertical);
    _splitLayout->setSplitBehaviour(SplitLayout::FillSize, SplitLayout::FixedSize);
    _splitLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _splitLayout->setSizePercent(Vec2(1.0f, 1.0f));
    addChild(_splitLayout);
    
    ui::Layout* firstLayout = _splitLayout->firstLayout();
    firstLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    firstLayout->setBackGroundColor(Style::Color::golden);
    
    ui::Layout* secondLayout = _splitLayout->secondLayout();
    secondLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    secondLayout->setBackGroundColor(Style::Color::dark);
    
    _categoryListView = StickerCategoryListView::create();
    _categoryListView->setSizeType(ui::Widget::SizeType::PERCENT);
    _categoryListView->setSizePercent(Vec2(1.0f, 1.0f));
    secondLayout->addChild(_categoryListView);
  
    return true;
}

void StickerSelector::onEnter()
{
    Super::onEnter();
    
    // Update the content
    _categoryListView->setItems(StickerCache::getInstance()->categories());
}

void StickerSelector::onExit()
{
    Super::onExit();
}

void StickerSelector::onSizeChanged()
{
    Super::onSizeChanged();
}

#pragma mark - Public

void StickerSelector::setTabBarHeight(float height)
{
    ui::Layout* secondLayout = _splitLayout->secondLayout();
    secondLayout->setContentSize(Size(secondLayout->getContentSize().width, height));
}

NS_AZOOMEE_CHAT_END
