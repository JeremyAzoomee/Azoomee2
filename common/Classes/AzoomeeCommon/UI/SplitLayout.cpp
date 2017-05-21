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
    if(_layoutBehaviour[0] == 0.0f && _layoutBehaviour[1] == 0.0f)
    {
        _layoutBehaviour[0] = 0.5f;
        _layoutBehaviour[1] = 0.5f;
    }
    
    if(_mode == SplitLayout::Mode::Horizontal)
    {
        Size firstSize = Size(contentSize.width * _layoutBehaviour[0], contentSize.height);
        Size secondSize = Size(contentSize.width * _layoutBehaviour[1], contentSize.height);
        
        // -1 means it's a fixed size and we use whatever the layout has set
        if(_layoutBehaviour[0] == -1)
        {
            firstSize.width = _firstLayout->getContentSize().width;
        }
        if(_layoutBehaviour[1] == -1)
        {
            secondSize.width = _secondLayout->getContentSize().width;
        }
        // 0 means we need to calculate the remainder
        if(_layoutBehaviour[0] == 0)
        {
            firstSize.width = contentSize.width - secondSize.width;
        }
        if(_layoutBehaviour[1] == 0)
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
        if(_layoutBehaviour[0] == -1)
        {
            firstSize.height = _firstLayout->getContentSize().height;
        }
        if(_layoutBehaviour[1] == -1)
        {
            secondSize.height = _secondLayout->getContentSize().height;
        }
        // 0 means we need to calculate the remainder
        if(_layoutBehaviour[0] == 0)
        {
            firstSize.height = contentSize.height - secondSize.height;
        }
        if(_layoutBehaviour[1] == 0)
        {
            secondSize.height = contentSize.height - firstSize.height;
        }
        
        _firstLayout->setContentSize(firstSize);
        _secondLayout->setContentSize(secondSize);
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
    
//    _firstLayout->setSizeType(_layoutBehaviour[0] == FixedSize ? ui::Widget::SizeType::ABSOLUTE : ui::Widget::SizeType::PERCENT);
//    _secondLayout->setSizeType(_layoutBehaviour[1] == FixedSize ? ui::Widget::SizeType::ABSOLUTE : ui::Widget::SizeType::PERCENT);
    
    resizeLayouts();
}

NS_AZOOMEE_END
