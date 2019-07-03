//
//  ArtListColumn.cpp
//  AzoomeeChat
//
//  Created by Macauley on 06/11/2017.
//

#include "ArtListColumn.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_CHAT_BEGIN

const std::string ArtListColumn::_kPlaceholderAssetLoc = "res/contentPlaceholders/Create1X1.png";

void ArtListColumn::enableOnScreenChecker()
{
    _onScreenChecker = ArtListItemOnScreenChecker();
    _onScreenChecker.startCheckingForOnScreenPosition(this);
}

bool ArtListColumn::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setLayoutType(ui::Layout::Type::RELATIVE);
    
    // Content layer fills the height minus some margin
    const Vec2& contentMargin = Vec2(0.03f, 0.03f);
    _contentLayout = ui::Layout::create();
    _contentLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _contentLayout->setSizePercent(Vec2(1.0f - contentMargin.x, 1.0f - contentMargin.y));
    _contentLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _contentLayout->setLayoutParameter(CreateCenterRelativeLayoutParam());
    addChild(_contentLayout);
    
    const float rowMargin = 0.02f;
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
    _topRowButton->getRendererNormal()->setStrechEnabled(true);
    _topRowButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    _topRowButton->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _topRowButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_selectedEventCallback)
            {
                _selectedEventCallback(_data[0]);
            }
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
    _bottomRowButton->getRendererNormal()->setStrechEnabled(true);
    _bottomRowButton->setAnchorPoint(Vec2(0.5f, 0.5f));
    _bottomRowButton->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _bottomRowButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_selectedEventCallback && _data[1] != "")
            {
                _selectedEventCallback(_data[1]);
            }
        }
    });
    _bottomRowLayout->addChild(_bottomRowButton);
    
    return true;
}

void ArtListColumn::onEnter()
{
    Super::onEnter();
}

void ArtListColumn::onExit()
{
    //if(_onScreenChecker)
    //{
        _onScreenChecker.endCheck();
        //_onScreenChecker->release();
    //}
    
    Super::onExit();
}

void ArtListColumn::doLayout()
{
    // Size the buttons
    std::vector<ui::Button*> resizeItemsFillParent = {
        _topRowButton,
        _bottomRowButton
    };
    
    for(auto button : resizeItemsFillParent)
    {
        const Size& parentContentSize = button->getParent()->getContentSize();
        button->setContentSize(parentContentSize);
    }
    
    Super::doLayout();
}

#pragma mark - Size changes

void ArtListColumn::setContentSize(const cocos2d::Size& contentSize)
{
    Super::setContentSize(contentSize);
}

void ArtListColumn::onSizeChanged()
{
    Super::onSizeChanged();
}

#pragma mark - Public

void ArtListColumn::setData(const std::vector<std::string>& data, int startIndex)
{
    // Grab first sticker
    _data[0] = data[startIndex];
    // Grab next sticker if available
    int nextIndex = startIndex + 1;
    _data[1] = (nextIndex < data.size()) ? data[nextIndex] : "";
    
    _topRowButton->loadTextureNormal(_kPlaceholderAssetLoc);
    _bottomRowButton->loadTextureNormal(_data[1] != "" ? _kPlaceholderAssetLoc : "");
    _bottomRowLayout->setVisible(_data[1] != "");
    
    // Force elements to update their sizes
    forceDoLayout();
}

void ArtListColumn::addItemSelectedEventListener(const ItemSelectedCallback& callback)
{
    _selectedEventCallback = callback;
}

void ArtListColumn::addImage(cocos2d::Texture2D* tex, ui::Button* button)
{
    button->loadTextureNormal(tex->getPath());

    forceDoLayout();
}

void ArtListColumn::addPlaceHolder()
{
    _topRowButton->loadTextureNormal(_kPlaceholderAssetLoc);
    if(_data[1] != "")
    {
        _bottomRowButton->loadTextureNormal(_kPlaceholderAssetLoc);
    }
    forceDoLayout();
}

void ArtListColumn::loadImageTex()
{
    this->retain();
    Director::getInstance()->getTextureCache()->addImageAsync(_data[0], [&](Texture2D* tex)
    {
        this->addImage(tex,_topRowButton);
        this->release();
    });
    if(_data[1] != "")
    {
        this->retain();
        Director::getInstance()->getTextureCache()->addImageAsync(_data[1], [&](Texture2D* tex)
        {
            this->addImage(tex, _bottomRowButton);
            this->release();
        });
    }
}

NS_AZOOMEE_CHAT_END

