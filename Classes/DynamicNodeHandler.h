//
//  DynamicNodeHandler.h
//  azoomee2
//
//  Created by Macauley on 25/09/2017.
//
//

#ifndef DynamicNodeHandler_h
#define DynamicNodeHandler_h

#include <AzoomeeCommon/Azoomee.h>
#include "cocos2d.h"
#include "DynamicNodeCreator.h"
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_BEGIN

class DynamicNodeHandler : cocos2d::Ref
{
private:
    std::vector<std::string> getFilesInDirectory(const std::string& path);
    std::vector<std::string> getFoldersInDirectory(const std::string& path);
public:
    static DynamicNodeHandler* getInstance(void);
    virtual ~DynamicNodeHandler();
    bool init(void);
    
    void createDynamicNodeById(const std::string& uniqueId);
    void createDynamicNodeByGroupId(const std::string& groupId);
};

NS_AZOOMEE_END

#endif /* DynamicNodeHandler_h */
