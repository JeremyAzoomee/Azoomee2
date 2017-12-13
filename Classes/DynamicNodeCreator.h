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
#include "ButtonActionData.h"
#include "DynamicNodeImage.h"

NS_AZOOMEE_BEGIN

class DynamicNodeCreator : public cocos2d::Ref
{
public:
    static const std::string kCTAAssetLoc;
    static const std::string kCTABundleImageLoc;
    static const std::string kCTADeviceImageCacheLoc;
private:
    
    
    cocos2d::Node* _CTANode = nullptr;
    cocos2d::Size _windowSize;
    
    //CTA components
    cocos2d::ui::Scale9Sprite* _popupFrame = nullptr;
    cocos2d::Node* _popupButtonsLayer = nullptr;
    cocos2d::Node* _popupImages = nullptr;
    cocos2d::Node* _textLayer = nullptr;
    cocos2d::Node* _textInputLayer = nullptr;
    cocos2d::Node* _linesLayer = nullptr;
    cocos2d::ui::Button* _closeButton = nullptr;
    cocos2d::LayerColor* _bgColour = nullptr;
    
    //masking node, stencil and background image
    cocos2d::ClippingNode* _clippingNode = nullptr;
    cocos2d::ui::Scale9Sprite* _stencil = nullptr;
    DynamicNodeImage* _maskedBGImage = nullptr;
    
    // external parameters
    rapidjson::Document _externParams;
    bool _usingExternalParams = false;
    
    void processFile(const rapidjson::Document& configFile);
    void initCTANode();
    void configNodeSize(const rapidjson::Value& sizePercentages);
    void configCloseButton(const rapidjson::Value& closeButtonToggle);
    void configBackgroundColour(const rapidjson::Value& backgroundColour);
    void configBackgroundImage(const rapidjson::Value& backgroundImageData);
    void configButtons(const rapidjson::Value& buttonsList);
    void configExtraImages(const rapidjson::Value& imageList);
    void configText(const rapidjson::Value& textConfig);
    void configTextInput(const rapidjson::Value& textInputConfig);
    void configLines(const rapidjson::Value& linesConfig);
    
public:
    static DynamicNodeCreator* getInstance(void);
    static std::string addExternalParamsToString(const std::string& str);
    virtual ~DynamicNodeCreator();
    bool init(void);
    
    cocos2d::Node* createCTAFromFile(const std::string& filepath);
    cocos2d::Node* createCTAFromFileWithParams(const std::string& filepath, const std::string& params);
    void resetCTAPopup();
};

NS_AZOOMEE_END


#endif /* DynamicNodeCreator_h */
