#ifndef AzoomeeCommon_CocosSharedPtr_h
#define AzoomeeCommon_CocosSharedPtr_h

#include <cocos/base/CCRef.h>
#include <memory>


namespace Azoomee
{

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
  
}

#endif
