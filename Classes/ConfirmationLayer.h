#ifndef __CONFIRMATION_LAYER_H__
#define __CONFIRMATION_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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

NS_AZOOMEE_END

#endif
