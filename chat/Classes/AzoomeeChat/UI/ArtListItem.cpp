//
//  ArtListItem.cpp
//  AzoomeeChat
//
//  Created by Macauley on 26/10/2017.
//
//

#include "ArtListItem.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>

using namespace cocos2d;

NS_AZOOMEE_CHAT_BEGIN

void ArtListItem::enableOnScreenChecker()
{
    _onScreenChecker = new ArtListItemOnScreenChecker();
    _onScreenChecker->startCheckingForOnScreenPosition(this);
}

void ArtListItem::doLayout()
{
    // Update the size of the imageview to maintain aspect ratio
    const Size& displaySize = getContentSize() * 0.8f;
    const Size& imageSize = _itemView->getVirtualRendererSize();
    if(imageSize.height > imageSize.width)
    {
        float imageAspectRatio = imageSize.width / imageSize.height;
        _itemView->setContentSize(Size(displaySize.height * imageAspectRatio, displaySize.height));
    }
    else
    {
        float imageAspectRatio = imageSize.height / imageSize.width;
        _itemView->setContentSize(Size(displaySize.width, displaySize.width * imageAspectRatio));
    }
    
    Super::doLayout();
}

void ArtListItem::onSizeChanged()
{
    Super::onSizeChanged();
}

void ArtListItem::setData(const std::string& data)
{
    _itemData = data;
    
    _itemView->loadTexture("res/contentPlaceholders/Create1X1.png");
    
    
    forceDoLayout();
}

/// Register for item selection events
void ArtListItem::addItemSelectedEventListener(const ItemSelectedCallback& callback)
{
    _selectedEventCallback = callback;
}

bool ArtListItem::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Style::Color::dark);
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
    _itemView = ui::ImageView::create();
    _itemView->ignoreContentAdaptWithSize(false); // stretch the image
    _itemView->setAnchorPoint(Vec2(0.5f, 0.5f));
    _itemView->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _contentLayout->addChild(_itemView);

    
    return true;
}

void ArtListItem::onEnter()
{
    Super::onEnter();
}

void ArtListItem::onExit()
{
    if(_onScreenChecker)
    {
        _onScreenChecker->endCheck();
        _onScreenChecker->release();
    }

    Super::onExit();
}

void ArtListItem::setContentSize(const cocos2d::Size& contentSize)
{
    // Ignore the width and set it based on the height
    Size correctedSize = contentSize;
    correctedSize.width = contentSize.height;
    Super::setContentSize(correctedSize);
}

void ArtListItem::addImage(cocos2d::Texture2D* tex)
{
    _itemView->loadTexture(tex->getPath());
    forceDoLayout();
}

void ArtListItem::addPlaceHolder()
{
    _itemView->loadTexture("res/contentPlaceholders/Create1X1.png");
    forceDoLayout();
}

void ArtListItem::loadImageTex()
{
    Director::getInstance()->getTextureCache()->addImageAsync(_itemData, [&](Texture2D* tex)
    {
        this->addImage(tex);
    });

}

NS_AZOOMEE_CHAT_END
