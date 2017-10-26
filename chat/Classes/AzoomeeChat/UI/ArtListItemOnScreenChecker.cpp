//
//  ArtListItemOnScreenChecker.cpp
//  AzoomeeChat
//
//  Created by Macauley on 26/10/2017.
//
//

#include "ArtListItemOnScreenChecker.h"
#include "ArtListItem.h"

using namespace cocos2d;

NS_AZOOMEE_CHAT_BEGIN

void ArtListItemOnScreenChecker::elementAppeared(cocos2d::Node *sender)
{
    ArtListItem* element = (ArtListItem*)sender;
    element->loadImageTex();
}

void ArtListItemOnScreenChecker::elementDisappeared(cocos2d::Node *sender)
{
    ArtListItem* element = (ArtListItem*)sender;
    element->addPlaceHolder();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

NS_AZOOMEE_CHAT_END
