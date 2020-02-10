#ifndef AzoomeeCommon_ModalMessages_h
#define AzoomeeCommon_ModalMessages_h

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "AzoomeeArtApp.h"

NS_AZOOMEE_AA_BEGIN

class ModalMessages
{
public:
    static ModalMessages* getInstance(void);
    
private:
    static const std::string kLoadingLayerName;
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    void removeLayer();
    
    cocos2d::ui::Layout* loadingLayer = nullptr;
    
public:
    
    virtual ~ModalMessages();
    
    void startLoading();
    void stopLoading();
    
    void startSaving();
    void stopSaving();
};
  
NS_AZOOMEE_AA_END

#endif
