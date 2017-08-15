#ifndef __ARTSAPPHQELEMENT_H__
#define __ARTSAPPHQELEMENT_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include "ArtImageOnScreenChecker.h"

NS_AZOOMEE_BEGIN

class ArtsAppHQElement : public cocos2d::Layer
{
public:
    CREATE_FUNC(ArtsAppHQElement);
    virtual bool initWithURLAndSize(std::string filePath, cocos2d::Size size, bool newImage, bool deletable, bool locked, bool preload = true);
    void enableOnScreenChecker();
    void addImage(cocos2d::Texture2D* tex);
    void addPlaceHolder();
    void loadImageTex();
    
private:
    void createImageBorder();
    void createWhiteBackground();
    void addImage(std::string filePath);
    void addOverlay();
    void addListenerToElement(std::string filePath, bool preview);
    void addLockToElement();
    
    
    
    cocos2d::Sprite *addDeleteButton();
    cocos2d::Sprite *deleteButton;
    void addListenerToDeleteButton(cocos2d::Sprite *toBeAddedTo, std::string filePath);
    void showDeleteButton(float dt);
    void hideDeleteButton();
    bool deleteButtonIsShown();
    void scheduleShowingDeleteButton();
    void unscheduleShowingDeleteButton();
    
    void onExit() override;
    
    std::string getBase64Encoded(std::string input);
    std::string getFileNameFromPath(std::string path);
    
    bool notSendingFileData = false;
    
    cocos2d::LayerColor *baseLayer;
    cocos2d::LayerColor *overlayWhenTouched;
    bool movedAway = false;
    bool iamtouched = false;
    cocos2d::Point touchPoint;
    cocos2d::Sprite* artImage = nullptr;
    std::string imageURL;
    
    ArtImageOnScreenChecker* onScreenChecker = nullptr;
    
    //bool classStartedImageLoading = false;
};

NS_AZOOMEE_END

#endif
