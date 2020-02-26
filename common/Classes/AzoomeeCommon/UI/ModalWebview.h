#ifndef __MODAL_WEBVIEW_H__
#define __MODAL_WEBVIEW_H__

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "ui/UIWebView.h"
#include "../Tinizine.h"

NS_TZ_BEGIN

class ModalWebview : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:

    cocos2d::experimental::ui::WebView *_modalWebview = nullptr;
    
    cocos2d::ui::Button *_closeButton = nullptr;
    
    void createCloseButton(const std::string& closeButtonFilename);
    void createWebView(const std::string& url);
    void addLoadingCircles();
    void callbackFromJS(cocos2d::experimental::ui::WebView* webview, const std::string &answer);
    
public:
    static ModalWebview* createWithURL(const std::string& url, const std::string& closeButtonFilename);
    
    virtual bool init() override;
    
    void onSizeChanged() override;
    
    CREATE_FUNC(ModalWebview);

};

NS_TZ_END

#endif
