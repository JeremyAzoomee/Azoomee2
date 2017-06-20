#include "NativeShare.h"
#include <cocos/cocos2d.h>


namespace Azoomee
{

void shareKidCode(const std::string& shareString)
{    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "shareActionProviderWithString", "(Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jstringJSONProperties= methodInfo.env->NewStringUTF(shareString.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jstringJSONProperties);
    
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

}
