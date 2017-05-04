#ifndef __DEEP_LINKING_SINGLETON_H__
#define __DEEP_LINKING_SINGLETON_H__

#include "cocos2d.h"

namespace Azoomee
{


class DeepLinkingSingleton : public cocos2d::Ref
{
private:

    
public:
    static DeepLinkingSingleton* getInstance(void);
    virtual ~DeepLinkingSingleton();
    bool init(void);
    
    void setDeepLink(std::string Host, std::string Path);
    
};

}

#endif
