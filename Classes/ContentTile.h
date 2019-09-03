//
//  ContentTile.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 02/09/2019.
//

#ifndef ContentTile_h
#define ContentTile_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>

NS_AZOOMEE_BEGIN

class ContentTile : public cocos2d::ui::Layout , ImageDownloaderDelegate
{
    typedef cocos2d::ui::Layout Super;
protected:
    typedef std::function<void(HQContentItemObjectRef)> ContentSelectedCallback;
    
    HQContentItemObjectRef _contentItem = nullptr;
    ContentSelectedCallback _callback = nullptr;
    ImageDownloaderRef _imageDownloader = nullptr;
    
public:
    
    void setContentSelectedCallback(const ContentSelectedCallback& callback);
    void setContentItemData(const HQContentItemObjectRef& contentItem);
    HQContentItemObjectRef getContentItemData() const;
    
    CREATE_FUNC(ContentTile);
    
    // delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZOOMEE_END

#endif /* ContentTile_h */
