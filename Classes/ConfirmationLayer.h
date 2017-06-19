#ifndef __CONFIRMATION_LAYER_H__
#define __CONFIRMATION_LAYER_H__

#include <cocos/cocos2d.h>

USING_NS_CC;

class ConfirmationLayer : public Layer
{
private:
    std::string childName;
    std::string friendName;
    std::string friendCode;
    
public:
    
    static ConfirmationLayer* createWithDetails(std::string setChildName, std::string setFriendName, std::string setFriendCode);
    virtual bool init();
    
    CREATE_FUNC(ConfirmationLayer);
    
};

#endif
