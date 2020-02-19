//
//  FileUtil_ios.cpp
//  AzoomeeCommon
//
//  Created by Mitch on 16/05/2019.
//

#include "DirUtil.h"
#import <Foundation/Foundation.h>

NS_TZ_BEGIN
    
std::string DirUtil::getDocumentsPath()
{
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* directory = [paths objectAtIndex:0];
    std::string path = [directory UTF8String];
    path.append("/");
    return path;
}

std::string DirUtil::getCachesPath()
{
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString* directory = [paths objectAtIndex:0];
    std::string path = [directory UTF8String];
    path.append("/");
    return path;
}

NS_TZ_END
