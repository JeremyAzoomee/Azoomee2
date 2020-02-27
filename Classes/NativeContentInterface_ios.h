#ifndef __NATIVECONTENTINTERFACE_SCENE_H__
#define __NATIVECONTENTINTERFACE_SCENE_H__

#include <cocos/cocos2d.h>
#include <TinizineCommon/Tinizine.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class NativeContentInterface_ios : public cocos2d::Layer
{
public:
    CREATE_FUNC(NativeContentInterface_ios);
    cocos2d::Sprite *closeButton;
    static cocos2d::Scene* createSceneWithURL(const std::string &url, const cocos2d::Vec2& closeButtonAnchor, int videoProgressSeconds = 0);
    virtual bool init();
    virtual void onExit();
    void onEnterTransitionDidFinish();
	void removeWebViewFromScreen();
	void reAddWebViewToScreen();
    void addMediaPlayerToScreen(const std::string &url, int videoProgressSeconds);
    
private:
    void addWebViewToScreen(const std::string &url, const cocos2d::Vec2& closeButtonAnchor);
    void loadContentBasedOnUrl(const std::string &url, const cocos2d::Vec2& closeButtonAnchor, int videoProgressSeconds);
    
    void removeWebViewAndBack();
};

NS_AZ_END

#endif // __NATIVECONTENTINTERFACE_SCENE_H__
