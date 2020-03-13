//
//  Style.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 10/02/2020.
//

#include "Style.h"
#include <TinizineCommon/Utils/LocaleManager.h>

USING_NS_TZ

NS_AZ_BEGIN

namespace Style
{
    namespace Font
    {
        std::string GetFontForCurrentLanguage(const std::string& font)
        {
            auto it =  kFontOverrideMap.find({font, LocaleManager::getInstance()->getLanguageID()});
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

NS_AZ_END
