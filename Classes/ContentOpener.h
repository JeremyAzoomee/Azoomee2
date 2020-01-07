//
//  ContentOpener.h
//  azoomee2
//
//  Created by Macauley on 18/12/2017.
//

#ifndef ContentOpener_h
#define ContentOpener_h

#include <AzoomeeCommon/Azoomee.h>
#include "cocos2d.h"
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>
#include <AzoomeeCommon/API/API.h>

NS_AZOOMEE_BEGIN

class ContentOpener : public HttpRequestCreatorResponseDelegate
{
private:
	HQContentItemObjectRef _contentItemToOpen = nullptr;
public:
    static ContentOpener* getInstance();
    
    void openContentById(const std::string& contentId);
    void openContentObject(const HQContentItemObjectRef& contentItem);
	
	void doCarouselContentOpenLogic(const HQContentItemObjectRef& contentItem, int rowIndex, int elementIndex, const std::string& hqCategory, const std::string& location);
	
	//delegate functions
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
	void onHttpRequestFailed(const std::string& requestTag, long errorCode);
};

NS_AZOOMEE_END

#endif /* ContentOpener_h */
