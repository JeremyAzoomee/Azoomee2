//
//  EpisodeBar.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 23/09/2019.
//

#ifndef EpisodeBar_h
#define EpisodeBar_h

#include <TinizineCommon/Tinizine.h>
#include <TinizineCommon/ImageDownloader/OnScreenChecker.h>
#include <TinizineCommon/UI/DynamicText.h>
#include <TinizineCommon/UI/RoundedRectSprite.h>
#include "ContentTile.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class EpisodeBar : public ContentTile, public OnScreenChecker
{
    typedef ContentTile Super;
private:
    
    static const cocos2d::Vec2 kDropShadowPadding;
    static const float kTextPadding;
    static const cocos2d::Vec2 kTileAspectRatio;
    
    cocos2d::ui::ImageView* _dropShadow = nullptr;
    RoundedRectSprite* _contentImage = nullptr;
    cocos2d::ui::ImageView* _playIcon = nullptr;
    cocos2d::ui::Layout* _textLayout = nullptr;
    DynamicText* _episodeTag = nullptr;
    DynamicText* _episodeTitle = nullptr;
    
    RoundedRectSprite* _lockedOverlay = nullptr;
    
    void elementDisappeared(cocos2d::Node *sender) override;
    void elementAppeared(cocos2d::Node *sender) override;
    
    void resizeImageAndText();
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setContentItemData(const HQContentItemObjectRef& contentItem) override;
    void setBarColour(const cocos2d::Color3B& colour);
    void setEpisodeNumber(int episodeNumber);
    void setEpisodeTagColour(const cocos2d::Color3B& colour);
    void setEpisodeNameColour(const cocos2d::Color3B& colour);
    
    CREATE_FUNC(EpisodeBar);
    
    // delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZ_END

#endif /* EpisodeBar_h */
