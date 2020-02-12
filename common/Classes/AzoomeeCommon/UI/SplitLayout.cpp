#include "SplitLayout.h"
#include "Colour.h"


using namespace cocos2d;



NS_AZOOMEE_BEGIN

const float SplitLayout::FixedSize = -1.0f;
const float SplitLayout::FillSize = 0.0f;

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
    
    // Default to Landscape Orientation 50/50 split
    setOrientation(Orientation::Landscape);
    setSplitBehaviour(0.5f, 0.5f);
  
    return true;
}

void SplitLayout::onEnter()
{
    Super::onEnter();
    
    _firstLayout->addSizeChangedEventListener(CC_CALLBACK_1(SplitLayout::onChildLayoutSizeChanged, this));
    _secondLayout->addSizeChangedEventListener(CC_CALLBACK_1(SplitLayout::onChildLayoutSizeChanged, this));
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
    // Resize layouts if one of the fixed size layouts changed
    if(!_resizingLayouts &&
       ((layout == _firstLayout && _layoutBehaviour[0] == SplitLayout::FixedSize) ||
        (layout == _secondLayout && _layoutBehaviour[1] == SplitLayout::FixedSize)))
    {
        resizeLayouts();
    }
}

void SplitLayout::resizeLayouts()
{
    const Size& contentSize = getContentSize();
    _resizingLayouts = true;
    
    // 0,0 is the same as 0.5,0.5
    if(_layoutBehaviour[0] == SplitLayout::FillSize && _layoutBehaviour[1] == SplitLayout::FillSize)
    {
        _layoutBehaviour[0] = 0.5f;
        _layoutBehaviour[1] = 0.5f;
    }
    
    if(_orientation == Orientation::Landscape)
    {
        Size firstSize = Size(contentSize.width * _layoutBehaviour[0], contentSize.height);
        Size secondSize = Size(contentSize.width * _layoutBehaviour[1], contentSize.height);
        
        // -1 means it's a fixed size and we use whatever the layout has set
        if(_layoutBehaviour[0] == SplitLayout::FixedSize)
        {
            firstSize.width = _firstLayout->getContentSize().width;
        }
        if(_layoutBehaviour[1] == SplitLayout::FixedSize)
        {
            secondSize.width = _secondLayout->getContentSize().width;
        }
        // 0 means we need to calculate the remainder
        if(_layoutBehaviour[0] == SplitLayout::FillSize)
        {
            firstSize.width = contentSize.width - secondSize.width;
        }
        if(_layoutBehaviour[1] == SplitLayout::FillSize)
        {
            secondSize.width = contentSize.width - firstSize.width;
        }
        
        _firstLayout->setContentSize(firstSize);
        _secondLayout->setContentSize(secondSize);
    }
    else
    {
        Size firstSize = Size(contentSize.width, contentSize.height * _layoutBehaviour[0]);
        Size secondSize = Size(contentSize.width, contentSize.height * _layoutBehaviour[1]);
        
        // -1 means it's a fixed size and we use whatever the layout has set
        if(_layoutBehaviour[0] == SplitLayout::FixedSize)
        {
            firstSize.height = _firstLayout->getContentSize().height;
        }
        if(_layoutBehaviour[1] == SplitLayout::FixedSize)
        {
            secondSize.height = _secondLayout->getContentSize().height;
        }
        // 0 means we need to calculate the remainder
        if(_layoutBehaviour[0] == SplitLayout::FillSize)
        {
            firstSize.height = contentSize.height - secondSize.height;
        }
        if(_layoutBehaviour[1] == SplitLayout::FillSize)
        {
            secondSize.height = contentSize.height - firstSize.height;
        }
        
        _firstLayout->setContentSize(firstSize);
        _secondLayout->setContentSize(secondSize);
    }
    
    // We must force a layout since we changed the size of the inner layouts
    forceDoLayout();
    
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

void SplitLayout::setOrientation(Orientation orientation)
{
    _orientation = orientation;
    setLayoutType((orientation == Orientation::Landscape) ? ui::Layout::Type::HORIZONTAL : ui::Layout::Type::VERTICAL);
    
    resizeLayouts();
}

Orientation SplitLayout::orientation() const
{
    return _orientation;
}

void SplitLayout::setSplitBehaviour(float firstLayoutSize, float secondLayoutSize)
{
    _layoutBehaviour[0] = firstLayoutSize;
    _layoutBehaviour[1] = secondLayoutSize;
    
    _firstLayout->setSizeType(_layoutBehaviour[0] == FixedSize ? ui::Widget::SizeType::ABSOLUTE : ui::Widget::SizeType::PERCENT);
    _secondLayout->setSizeType(_layoutBehaviour[1] == FixedSize ? ui::Widget::SizeType::ABSOLUTE : ui::Widget::SizeType::PERCENT);
    
    resizeLayouts();
}

NS_AZOOMEE_END
