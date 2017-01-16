#ifndef __SLIDESHOW_SCENE_H__
#define __SLIDESHOW_SCENE_H__

#include "cocos2d.h"
#include "ui/UIPageView.h"

USING_NS_CC;
using namespace cocos2d::ui;

class SlideShowScene: public cocos2d::Layer
{
private:
    Size visibleSize;
    Vec2 origin;
    
    Layer* currentSlideLayer;
    
    int currentSlideIndex;
    int maxSlides;
    PageView* _pageView;
    
    void createStaticImages();
    void animageCurrentSlide(Layer* currentSlideLayer);
    void removeCurrentSlide(Layer* currentSlideLayer);
    void animateNextSlide(int newSlideIndex);
    void skipSlideShow();
    void createPageView();
    void pageViewScrollToNextPage();
    
    void pageViewEvent(Ref *pSender, PageView::EventType type);
    
    Layout* createSlide0();
    
    //cocos2d::UserDefault::getInstance()->setStringForKey("value", 1);
    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(SlideShowScene);
};

#endif // __SLIDESHOW_SCENE_H__
