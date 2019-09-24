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
#include "EpisodeBar.h"

NS_AZOOMEE_BEGIN

class EpisodeSelector : public cocos2d::ui::Layout, ImageDownloaderDelegate
{
    typedef cocos2d::ui::Layout Super;
private:
    
    cocos2d::ClippingNode* _contentClipper = nullptr;
    cocos2d::ui::Scale9Sprite* _stencil = nullptr;
    
    cocos2d::ui::Layout* _contentLayout = nullptr;
    cocos2d::ui::Layout* _headerLayout = nullptr;
    cocos2d::Sprite* _bannerImage = nullptr;
    cocos2d::ui::Button* _closeButton = nullptr;
    cocos2d::ui::Layout* _divider = nullptr;
    cocos2d::ui::ListView* _episodeListView = nullptr;
    cocos2d::Vector<EpisodeBar*> _episodeBars;
    cocos2d::LayerGradient* _bottonGradient = nullptr;
    
    HQDataObjectRef _hqData = nullptr;
    
    void setupEpisodeBars();
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setHqData(const HQDataObjectRef& hqData);
    
    CREATE_FUNC(EpisodeSelector);
    
    // delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZOOMEE_END

#endif /* EpisodeSelector_h */
