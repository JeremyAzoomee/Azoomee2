//
//  HQStructureParser.h
//  AzoomeeCommon
//
//  Created by Macauley on 08/05/2018.
//

#ifndef HQStructureParser_h
#define HQStructureParser_h

#include "../../Azoomee.h"
#include "../Json.h"

NS_AZOOMEE_BEGIN

class HQStructureParser
{
private:
    
public:
    static HQStructureParser* getInstance();
    ~HQStructureParser(void);
    
    void parseHQStructureData(const std::string hqStuctureData);
    
};

NS_AZOOMEE_END

#endif /* HQStructureParser_h */
