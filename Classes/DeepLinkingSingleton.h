#ifndef __DEEP_LINKING_SINGLETON_H__
#define __DEEP_LINKING_SINGLETON_H__

#include "cocos2d.h"

class DeepLinkingSingleton : public cocos2d::Ref
{
private:
    std::string Host;
    std::string Path;
    
    void setHostandPath(std::string UriString);
    void actionDeepLink();
    
    void completeContentAction(std::string type,std::string uri);
    
public:
    static DeepLinkingSingleton* getInstance(void);
    virtual ~DeepLinkingSingleton();
    bool init(void);
    
    void setDeepLink(std::string UriString);
    void resetDeepLink();
    
    void contentDetailsResponse(std::string responseBody);
    
};

#endif
