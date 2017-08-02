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
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    cocos2d::LayerColor *backgroundLayer;
    cocos2d::experimental::ui::WebView* modalWebview;
    
    ElectricDreamsButton* closeButton;
    
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
};

NS_AZOOMEE_END

#endif
