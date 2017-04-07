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
    
    ElectricDreamsButton *startExporingButton;
    
    long currentSlideIndex;
    bool slideShowStarted;
    PageView* _pageView;
    
    void createPageView();
    void pageViewScrollToNextPage();
    
    void pageViewEvent(Ref *pSender, PageView::EventType type);
    
    void skipSlideShow();
    
    Layout* layout2;
    Layout* layout3;
    Layout* layout4;
    Layout* layout5;
    Layout* layout6;
    
    Layout* addSlide(int SlideNumber);
    void SheduleSlideSpriteCreation(float dt);
    
    void fadeInObject(Node* objectToFade);
    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(SlideShowScene);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

#endif // __SLIDESHOW_SCENE_H__
