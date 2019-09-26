//
//  EpisodeBar.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 23/09/2019.
//

#ifndef EpisodeBar_h
#define EpisodeBar_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/ImageDownloader/OnScreenChecker.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include "ContentTile.h"

NS_AZOOMEE_BEGIN

class EpisodeBar : public ContentTile, public OnScreenChecker
{
    typedef ContentTile Super;
private:
    
    static const cocos2d::Vec2 kDropShadowPadding;
    static const float kTextPadding;
    
    cocos2d::ui::ImageView* _dropShadow = nullptr;
    cocos2d::ClippingNode* _contentClipper = nullptr;
    cocos2d::ui::Scale9Sprite* _clippingStencil = nullptr;
    cocos2d::ui::ImageView* _contentImage = nullptr;
    cocos2d::ui::ImageView* _playIcon = nullptr;
    cocos2d::ui::Layout* _textLayout = nullptr;
    DynamicText* _episodeTag = nullptr;
    DynamicText* _episodeTitle = nullptr;
    
    void elementDisappeared(cocos2d::Node *sender) override;
    void elementAppeared(cocos2d::Node *sender) override;
    
    void resizeImageAndText();
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setContentItemData(const HQContentItemObjectRef& contentItem) override;
    
    void setEpisodeNumber(int episodeNumber);
    void setEpisodeTagColour(const cocos2d::Color3B& colour);
    
    CREATE_FUNC(EpisodeBar);
    
    // delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZOOMEE_END

#endif /* EpisodeBar_h */
