//
//  FileUtil_android.cpp
//  AzoomeeCommon
//
//  Created by Mitch on 16/05/2019.
//

#include "DirUtil.h"
#include <cocos/cocos2d.h>


NS_AZOOMEE_BEGIN
    
std::string DirUtil::getDocumentsPath()
{
    return cocos2d::FileUtils::getInstance()->getWritablePath();
}

std::string DirUtil::getCachesPath()
{
    // On Android we use the same directory as documents for cache
    // This is partly for backwards compatability, since we have always done this from day 1
    // If we want to change this, we need to migrate user cache files over
    return cocos2d::FileUtils::getInstance()->getWritablePath();
}

NS_AZOOMEE_END
