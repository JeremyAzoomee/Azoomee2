#include "ObservableLayout.h"


using namespace cocos2d;



NS_TZ_BEGIN

void ObservableLayout::onSizeChanged()
{
    Super::onSizeChanged();
    
    if(_sizeChangedCallback)
    {
        _sizeChangedCallback(this);
    }
}

#pragma mark - Public

void ObservableLayout::addSizeChangedEventListener(const SizeChangedCallback& callback)
{
    _sizeChangedCallback = callback;
}

NS_TZ_END
