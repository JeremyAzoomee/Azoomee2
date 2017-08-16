//
//  ArtImageOnScreenChecker.cpp
//  azoomee2
//
//  Created by Macauley on 27/07/2017.
//
//

#include "ArtImageOnScreenChecker.h"
#include "ArtsAppHQElement.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

int ArtImageOnScreenChecker::imageRemovedCount = 0;

void ArtImageOnScreenChecker::elementAppeared(Node* sender)
{
    ArtsAppHQElement* element = (ArtsAppHQElement*)sender;
    
    element->loadImageTex();
}

void ArtImageOnScreenChecker::elementDisappeared(Node* sender)
{
    ArtsAppHQElement* element = (ArtsAppHQElement*)sender;
    element->addPlaceHolder();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

NS_AZOOMEE_END
