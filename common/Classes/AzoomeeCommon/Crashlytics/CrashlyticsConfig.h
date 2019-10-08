#ifndef AzoomeeCommon_CrashlyticsConfig_h
#define AzoomeeCommon_CrashlyticsConfig_h

#include <string>
#include <cocos/cocos2d.h>

// Enable Crashlytics for Android, or on iOS only for Test and CI builds
#if !defined(DEBUG) && ( (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || defined(AZOOMEE_ENVIRONMENT_TEST) || defined(AZOOMEE_ENVIRONMENT_CI))
#define CRASHLYTICS_ENABLED
#endif

namespace Azoomee
{
namespace CrashlyticsConsts
{
	const std::string kContentIdKey = "contentId";
	const std::string kAzoomeeEmailKey = "azoomeeEmail";
	const std::string kCurrentSceneKey = "currentScene";
	const std::string kLastHttpRequestTagKey = "lastHttpRequestTag";
	const std::string kSessionIdKey = "sessionId";
}
void createCrashlyticsException(const std::string& exceptionDomain, int exceptionCode, const std::string& exceptionMessage);
void createCrashlyticsUserInfo(const std::string& AdultIdentifier, const std::string& ChildIdentifier);
void setCrashlyticsKeyWithString(const std::string& key, const std::string& dataString);
  
}

#endif
