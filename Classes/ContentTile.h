//
//  ContentTile.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 02/09/2019.
//

#ifndef ContentTile_h
#define ContentTile_h

#include <AzoomeeCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ContentTile : public cocos2d::ui::Layout , public ImageDownloaderDelegate
{
public:
    enum ImageScaleMode {FIT_WIDTH, FIT_HEIGHT, SHOW_ALL, FILL_ALL};
    
private:
    typedef cocos2d::ui::Layout Super;
protected:
    typedef std::function<void(HQContentItemObjectRef)> ContentSelectedCallback;
    
    HQContentItemObjectRef _contentItem = nullptr;
    ContentSelectedCallback _callback = nullptr;
    ImageDownloaderRef _imageDownloader = nullptr;
    
    std::string _placholderFilename;
    
    ImageScaleMode _scaleMode = ImageScaleMode::FIT_WIDTH;
    
public:
    
    virtual bool init() override;
    void onExit() override;
    
    void setContentSelectedCallback(const ContentSelectedCallback& callback);
    virtual void setContentItemData(const HQContentItemObjectRef& contentItem);
    HQContentItemObjectRef getContentItemData() const;
    
    virtual void setPlaceholderFilename(const std::string& placeholder);
    
    void setImageScaleMode(const ImageScaleMode& scaleMode);
    
    CREATE_FUNC(ContentTile);
    
    // delegate functions
    virtual void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    virtual void onImageDownloadFailed() override;
};

NS_AZ_END

#endif /* ContentTile_h */
