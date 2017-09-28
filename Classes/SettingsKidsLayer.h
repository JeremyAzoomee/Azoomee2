#ifndef __SETTINGS_KIDS_LAYER_H__
#define __SETTINGS_KIDS_LAYER_H__

#include <cocos/cocos2d.h>
#include "ui/UIScrollView.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>

NS_AZOOMEE_BEGIN

using namespace cocos2d;

#define IDLE_COVER_LAYER_Z_ORDER 190
#define SELECTED_COVER_LAYER_Z_ORDER 210
#define IDLE_KID_LAYER_Z_ORDER 200
#define SELECTED_KID_LAYER_Z_ORDER 220

class SettingsKidsLayer : public Layer, public MessageBoxDelegate, public HttpRequestCreatorResponseDelegate
{
private:
    
    float layerHeight;
    bool swallowTouches;
    int childNumberToDelete;
    
    ui::ScrollView *scrollView;
    LayerColor* kidsCoverLayer;
    LayerColor* tabsCoverLayer;

    void addUIObjects();
    
    void createBlackCoverLayer(Size innerSize);
    void addTabsCoverLayer();
    void removeTabsCoverLayer();
    void addListenerToCoverLayer(Layer* listenerToLayer);
    
public:
    static Layer* createWithHeight(float setLayerHeight);
    virtual bool init() override;
    
    CREATE_FUNC(SettingsKidsLayer);
    
    void selectChildForTextInput(int ChildNumber);
    void selectChildForSharing(int ChildNumber);
    void deleteChild(int ChildNumber);
    void scrollReset();
    
    // DELEGATES
    void MessageBoxButtonPressed(std::string messageBoxTitle, std::string buttonTitle) override;
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;

};

NS_AZOOMEE_END

#endif
