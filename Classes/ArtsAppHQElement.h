#ifndef __ARTSAPPHQELEMENT_H__
#define __ARTSAPPHQELEMENT_H__

#include "cocos2d.h"

class ArtsAppHQElement : public cocos2d::Layer
{
public:
    CREATE_FUNC(ArtsAppHQElement);
    virtual bool initWithURLAndSize(std::string filePath, cocos2d::Size size, bool newImage);
    
private:
    void createImageBorder();
    void addImage(std::string filePath);
    void addOverlay();
    void addListenerToElement(std::string filePath);
    
    cocos2d::Sprite *addDeleteButton();
    cocos2d::Sprite *deleteButton;
    void addListenerToDeleteButton(cocos2d::Sprite *toBeAddedTo, std::string filePath);
    void showDeleteButton(float dt);
    void hideDeleteButton();
    bool deleteButtonIsShown();
    void scheduleShowingDeleteButton();
    void unscheduleShowingDeleteButton();
    
    std::string getBase64Encoded(std::string input);
    std::string getFileNameFromPath(std::string path);
    
    bool notSendingFileData;
    
    cocos2d::LayerColor *baseLayer;
    cocos2d::LayerColor *overlayWhenTouched;
    bool movedAway;
    cocos2d::Point touchPoint;
};

#endif
