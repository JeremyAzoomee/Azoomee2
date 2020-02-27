#ifndef TinizineCommon_CocosSharedPtr_h
#define TinizineCommon_CocosSharedPtr_h

#include <cocos/base/CCRef.h>
#include <memory>
#include "../Tinizine.h"

NS_TZ_BEGIN

/**
 * A cocos2d::Ref which holds a shared_ptr.
 */
template<typename T>
class CocosSharedPtr : public cocos2d::Ref
{
private:
    std::shared_ptr<T> _ptr;
    
public:
    CocosSharedPtr(const std::shared_ptr<T>& ptr) : _ptr(ptr) {}
    
    /// Return the shared_ptr contained in this Ref
    std::shared_ptr<T> get() const
    {
        return _ptr;
    }
};
  
NS_TZ_END

#endif
