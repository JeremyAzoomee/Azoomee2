//
//  OomeeDisplay.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 07/10/2019.
//

#ifndef OomeeDisplay_h
#define OomeeDisplay_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>


NS_AZOOMEE_BEGIN

class OomeeDisplay : public cocos2d::ui::Layout, ImageDownloaderDelegate
{
    typedef cocos2d::ui::Layout Super;
private:
    
    RoundedRectSprite* _background = nullptr;
    RoundedRectSprite* _bgPattern = nullptr;
    cocos2d::Sprite* _stencil = nullptr;
    cocos2d::ClippingNode* _patternClipper = nullptr;
    cocos2d::LayerRadialGradient* _circleGradient = nullptr;
    cocos2d::Sprite* _oomee = nullptr;
    cocos2d::Sprite* _oomeeShadow = nullptr;
    
    RoundedRectSprite* _kidCodeFrame = nullptr;
    RoundedRectSprite* _kidCodeBody = nullptr;
    DynamicText* _kidCodeTitle = nullptr;
    DynamicText* _kidCode = nullptr;
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setKidCode(const std::string& kidCode);
    void setOomeeImgUrl(const std::string& oomeeImageUrl);
    void setBgPatternColour(const cocos2d::Color3B& colour);
    
    CREATE_FUNC(OomeeDisplay);
    
    //deleagte functions
    void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    void onImageDownloadFailed() override;
};

NS_AZOOMEE_END

#endif /* OomeeDisplay_h */
