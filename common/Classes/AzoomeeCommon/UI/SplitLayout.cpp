#include "SplitLayout.h"
#include "Style.h"


using namespace cocos2d;



NS_AZOOMEE_BEGIN

const float SplitLayout::FixedSize = -1;

bool SplitLayout::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _firstLayout = ObservableLayout::create();
    addChild(_firstLayout);
    
    _secondLayout = ObservableLayout::create();
    addChild(_secondLayout);
    
    // Default to horizontal mode 50/50 split
    setMode(SplitLayout::Mode::Horizontal);
    setSplitBehaviour(0.5f, 0.5f);
  
    return true;
}

void SplitLayout::onEnter()
{
    Super::onEnter();
    
    _firstLayout->addSizeChangedEventListener(std::bind(&SplitLayout::onChildLayoutSizeChanged, this, std::placeholders::_1));
    _secondLayout->addSizeChangedEventListener(std::bind(&SplitLayout::onChildLayoutSizeChanged, this, std::placeholders::_1));
}

void SplitLayout::onExit()
{
    Super::onExit();
    
    _firstLayout->addSizeChangedEventListener(nullptr);
    _secondLayout->addSizeChangedEventListener(nullptr);
}

void SplitLayout::onSizeChanged()
{
    Super::onSizeChanged();
    resizeLayouts();
}

void SplitLayout::setContentSize(const cocos2d::Size& contentSize)
{
    Super::setContentSize(contentSize);
}

#pragma mark - Private

void SplitLayout::onChildLayoutSizeChanged(cocos2d::ui::Layout* layout)
{
    // If we didn't cause this size change, resize the layouts
    if(!_resizingLayouts)
    {
        resizeLayouts();
    }
}

void SplitLayout::resizeLayouts()
{
    _resizingLayouts = true;
    
    // 0,0 is the same as 0.5,0.5
    if(_layoutBehaviour[0] == 0.0f && _layoutBehaviour[1] == 0.0f)
    {
        _layoutBehaviour[0] = 0.5f;
        _layoutBehaviour[1] = 0.5f;
    }
    
    if(_mode == SplitLayout::Mode::Horizontal)
    {
        Vec2 firstSize = Vec2(_layoutBehaviour[0], 1.0f);
        Vec2 secondSize = Vec2(_layoutBehaviour[1], 1.0f);
        
        // -1 means it's a fixed size and we use whatever the layout has set
        if(firstSize.x == -1)
        {
            firstSize.x = _firstLayout->getSizePercent().x;
        }
        if(secondSize.x == -1)
        {
            secondSize.x = _secondLayout->getSizePercent().x;
        }
        // 0 means we need to calculate the remainder
        if(firstSize.x == 0)
        {
            firstSize.x = 1.0f - secondSize.x;
        }
        if(secondSize.x == 0)
        {
            secondSize.x = 1.0f - firstSize.x;
        }
        
        _firstLayout->setSizePercent(firstSize);
        _secondLayout->setSizePercent(secondSize);
    }
    else
    {
        Vec2 firstSize = Vec2(1.0f, _layoutBehaviour[0]);
        Vec2 secondSize = Vec2(1.0f, _layoutBehaviour[1]);
        
        // -1 means it's a fixed size and we use whatever the layout has set
        if(firstSize.y == -1)
        {
            firstSize.y = _firstLayout->getSizePercent().y;
        }
        if(secondSize.y == -1)
        {
            secondSize.y = _secondLayout->getSizePercent().y;
        }
        // If any have 0, we need to calculate the remainder
        if(firstSize.y == 0)
        {
            firstSize.y = 1.0f - secondSize.y;
        }
        if(secondSize.x == 0)
        {
            secondSize.y = 1.0f - firstSize.y;
        }
        
        _firstLayout->setSizePercent(firstSize);
        _secondLayout->setSizePercent(secondSize);
    }
    
    _resizingLayouts = false;
}

#pragma mark - Public

cocos2d::ui::Layout* SplitLayout::firstLayout() const
{
    return _firstLayout;
}

cocos2d::ui::Layout* SplitLayout::secondLayout() const
{
    return _secondLayout;
}

void SplitLayout::setMode(SplitLayout::Mode mode)
{
    _mode = mode;
    setLayoutType((mode == SplitLayout::Mode::Horizontal) ? ui::Layout::Type::HORIZONTAL : ui::Layout::Type::VERTICAL);
    
    resizeLayouts();
}

SplitLayout::Mode SplitLayout::mode() const
{
    return _mode;
}

void SplitLayout::setSplitBehaviour(float firstLayoutSize, float secondLayoutSize)
{
    _layoutBehaviour[0] = firstLayoutSize;
    _layoutBehaviour[1] = secondLayoutSize;
    
    _firstLayout->setSizeType(_layoutBehaviour[0] >= 0 ? ui::Widget::SizeType::PERCENT : ui::Widget::SizeType::ABSOLUTE);
    _secondLayout->setSizeType(_layoutBehaviour[1] >= 0 ? ui::Widget::SizeType::PERCENT : ui::Widget::SizeType::ABSOLUTE);
    
    resizeLayouts();
}

NS_AZOOMEE_END
