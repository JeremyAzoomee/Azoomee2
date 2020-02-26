#ifndef AzoomeeCommon_StringFunctions_h
#define AzoomeeCommon_StringFunctions_h

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <string>
#include <vector>
#include "../Tinizine.h"

NS_TZ_BEGIN

class StringFunctions
{
public:
    static std::vector<std::string> splitStringToVector(std::string inputString, std::string separator);
    static std::string stringToLower(std::string input);
    static std::string stringToUpper(std::string input);
    static std::string stringReplace(std::string originalString, std::string stringToReplace, std::string withNewString);
    static std::string getJSONStringFromVectorOfMaps(std::vector<std::map<std::string, std::string>> inputMap);
    static std::string getJSONStringFromMap(std::map<std::string, std::string> inputMap);
    static std::string replaceAll(std::string& str, const std::string& from, const std::string& to);
    static std::string getValueFromHttpResponseHeaderForKey(const std::string &key, const std::string &responseHeaderString);
    static std::string& trim(std::string& string);
    static std::string shortenString(const std::string& string, int maxLength);
    static std::string joinStrings(const std::vector<std::string>& stringList, const std::string& divider);
    static std::string convertStringToAssetSafeString(const std::string& string);
    static std::string convertToHQNameString(const std::string& string);
    static bool stringEndsWith(const std::string &strToTest, const std::string &endStr);
        
    static bool stringToBool(const std::string& string);
    static std::string boolToString(bool boolean);

    /// Trim whitespace from the start of string
    static std::string ltrim(const std::string& s);
    /// Trim whitespace from the end of string
    static std::string rtrim(const std::string& s);
    /// Trim whitespace from the start and end of string
    static std::string trim(const std::string& s);

    static int findPositionOfNthString(std::string string, std::string whatToFind, int whichOne);

    static std::string getPureVersionNumber(std::string version);
    static std::vector<std::string> getVersionNumberElementsInVector(std::string version);
    
    static int compareVersionNumbers(const std::string& targetVersion, const std::string& compVersion);
    
    static void reduceLabelTextToFitWidth(cocos2d::Label* label,float maxWidth);
    static void reduceLabelTextToFitWidth(cocos2d::ui::Text* label,float maxWidth);
};

NS_TZ_END

#endif

