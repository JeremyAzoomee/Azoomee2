#ifndef __OFFLINEHUB_HQ_H__
#define __OFFLINEHUB_HQ_H__

#include "cocos2d.h"

class OfflineHubHQ : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineHubHQ);
    
    virtual bool init();
    
private:

};

#endif
