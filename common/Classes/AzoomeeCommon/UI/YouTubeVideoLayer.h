#ifndef AzoomeeCommon_YouTubeVideoLayer_h
#define AzoomeeCommon_YouTubeVideoLayer_h

#include <cocos/cocos2d.h>
#include "ui/UIWebView.h"
#include "ElectricDreamsButton.h"

namespace Azoomee
{
    
class YouTubeVideoLayer : public cocos2d::Layer, public ElectricDreamsButtonDelegate
{
private:
    
    cocos2d::LayerColor* backgroundLayer;
    cocos2d::experimental::ui::WebView* videoWebview;
    
    ElectricDreamsButton *closeVideoButton;
    
    Layer* windowLayer = nullptr;
    
    void createBackgroundLayer();
    void addListenerToBackgroundLayer();
    void createCloseButton();
    void createVideoWebview();
    
    void setToPortrait();
    void setToLandscape();
    
    void removeSelf(float dt);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button) override;
    
protected:
    
    virtual bool init() override;
    
public:
    //Main function for creating a MessageBox
    static YouTubeVideoLayer* createWith(std::string YouTubeID);
    
    /// Called when the content size of the scene has changed
    void onSizeChanged();
    
    CREATE_FUNC(YouTubeVideoLayer);

};
  
}

#endif
