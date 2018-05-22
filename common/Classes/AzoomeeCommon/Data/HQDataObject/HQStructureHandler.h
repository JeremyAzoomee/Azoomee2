//
//  HQStructureHandler.h
//  AzoomeeCommon
//
//  Created by Macauley on 08/05/2018.
//

#ifndef HQStructureHandler_h
#define HQStructureHandler_h

#include "../../Azoomee.h"
#include "../../API/HttpRequestCreator.h"

NS_AZOOMEE_BEGIN

class HQStructureHandler : public HttpRequestCreatorResponseDelegate
{
private:
    void loadHQStructureDataByName(const std::string& userFeedName);
    
public:
    static HQStructureHandler* getInstance();
    ~HQStructureHandler();
    
    void getLatestHQStructureFeed();
    
    void loadHQStructureForUser();
    
    //delegate functions
    virtual void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
    virtual void onHttpRequestFailed(const std::string& requestTag, long errorCode);
};

NS_AZOOMEE_END

#endif /* HQStructureHandler_h */
