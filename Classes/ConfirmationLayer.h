#ifndef __CONFIRMATION_LAYER_H__
#define __CONFIRMATION_LAYER_H__

#include <cocos/cocos2d.h>

USING_NS_CC;

class ConfirmationLayer : public Layer
{
private:
    
    Label* childNameLabel;
    Label* friendNameLabel;
    Label* friendCodeLabel;
    
    Sprite* connectorSprite;
    
    void clearUIItems();
    void createRect(Color4F withColour);
    
public:
    
    virtual bool init();
    
    CREATE_FUNC(ConfirmationLayer);
    
    void addDetailsToLayer(std::string setChildName, std::string setFriendName, std::string setFriendCode);
    
    void setToConfirm();
    void setToReject();
    void setToRejected();
    void setIdle();
};

#endif
