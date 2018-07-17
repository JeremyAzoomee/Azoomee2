//
//  MeHQProfileDetails.h
//  azoomee2
//
//  Created by Macauley on 14/05/2018.
//

#ifndef MeHQProfileDetails_h
#define MeHQProfileDetails_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeOomeeMaker/UI/OomeeCarouselButton.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>

NS_AZOOMEE_BEGIN

class MeHQProfileDetails : public cocos2d::ui::Layout, ImageDownloaderDelegate
{
    typedef cocos2d::ui::Layout Super;
private:
    cocos2d::ui::Layout* _contentLayer = nullptr;
    OomeeMaker::OomeeCarouselButton* _avatar = nullptr;
    cocos2d::ui::Layout* _labelLayout = nullptr;
    cocos2d::ui::Button* _editOomeeButton = nullptr;
    cocos2d::Label* _nameLabel = nullptr;
    cocos2d::ui::Text* _kidCodeLabel = nullptr;
    
    ImageDownloaderRef _profileImageDownloader = nullptr;
protected:
    virtual void onSizeChanged() override;
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(MeHQProfileDetails);
    
    //delegate functions
    void onImageDownloadComplete(const ImageDownloaderRef& downloader) override;
    void onImageDownloadFailed() override;
};

NS_AZOOMEE_END

#endif /* MeHQProfileDetails_h */
