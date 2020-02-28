#ifndef TinizineCommon_ModalMessages_h
#define TinizineCommon_ModalMessages_h

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "AzoomeeArtApp.h"

USING_NS_TZ

NS_AZ_ART_BEGIN

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
  
NS_AZ_ART_END

#endif
