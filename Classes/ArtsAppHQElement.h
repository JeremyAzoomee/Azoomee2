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
    virtual bool initWithURLAndSize(const std::string& filePath, const cocos2d::Size& size, bool deletable, bool newImage, bool preload = true);
    void enableOnScreenChecker();
    void addImage(cocos2d::Texture2D* tex);
    void addPlaceHolder();
    void loadImageTex();
    
private:
    void createImageBorder();
    void createWhiteBackground();
    void addOverlay();
    void addListenerToElement();
    void addLockToElement();
    bool _elementActive = false;
    
    cocos2d::Sprite *addDeleteButton();
    cocos2d::Sprite *_deleteButton;
    void addListenerToDeleteButton(cocos2d::Sprite *toBeAddedTo);
    void showDeleteButton(float dt);
    void hideDeleteButton();
    bool deleteButtonIsShown();
    void scheduleShowingDeleteButton();
    void unscheduleShowingDeleteButton();
    
    void onExit() override;
    
    cocos2d::LayerColor *_baseLayer;
    cocos2d::LayerColor *_overlayWhenTouched;
    bool _movedAway = false;
    bool _iamtouched = false;
    cocos2d::Point _touchPoint;
    cocos2d::Sprite* _artImage = nullptr;
    std::string _imageURL;
    
    ArtImageOnScreenChecker* _onScreenChecker = nullptr;
};

NS_AZOOMEE_END

#endif
