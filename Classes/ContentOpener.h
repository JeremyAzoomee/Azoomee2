//
//  ContentOpener.h
//  azoomee2
//
//  Created by Macauley on 18/12/2017.
//

#ifndef ContentOpener_h
#define ContentOpener_h

#include <TinizineCommon/Tinizine.h>
#include "cocos2d.h"
#include <TinizineCommon/Data/HQDataObject/HQContentItemObject.h>
#include <TinizineCommon/API/API.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ContentOpener : public HttpRequestCreatorResponseDelegate
{
private:
	HQContentItemObjectRef _contentItemToOpen = nullptr;
public:
    static const std::string kGroupRefreshEvent;
    
    static ContentOpener* getInstance();
    
    void openContentById(const std::string& contentId);
    void openContentObject(const HQContentItemObjectRef& contentItem);
	
	void doCarouselContentOpenLogic(const HQContentItemObjectRef& contentItem, int rowIndex, int elementIndex, const std::string& hqCategory, const std::string& location);
	
    void getDataForGroupHQ(const std::string &uri, const cocos2d::Color4B& carouselColour);
    
	//delegate functions
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
	void onHttpRequestFailed(const std::string& requestTag, long errorCode);
};

NS_AZ_END

#endif /* ContentOpener_h */
