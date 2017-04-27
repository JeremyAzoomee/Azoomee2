#include "VersionChecker.h"
#include "StringFunctions.h"
#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>

using namespace cocos2d;

namespace Azoomee
{
    bool azoomeeMeetsVersionRequirement(std::string reqVersion)
    {
        std::string azoomeeVersion = ConfigStorage::getInstance()->getVersionNumber();
        
        std::vector<std::string> azoomeeVersionVector = getVersionNumberElementsInVector(azoomeeVersion);
        std::vector<std::string> requiredVersionVector = getVersionNumberElementsInVector(reqVersion);
        
        if(azoomeeVersionVector.size() != requiredVersionVector.size()) return true;      //Version numbers not comparable - different amount of elements
        
        bool requirementMet = true;
        
        for(int i = 0; i < azoomeeVersionVector.size(); i++)
        {
            if(std::stoi(azoomeeVersionVector.at(i)) != std::stoi(requiredVersionVector.at(i))) requirementMet = false;
            if(std::stoi(azoomeeVersionVector.at(i)) > std::stoi(requiredVersionVector.at(i))) return true;
        }
        
        return requirementMet;
    }
    
    bool isUpgradeRequired(std::string currentVersion, std::string newVersion)
    {
        std::vector<std::string> currentVersionVector = getVersionNumberElementsInVector(currentVersion);
        std::vector<std::string> newVersionVector = getVersionNumberElementsInVector(newVersion);
        
        if(currentVersionVector.size() != newVersionVector.size()) return false;      //Version numbers not comparable - different amount of elements
        
        for(int i = 0; i < currentVersionVector.size(); i++)
        {
            if(std::stoi(newVersionVector.at(i)) > std::stoi(currentVersionVector.at(i))) return true;
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
