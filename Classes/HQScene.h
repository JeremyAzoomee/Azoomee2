#ifndef __HQSCENE_SCENE_H__
#define __HQSCENE_SCENE_H__

#include "cocos2d.h"
#include "ui/UIScrollView.h"

class HQScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    bool directionDecided;
    cocos2d::Point startLocation;
    void addListenerToScrollView(cocos2d::ui::ScrollView *vScrollView);
    void setName(std::string name);
    void setBackground(std::string name);
    void setCategoryFromName(std::string name);
    void createScrollView();
    
    int category;
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(HQScene);
};

#endif // __HQSCENE_SCENE_H__
