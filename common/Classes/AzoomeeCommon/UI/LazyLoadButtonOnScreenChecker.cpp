//
//  LazyLoadButtonOnScreenChecker.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 01/03/2018.
//

#include "LazyLoadButtonOnScreenChecker.h"
#include "LazyLoadingButton.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

LazyLoadButtonOnScreenCheckerRef LazyLoadButtonOnScreenChecker::create()
{
    return std::make_shared<LazyLoadButtonOnScreenChecker>();
}

void LazyLoadButtonOnScreenChecker::elementAppeared(Node* sender)
{
    LazyLoadingButton* button = dynamic_cast<LazyLoadingButton*>(sender);
    if(button)
    {
        button->loadMainImage();
    }
}

void LazyLoadButtonOnScreenChecker::elementDisappeared(Node* sender)
{
    LazyLoadingButton* button = dynamic_cast<LazyLoadingButton*>(sender);
    if(button)
    {
        button->loadPlaceholderImage();
    }
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

NS_AZOOMEE_END
