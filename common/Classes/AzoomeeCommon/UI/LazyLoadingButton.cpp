//
//  LazyLoadingButton.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 01/03/2018.
//

#include "LazyLoadingButton.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void LazyLoadingButton::onEnter()
{
    _onScreenChecker = LazyLoadButtonOnScreenChecker();
    _onScreenChecker.startCheckingForOnScreenPosition(this);
    
    Super::onEnter();
}

void LazyLoadingButton::onExit()
{
	_onScreenChecker.stopCheckingOnScreenPosition();
    
    Super::onExit();
}
    
void LazyLoadingButton::setPlaceholderImage(const std::string& placeholderFilename)
{
    _placeholderImageFile = placeholderFilename;
}

void LazyLoadingButton::setMainImage(const std::string& imageFilename)
{
    _mainImageFile = imageFilename;
}
    
void LazyLoadingButton::loadMainImage()
{
    this->retain();
    Director::getInstance()->getTextureCache()->addImageAsync(_mainImageFile, [&](Texture2D* tex){
        this->loadTextureNormal(_mainImageFile);
        this->release();
    });
}

void LazyLoadingButton::loadPlaceholderImage()
{
    loadTextureNormal(_placeholderImageFile);
}

NS_AZOOMEE_END
