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
#include "ui/CocosGUI.h"
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_BEGIN

class DynamicNodeCreator : public cocos2d::Ref
{
private:
    const std::string _kCTAAssetLoc = "res/CTA_Assets/";
    
    cocos2d::Node* _CTANode = nullptr;
    cocos2d::Size _windowSize;
    
    //CTA components
    cocos2d::ui::Scale9Sprite* _popupFrame = nullptr;
    cocos2d::Sprite* _popupContentImage = nullptr;
    cocos2d::Node* _popupButtonsLayer = nullptr;
    cocos2d::Node* _popupImages = nullptr;
    cocos2d::ui::Button* _closeButton = nullptr;
    cocos2d::LayerColor* _bgColour = nullptr;
    
    //masking node, stencil and background image
    cocos2d::ClippingNode* _clippingNode = nullptr;
    cocos2d::ui::Scale9Sprite* _stencil = nullptr;
    cocos2d::Sprite* _maskedBGImage = nullptr;
    
    
    void initCTANode();
    void configNodeSize(const rapidjson::Value& sizePercentages);
    void configCloseButton(const rapidjson::Value& closeButtonToggle);
    void configBackgroundColour(const rapidjson::Value& backgroundColour);
    void configBackgroundImage(const rapidjson::Value& backgroundImageData);
    void configButtons(const rapidjson::Value& buttonsList);
    void configExtraImages(const rapidjson::Value& imageList);
    
    cocos2d::Texture2D* getTextureFromBase64imageData(std::string data, const std::string& imageName);
    
public:
    static DynamicNodeCreator* getInstance(void);
    virtual ~DynamicNodeCreator();
    bool init(void);
    
    cocos2d::Node* createCTAFromFile(const std::string& filepath);
    void resetCTAPopup();
};

NS_AZOOMEE_END


#endif /* DynamicNodeCreator_h */
