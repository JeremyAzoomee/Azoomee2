//
//  FileUtil_ios.cpp
//  AzoomeeCommon
//
//  Created by Mitch on 16/05/2019.
//

#include "FileUtil.h"
#import <Foundation/Foundation.h>

NS_AZOOMEE_BEGIN
    
std::string FileUtil::getDocumentsPath()
{
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* directory = [paths objectAtIndex:0];
    std::string path = [directory UTF8String];
    path.append("/");
    return path;
}

std::string FileUtil::getCachesPath()
{
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString* directory = [paths objectAtIndex:0];
    std::string path = [directory UTF8String];
    path.append("/");
    return path;
}

NS_AZOOMEE_END
