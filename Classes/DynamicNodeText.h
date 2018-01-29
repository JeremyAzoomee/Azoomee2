//
//  DynamicNodeText.h
//  azoomee2-mobile
//
//  Created by Macauley on 06/12/2017.
//

#ifndef DynamicNodeText_h
#define DynamicNodeText_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_BEGIN

class DynamicNodeText : public cocos2d::Node
{
private:
    cocos2d::Label* _text = nullptr;
    
public:
    virtual bool init() override;

    bool initWithParams(int fontSize, cocos2d::Color4B fontColour, const rapidjson::Value& params, const cocos2d::Size& dynamicNodeSize, bool usingExternParams);
    
    static const int kDefaultTitleFontSize;
    static const int kDefaultBodyFontSize;
    static const int kDefaultFooterFontSize;
    
    CREATE_FUNC(DynamicNodeText);
};

NS_AZOOMEE_END


#endif /* DynamicNodeText_h */
