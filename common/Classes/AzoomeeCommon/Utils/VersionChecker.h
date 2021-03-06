#ifndef AzoomeeCommon_VersionChecker_h
#define AzoomeeCommon_VersionChecker_h

#include <string>
#include <vector>


namespace Azoomee
{
    bool azoomeeMeetsVersionRequirement(std::string version);
    bool azoomeeVersionEqualsVersionNumber(std::string version);
    
    std::string getPureVersionNumber(std::string version);
    std::vector<std::string> getVersionNumberElementsInVector(std::string version);

}

#endif
