#ifndef __MODAL_WEBVIEW_H__
#define __MODAL_WEBVIEW_H__

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "ui/UIWebView.h"
#include "../Tinizine.h"

NS_TZ_BEGIN

class ModalWebview : public cocos2d::Layer
{
private:
    cocos2d::Size _visibleSize;
    cocos2d::Size _runningSceneSize;
    
    cocos2d::LayerColor *_backgroundLayer = nullptr;
    cocos2d::experimental::ui::WebView *_modalWebview = nullptr;
    
    cocos2d::ui::Button *_closeButton = nullptr;
    
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
    
    /// Called when the content size of the scene has changed
    void onSizeChanged();

};

NS_TZ_END

#endif
