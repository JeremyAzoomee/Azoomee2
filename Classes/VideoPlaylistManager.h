#ifndef __VIDEOPLAYLISTMANAGER_H__
#define __VIDEOPLAYLISTMANAGER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>
#include <AzoomeeCommon/Data/HQDataObject/HQCarouselObject.h>

NS_AZOOMEE_BEGIN

class VideoPlaylistManager : public cocos2d::Ref
{
    
public:
    static VideoPlaylistManager* getInstance(void);
    virtual ~VideoPlaylistManager();
    bool init(void);
    
    void setPlaylist(HQCarouselObjectRef hqCarouselObject);
    std::string getPlaylist();
    HQContentItemObjectRef getContentItemDataForPlaylistElement(int elementNumber);
    void clearPlaylist();
    
private:
    HQCarouselObjectRef storedPlaylist;
};

NS_AZOOMEE_END

#endif
