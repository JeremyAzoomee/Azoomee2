#ifndef __MODAL_WEBVIEW_H__
#define __MODAL_WEBVIEW_H__

#include <cocos/cocos2d.h>
#include "ElectricDreamsButton.h"
#include "ui/UIWebView.h"
#include "../Azoomee.h"


NS_AZOOMEE_BEGIN

class ModalWebview : public cocos2d::Layer, public ElectricDreamsButtonDelegate
{
private:
    cocos2d::Size _visibleSize;
    cocos2d::Size _runningSceneSize;
    
    cocos2d::LayerColor *_backgroundLayer;
    cocos2d::experimental::ui::WebView* _modalWebview;
    
    ElectricDreamsButton* _closeButton;
    
    void createBackgroundLayer();
    void addListenerToBackgroundLayer();
    void createCloseButton();
    void createWebView(std::string url);
    void addLoadingCircles();
    void callbackFromJS(cocos2d::experimental::ui::WebView* webview, const std::string &answer);

    void removeSelf();
    
public:
    static cocos2d::Layer* createWithURL(std::string url);
    
    virtual bool init();
    
    CREATE_FUNC(ModalWebview);

    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    
    /// Called when the content size of the scene has changed
    void onSizeChanged();

};

NS_AZOOMEE_END

#endif
