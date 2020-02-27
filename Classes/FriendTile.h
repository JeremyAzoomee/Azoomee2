//
//  FriendTile.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 23/10/2019.
//

#ifndef FriendTile_h
#define FriendTile_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/UI/DynamicText.h>
#include <TinizineCommon/ImageDownloader/ImageDownloader.h>
#include "AzoomeeChat/Data/Friend.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class FriendTile : public cocos2d::ui::Layout, public ImageDownloaderDelegate
{
    typedef cocos2d::ui::Layout Super;
    typedef std::function<void(Chat::FriendRef)> SelectedCallback;
private:
    static const float kHeightScale;
    static const float kFrameThickness;
    
    cocos2d::ui::ImageView* _oomee = nullptr;
    cocos2d::ClippingNode* _contentClipper = nullptr;
    cocos2d::Sprite* _clippingStencil = nullptr;
    cocos2d::ui::ImageView* _pattern = nullptr;
    cocos2d::ui::ImageView* _bgColour = nullptr;
    cocos2d::ui::ImageView* _frame = nullptr;
    cocos2d::LayerRadialGradient* _circleGradient = nullptr;
    
    DynamicText* _friendName = nullptr;
    
    Chat::FriendRef _friendData = nullptr;
    
    SelectedCallback _selectedCallback = nullptr;
    
    ImageDownloaderRef _imgDownloader = nullptr;
    
    void resizeContent();
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setFriendData(const Chat::FriendRef& friendData);
    void setTileWidth(float width);
    
    void setSelectedCallback(const SelectedCallback& callback);
    
    CREATE_FUNC(FriendTile);
    
    //delegate functions
    void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    void onImageDownloadFailed() override;

};

NS_AZ_END

#endif /* FriendTile_h */
