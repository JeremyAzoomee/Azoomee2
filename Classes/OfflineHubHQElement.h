#ifndef __OFFLINEHUB_HQELEMENT_H__
#define __OFFLINEHUB_HQELEMENT_H__

#include "cocos2d.h"

class OfflineHubHQElement : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineHubHQElement);
    virtual bool init();
    
private:

};

#endif
