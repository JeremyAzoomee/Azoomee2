//
//  DynamicNodeCreator.h
//  azoomee2
//
//  Created by Macauley on 21/09/2017.
//
//

#ifndef DynamicNodeCreator_h
#define DynamicNodeCreator_h

#include "cocos2d.h"
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class DynamicNodeCreator : public cocos2d::Ref
{
private:
    
public:
    static DynamicNodeCreator* getInstance(void);
    virtual ~DynamicNodeCreator();
    bool init(void);
    
    cocos2d::Node* createCTAFromFile(const std::string& filepath);
    
};

NS_AZOOMEE_END


#endif /* DynamicNodeCreator_h */
