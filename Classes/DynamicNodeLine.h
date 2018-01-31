//
//  DynamicNodeLine.h
//  azoomee2-mobile
//
//  Created by Macauley on 13/12/2017.
//

#ifndef DynamicNodeLine_h
#define DynamicNodeLine_h

#include "cocos2d.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_BEGIN

class DynamicNodeLine : public cocos2d::Node
{
private:
    cocos2d::DrawNode* _line = nullptr;
    
    void addLineWithParams(const cocos2d::Vec2& startPos, const cocos2d::Vec2& endPos, int width, const cocos2d::Color4B& colour);
    
public:
    virtual bool init() override;
    
    bool initWithParams(const rapidjson::Value& params, const cocos2d::Size& dynamicNodeSize);
    
    CREATE_FUNC(DynamicNodeLine);
};

NS_AZOOMEE_END


#endif /* DynamicNodeLine_h */
