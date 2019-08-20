//
//  Style.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 07/11/2018.
//

#include "Style.h"

NS_AZOOMEE_BEGIN

namespace Style
{

namespace Font
{

std::string GetFontForCurrentLanguage(const std::string& font)
{
    auto it =  kFontOverrideMap.find({font, StringMgr::getInstance()->getLanguageID()});
    if(it != kFontOverrideMap.end())
    {
        return it->second;
    }
    return font;
}
    
std::string Regular()
{
    return GetFontForCurrentLanguage(SofiaRegular);
}
std::string Medium()
{
	return GetFontForCurrentLanguage(SofiaMedium);
}
std::string Bold()
{
	return GetFontForCurrentLanguage(SofiaBold);
}
std::string Input()
{
	return GetFontForCurrentLanguage(SofiaRegular);
}

std::string RewardRegular()
{
	return GetFontForCurrentLanguage(PassionOneRegular);
}
	
std::string PoppinsRegular()
{
	return GetFontForCurrentLanguage(poppinsRegular);
}
std::string PoppinsMedium()
{
    return GetFontForCurrentLanguage(poppinsMedium);
}
std::string PoppinsBold()
{
	return GetFontForCurrentLanguage(poppinsBold);
}
}
}

NS_AZOOMEE_END
