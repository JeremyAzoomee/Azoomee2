#ifndef __SLIDESHOW_SCENE_H__
#define __SLIDESHOW_SCENE_H__

#include <cocos/cocos2d.h>
#include "ui/UIPageView.h"
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace Azoomee;


class SlideShowScene: public cocos2d::Layer, public ElectricDreamsButtonDelegate
{
private:
    Size visibleSize;
    Vec2 origin;
    
    ElectricDreamsButton *startExporingButton;
    ElectricDreamsButton *loginButton;
    ElectricDreamsButton *startTrialButton;
    
    long currentSlideIndex;
    bool slideShowStarted;
    PageView* _pageView;
    
    void createPageView();
    void pageViewScrollToNextPage();
    
    void pageViewEvent(Ref *pSender, PageView::EventType type);
    
    std::vector<Layout*> layoutVector;
    
    void imageAddedToCache(Texture2D* resulting_texture);
    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(SlideShowScene);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

#endif // __SLIDESHOW_SCENE_H__
