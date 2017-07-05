#include "StickerSelector.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "../Data/StickerCache.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

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
    firstLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    
    _stickerListView = StickerListView::create();
    _stickerListView->setSizeType(ui::Widget::SizeType::PERCENT);
    _stickerListView->setSizePercent(Vec2(1.0f, 0.98f));
    _stickerListView->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _stickerListView->addItemSelectedEventListener([this](const StickerRef& sticker) {
        if(_selectedEventCallback)
        {
            AnalyticsSingleton::getInstance()->chatSelectedSticker(sticker->imageURL());
            _selectedEventCallback(sticker);
        }
    });
    firstLayout->addChild(_stickerListView);
    
    ui::Layout* secondLayout = _splitLayout->secondLayout();
    secondLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    secondLayout->setBackGroundColor(Style::Color::dark);
    
    _categoryListView = StickerCategoryListView::create();
    _categoryListView->setSizeType(ui::Widget::SizeType::PERCENT);
    _categoryListView->setSizePercent(Vec2(1.0f, 1.0f));
    _categoryListView->addItemSelectedEventListener([this](const StickerCategoryRef& category){
        AudioMixer::getInstance()->playEffect(OK_BUTTON_AUDIO_EFFECT);
        AnalyticsSingleton::getInstance()->chatSelectedStickerTab(category->categoryID());
        selectCategory(category);
    });
    secondLayout->addChild(_categoryListView);
  
    return true;
}

void StickerSelector::onEnter()
{
    Super::onEnter();
    
    // Update the content
    _categories = StickerCache::getInstance()->categories();
    _categoryListView->setItems(_categories);
    
    // By default select the first category
    selectCategory(_categories[0]);
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

void StickerSelector::selectCategory(const StickerCategoryRef& category)
{
    _selectedCategory = category;
    _categoryListView->selectItem(category);
    _stickerListView->setItems(category->stickers());
}

void StickerSelector::addStickerSelectedEventListener(const StickerSelectedCallback& callback)
{
    _selectedEventCallback = callback;
}

NS_AZOOMEE_CHAT_END
