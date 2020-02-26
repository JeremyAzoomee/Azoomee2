//
//  Style.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 10/02/2020.
//

#ifndef Style_h
#define Style_h

#include <AzoomeeCommon/Tinizine.h>
#include <string>
#include <map>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

namespace Style
{
    namespace Font
    {
        // Path to the font file
        const std::string SofiaRegular = "fonts/Sofia Pro Soft Regular.otf";
        // The system name of the font, usually inside the TTF file
        const std::string SofiaRegularSystemName = "SofiaProSoftRegular";
        
        // Path to the font file
        const std::string SofiaMedium = "fonts/Sofia Pro Soft Medium.otf";
        // The system name of the font, usually inside the TTF file
        const std::string SofiaMediumSystemName = "SofiaProSoftRegular";
        
        // Path to the font file
        const std::string SofiaBold = "fonts/Sofia Pro Soft Bold.otf";
        // The system name of the font, usually inside the TTF file
        const std::string SofiaBoldSystemName = "SofiaProSoftRegular";
        
        // Font for the Kid Code to be More readable
        const std::string kidCodeRegular = "fonts/Inconsolata-Regular.ttf";
        
        // Path to the font file
        const std::string ArialRegular = "fonts/Arial.ttf";
        // The system name of the font, usually inside the TTF file
        const std::string ArialRegularSystemName = "Arial";
        
        // Path to the font file
        const std::string ArialBold = "fonts/Arial Bold.ttf";
        // The system name of the font, usually inside the TTF file
        const std::string ArialBoldSystemName = "ArialBold";
        
        const std::string PassionOneRegular = "fonts/PassionOne-Regular.ttf";
        
        const std::string poppinsRegular = "fonts/Poppins-Regular.ttf";
        const std::string poppinsBold = "fonts/Poppins-SemiBold.ttf";
        const std::string poppinsMedium = "fonts/Poppins-Medium.ttf";
        
        std::string Regular();
        std::string Medium();
        std::string Bold();
        std::string Input();
        std::string RewardRegular();
        
        std::string PoppinsRegular();
        std::string PoppinsMedium();
        std::string PoppinsBold();
        
        const std::string RegularSystemName = SofiaRegularSystemName;
        const std::string MediumSystemName = SofiaMediumSystemName;
        const std::string BoldSystemName = SofiaBoldSystemName;
        const std::string InputSystemName = SofiaRegularSystemName;
        
        std::string GetFontForCurrentLanguage(const std::string& font);
        
        const std::map<std::pair<std::string, std::string>, std::string> kFontOverrideMap = {
            {{SofiaRegular,"gre"},ArialRegular},
            {{SofiaMedium,"gre"},ArialRegular},
            {{SofiaBold,"gre"},ArialBold},
            {{poppinsRegular,"gre"},ArialRegular},
            {{poppinsMedium,"gre"},ArialRegular},
            {{poppinsBold,"gre"},ArialBold},
            {{PassionOneRegular, "gre"}, ArialBold},
            {{PassionOneRegular, "tur"}, ArialBold}
        };
    }
}

NS_AZ_END

#endif /* Style_h */
