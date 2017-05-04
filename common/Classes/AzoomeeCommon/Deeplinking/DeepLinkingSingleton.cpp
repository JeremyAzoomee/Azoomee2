#include "DeepLinkingSingleton.h"

namespace Azoomee
{
    
static DeepLinkingSingleton *_sharedDeepLinkingSingleton = NULL;

DeepLinkingSingleton* DeepLinkingSingleton::getInstance()
{
    if (! _sharedDeepLinkingSingleton)
    {
        _sharedDeepLinkingSingleton = new DeepLinkingSingleton();
        _sharedDeepLinkingSingleton->init();
    }
    
    return _sharedDeepLinkingSingleton;
}

DeepLinkingSingleton::~DeepLinkingSingleton(void)
{
}

bool DeepLinkingSingleton::init(void)
{
    return true;
}

void DeepLinkingSingleton::setDeepLink(std::string Host, std::string Path)
{
    CCLOG("HOST:%s | PATH:%s",host.c_str(),path.c_str());
    
    
}

}
