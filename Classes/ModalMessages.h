#ifndef __MODALMESSAGES_SCENE_H__
#define __MODALMESSAGES_SCENE_H__

#include "cocos2d.h"

class ModalMessages: public cocos2d::Layer
{
private:
    void createLayer();
    void addListenerToBackgroundLayer();
    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(ModalMessages);
    
    void startLoading();
    void stopLoading();
    
    void createMessageWithSingleButton(std::string messageTitle, std::string messageBody, std::string buttonText);
    
    void createMessageBox();
    void createMessageTitle(std::string messageTitle);
    void createMessageBody(std::string messageBody);
    void createButton(std::string buttonText);
    
    cocos2d::LayerColor *loadingLayer;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
};

#endif // __MODALMESSAGES_SCENE_H__
