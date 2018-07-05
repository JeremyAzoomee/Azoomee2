//
//  ChildOomeeLayer.h
//  azoomee2
//
//  Created by Macauley on 03/07/2018.
//

#ifndef ChildOomeeLayer_h
#define ChildOomeeLayer_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeOomeeMaker/UI/OomeeCarouselButton.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include <cocos/cocos2d.h>
#include "AddChildLayer.h"

NS_AZOOMEE_BEGIN

class ChildOomeeLayer : public AddChildLayer, ImageDownloaderDelegate
{
    typedef cocos2d::Layer Super;
private:
    OomeeMaker::OomeeCarouselButton* _oomee = nullptr;
    ImageDownloaderRef _oomeeDownloader = nullptr;
public:

    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(ChildOomeeLayer);
    
    //delegate functions
    void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    void onImageDownloadFailed() override;
};

NS_AZOOMEE_END

#endif /* ChildOomeeLayer_h */
