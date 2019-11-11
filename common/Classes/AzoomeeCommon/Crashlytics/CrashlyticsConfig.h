#ifndef AzoomeeCommon_CrashlyticsConfig_h
#define AzoomeeCommon_CrashlyticsConfig_h

#include <string>
#include <cocos/cocos2d.h>

#if !defined(DEBUG)
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
