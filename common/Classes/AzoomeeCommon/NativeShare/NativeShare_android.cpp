#include "NativeShare.h"
#include <cocos/cocos2d.h>
#include <cocos/platform/android/jni/JniHelper.h>

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";

namespace Azoomee
{

void nativeShareScreenString(const std::string& shareString)
{
    cocos2d::JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "shareActionProviderWithString", shareString);
}

}
