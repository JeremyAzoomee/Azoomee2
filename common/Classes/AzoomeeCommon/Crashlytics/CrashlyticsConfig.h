#ifndef AzoomeeCommon_CrashlyticsConfig_h
#define AzoomeeCommon_CrashlyticsConfig_h

#include <string>

namespace Azoomee
{

void createCrashlyticsException(const std::string& exceptionDomain, int exceptionCode, const std::string& exceptionMessage);
void createCrashlyticsUserInfo(const std::string& AdultIdentifier, const std::string& ChildIdentifier);
void setCrashlyticsKEYwithSTRING(const std::string& key, const std::string& dataString);
  
}

#endif
