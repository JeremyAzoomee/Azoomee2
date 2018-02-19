#include "NavigationItem.h"
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

NavigationItem* NavigationItem::create()
{
    NavigationItem* pRet = new(std::nothrow) NavigationItem();
    if(pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        delete pRet;
        pRet = nullptr;
    }
    return pRet;
}

NavigationItem::NavigationItem()
{
}

NavigationItem::~NavigationItem()
{
}

bool NavigationItem::init()
{
    if(!Super::init())
    {
        return false;
    }
    
//    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
//    setBackGroundColor(Color3B::RED);
//    setBackGroundColorOpacity(256 * 0.5f);
    setLayoutType(ui::Layout::Type::RELATIVE);
    
    // Border
    _highlightIcon = ui::ImageView::create("res/navigation/chatAlert.png");
    _highlightIcon->setLayoutParameter(CreateCenterRelativeLayoutParam());
    _highlightIcon->setVisible(_isHighlighted);
    addChild(_highlightIcon);
    
    return true;
}

void NavigationItem::onEnter()
{
    Super::onEnter();
}

void NavigationItem::onExit()
{
    Super::onExit();
}

bool NavigationItem::isHighlighted() const
{
    return _isHighlighted;
}

void NavigationItem::setHighlighted(bool highlight)
{
    _isHighlighted = highlight;
    _highlightIcon->setVisible(_isHighlighted);
}

NS_AZOOMEE_END
