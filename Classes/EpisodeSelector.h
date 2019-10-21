//
//  EpisodeSelector.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 20/09/2019.
//

#ifndef EpisodeSelector_h
#define EpisodeSelector_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObject.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>
#include "EpisodeBar.h"

NS_AZOOMEE_BEGIN

class EpisodeSelector : public cocos2d::ui::Layout, ImageDownloaderDelegate
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void(HQContentItemObjectRef, int)> ContentSelectedCallback;
    typedef std::function<void(void)> CloseButtonCallback;
private:
    
    static const float kListViewPadding;
    static const float kHeaderHeightPercent;
    static const float kEpisodeBarSpacing;

    
    RoundedRectSprite* _background = nullptr;
    
    cocos2d::ui::Layout* _contentLayout = nullptr;
    cocos2d::ui::Layout* _headerLayout = nullptr;
    RoundedRectSprite* _bannerImage = nullptr;
    cocos2d::Sprite* _logoImage = nullptr;
    cocos2d::ui::Button* _closeButton = nullptr;
    cocos2d::ui::Layout* _divider = nullptr;
    cocos2d::ui::ListView* _episodeListView = nullptr;
    cocos2d::Vector<EpisodeBar*> _episodeBars;
    //cocos2d::LayerGradient* _bottomGradient = nullptr;
    RoundedRectSprite* _bottomGradient = nullptr;
    
    HQDataObjectRef _hqData = nullptr;
    
    ContentSelectedCallback _callback = nullptr;
    CloseButtonCallback _closeCallback = nullptr;
    
    ImageDownloaderRef _logoDownloader = nullptr;
    ImageDownloaderRef _bannerDownloader = nullptr;
    
    cocos2d::Color3B _lineAndTextColour;
    
    float _episodeBarHeight;
    
    void setupEpisodeBars();
    
    void resizeBannerImage();
    
public:
    static const float kEpisodeBarHeightPortrait;
    static const float kEpisodeBarHeightLandscape;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setHqData(const HQDataObjectRef& hqData);
    void setContentSelectedCallback(const ContentSelectedCallback& callback);
    void setCloseButtonCallback(const CloseButtonCallback& callback);
    
    void enableCloseButton(bool enable);
    
    void toggleBottomGradient(bool enabled);
    
    void setLineAndTextColour(const cocos2d::Color3B& colour);
    
    void setEpisodeBarHeight(float height);
    
    CREATE_FUNC(EpisodeSelector);
    
    // delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZOOMEE_END

#endif /* EpisodeSelector_h */
