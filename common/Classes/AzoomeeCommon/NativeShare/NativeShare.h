#ifndef AzoomeeCommon_NativeShare_h
#define AzoomeeCommon_NativeShare_h

#include <cocos/cocos2d.h>

namespace Azoomee
{
  
    void nativeShareScreenString(const std::string& shareString);
	void openDeeplink(const std::string& url);
}

#endif
