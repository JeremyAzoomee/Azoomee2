#ifndef AzoomeeCommon_Net_Utils_h
#define AzoomeeCommon_Net_Utils_h

#include "../Tinizine.h"
#include <string>

NS_TZ_BEGIN
namespace Net
{

std::string urlEncode(const std::string &url);
std::string urlDecode(const std::string &encoded);
std::string getUrlParamsInAlphabeticalOrder(const std::string &originalParams);

std::string charToHex(unsigned char c);
unsigned char hexToChar(const std::string &str);

}
NS_TZ_END


#endif
