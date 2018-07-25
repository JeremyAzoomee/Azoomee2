//
//  ContentItemPoolParser.h
//  AzoomeeCommon
//
//  Created by Macauley on 08/05/2018.
//

#ifndef ContentItemPoolParser_h
#define ContentItemPoolParser_h

#include "../../Azoomee.h"
#include "../Json.h"

NS_AZOOMEE_BEGIN

class ContentItemPoolParser
{
private:
    void parseContentItem(const std::string& contentId, const rapidjson::Value& contentItemData);
public:
    static ContentItemPoolParser* getInstance();
    ~ContentItemPoolParser(void);
    
    void parseContentItemPool(const std::string& contentItemPoolString);
    
};

NS_AZOOMEE_END


#endif /* ContentItemPoolParser_h */
