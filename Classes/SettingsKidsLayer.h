#ifndef __SETTINGS_KIDS_LAYER_H__
#define __SETTINGS_KIDS_LAYER_H__

#include <cocos/cocos2d.h>
#include "ui/UIScrollView.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>

NS_AZOOMEE_BEGIN

using namespace cocos2d;

class SettingsKidsLayer : public Layer, public MessageBoxDelegate, public HttpRequestCreatorResponseDelegate
{
private:
    
    float layerHeight;
    int childNumberToDelete;
    
    ui::ScrollView *_scrollView = nullptr;
    LayerColor *_tabsCoverLayer = nullptr;

    void addScrollView();
    void addTabsCoverLayer();
    void addListenerToCoverLayer(Layer* listenerToLayer);
    
    cocos2d::Vec2 _origin;
    cocos2d::Size _visibleSize;
    
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
