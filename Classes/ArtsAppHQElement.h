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
    std::string getBase64Encoded(std::string input);
    std::string getFileNameFromPath(std::string path);
    
    bool notSendingFileData;
    
    cocos2d::LayerColor *baseLayer;
    cocos2d::LayerColor *overlayWhenTouched;
    bool movedAway;
    cocos2d::Point touchPoint;
};

#endif
