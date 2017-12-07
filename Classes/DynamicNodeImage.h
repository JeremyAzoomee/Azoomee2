//
//  DynamicNodeImage.h
//  azoomee2-mobile
//
//  Created by Macauley on 07/12/2017.
//

#ifndef DynamicNodeImage_h
#define DynamicNodeImage_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_BEGIN

class DynamicNodeImage : public cocos2d::Node
{
private:
    cocos2d::Sprite* _image = nullptr;
    
    void addImageWithParams(const cocos2d::Vec2& size, const cocos2d::Size& dynamicNodeSize, const cocos2d::Vec2& pos, int opacity, const std::string& filename);
    void addRemoteImageWithParams(const cocos2d::Vec2& size, const cocos2d::Size& dynamicNodeSize, const cocos2d::Vec2& pos, int opacity, const std::string& url);
    
public:
    virtual bool init() override;
    
     bool initWithParams(const rapidjson::Value& params, const cocos2d::Size& dynamicNodeSize, bool usingExternParams);
    
    CREATE_FUNC(DynamicNodeImage);
};

NS_AZOOMEE_END

#endif /* DynamicNodeImage_h */
