#include "VersionChecker.h"
#include "StringFunctions.h"
#include <cocos/cocos2d.h>
#include "../Data/ConfigStorage.h"

using namespace cocos2d;

namespace Azoomee
{
    bool azoomeeMeetsVersionRequirement(std::string reqVersion)
    {
        std::string azoomeeVersion = ConfigStorage::getInstance()->getVersionNumber();
        
        std::vector<std::string> azoomeeVersionVector = getVersionNumberElementsInVector(azoomeeVersion);
        std::vector<std::string> requiredVersionVector = getVersionNumberElementsInVector(reqVersion);
        
        if(azoomeeVersionVector.size() != requiredVersionVector.size()) return true;      //Version numbers not comparable - different amount of elements
        
        for(int i = 0; i < azoomeeVersionVector.size(); i++)
        {
            if(std::atoi(azoomeeVersionVector.at(i).c_str()) < std::atoi(requiredVersionVector.at(i).c_str())) return false;
            if(std::atoi(azoomeeVersionVector.at(i).c_str()) > std::atoi(requiredVersionVector.at(i).c_str())) return true;
        }
        
        return true;
    }
    
    bool azoomeeVersionEqualsVersionNumber(std::string version)
    {
        std::string azoomeeVersion = ConfigStorage::getInstance()->getVersionNumber();
        
        std::vector<std::string> azoomeeVersionVector = getVersionNumberElementsInVector(azoomeeVersion);
        std::vector<std::string> requiredVersionVector = getVersionNumberElementsInVector(version);
        
        if(azoomeeVersionVector.size() != requiredVersionVector.size()) return true;      //Version numbers not comparable - different amount of elements
        
        for(int i = 0; i < azoomeeVersionVector.size(); i++)
        {
            if(std::atoi(azoomeeVersionVector.at(i).c_str()) != std::atoi(requiredVersionVector.at(i).c_str()))
            {
                return false;
            }
        }
        
        return true;
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
