#ifndef Azoomee_Urls_h
#define Azoomee_Urls_h

#include <TinizineCommon/Tinizine.h>
#include <string>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

namespace Url
{
    const char* const PrivacyPolicy = "http://azoomee.com/index.php/privacy-policy-2/";
    const char* const PrivacyPolicyNoLinks = "https://assets.azoomee.com/policies/privacy-policy/index.html";
    const char* const TermsOfUse = "https://assets.azoomee.com/policies/terms-and-conditions/index.html";

    const std::string kIOSSubURL = "https://buy.itunes.apple.com/WebObjects/MZFinance.woa/wa/DirectAction/manageSubscriptions";
    const std::string kAndroidSubURL = "https://play.google.com/store/account/subscriptions";
    const std::string kAmazonSubURL = "amzn://apps/library/subscriptions";
}
  
NS_AZ_END

#endif