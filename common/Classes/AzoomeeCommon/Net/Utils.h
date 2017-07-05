#ifndef AzoomeeCommon_Net_Utils_h
#define AzoomeeCommon_Net_Utils_h

#include "../Azoomee.h"
#include <string>

NS_AZOOMEE_BEGIN
namespace Net
{

std::string urlEncode(const std::string &url);
std::string urlDecode(const std::string &encoded);

std::string charToHex(unsigned char c);
unsigned char hexToChar(const std::string &str);

}
NS_AZOOMEE_END


#endif
