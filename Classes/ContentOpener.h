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

NS_AZOOMEE_BEGIN

class ContentOpener: public cocos2d::Ref
{
private:
    
public:
    static ContentOpener* getInstance();
    virtual ~ContentOpener();
    bool init(void);
    
    void openContent(const std::string& contentId);
};

NS_AZOOMEE_END

#endif /* ContentOpener_h */
