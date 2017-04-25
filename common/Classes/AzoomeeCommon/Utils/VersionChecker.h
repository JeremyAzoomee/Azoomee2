#ifndef AzoomeeCommon_VersionChecker_h
#define AzoomeeCommon_VersionChecker_h

#include <string>
#include <vector>


namespace Azoomee
{

    bool isString1EqualsString2(std::string version1, std::string version2);
    bool isString1GreaterThanString2(std::string version1, std::string version2);
    
    std::string getPureVersionNumber(std::string version);
    std::vector<std::string> getVersionNumberElementsInVector(std::string version);

}

#endif
