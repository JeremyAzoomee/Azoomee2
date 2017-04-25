#include "VersionChecker.h"
#include "StringFunctions.h"


namespace Azoomee
{
    bool isString1EqualsString2(std::string version1, std::string version2)
    {
        std::string toCheck1 = splitStringToVector(version1, " ").at(0);
        std::string toCheck2 = splitStringToVector(version2, " ").at(0);
        
        if(toCheck1 == toCheck2) return true;
        
        return false;
    }

    bool isString1GreaterThanString2(std::string version1, std::string version2)
    {
        if(isString1EqualsString2(getPureVersionNumber(version1), getPureVersionNumber(version2))) return true;
        
        std::vector<std::string> versionNumbersVector1 = getVersionNumberElementsInVector(version1);
        std::vector<std::string> versionNumbersVector2 = getVersionNumberElementsInVector(version2);
        
        if(versionNumbersVector1.size() != versionNumbersVector2.size()) return false;      //Version numbers not comparable - different amount of elements
        
        for(int i = 0; i < versionNumbersVector1.size(); i++)
        {
            if(std::stoi(versionNumbersVector1.at(i)) > std::stoi(versionNumbersVector2.at(i))) return true;
        }
        
        return false;
    }
    
    std::string getPureVersionNumber(std::string version)
    {
        return splitStringToVector(version, " ").at(0);
    }
    
    std::vector<std::string> getVersionNumberElementsInVector(std::string version)
    {
        return splitStringToVector(getPureVersionNumber(version), ".");
    }

} // Azoomee
