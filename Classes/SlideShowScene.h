#ifndef __SLIDESHOW_SCENE_H__
#define __SLIDESHOW_SCENE_H__

#include "cocos2d.h"
#include "ui/UIPageView.h"
#include "ElectricDreamsButton.h"

USING_NS_CC;
using namespace cocos2d::ui;

class SlideShowScene: public cocos2d::Layer, public ElectricDreamsButtonDelegate
{
private:
    Size visibleSize;
    Vec2 origin;
    
    ElectricDreamsButton *skipButton;
    
    Layer* currentSlideLayer;
    
    int currentSlideIndex;
    int maxSlides;
    PageView* _pageView;
    
    void createStaticImages();
    void createButtons();
    void createPageView();
    void pageViewScrollToNextPage();
    
    void pageViewEvent(Ref *pSender, PageView::EventType type);
    
    void skipSlideShow();
    
    Layout* createSlide0();
    Layout* createSlide1();
    Layout* createSlide2();
    
    //cocos2d::UserDefault::getInstance()->setStringForKey("value", 1);
    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(SlideShowScene);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

#endif // __SLIDESHOW_SCENE_H__
