#ifndef __VIDEOPLAYLISTMANAGER_H__
#define __VIDEOPLAYLISTMANAGER_H__

#include <cocos/cocos2d.h>
#include "../Tinizine.h"
#include "../Data/HQDataObject/HQContentItemObject.h"
#include "../Data/HQDataObject/HQCarouselObject.h"

NS_TZ_BEGIN

class VideoPlaylistManager
{
    
public:
    static VideoPlaylistManager* getInstance();
    virtual ~VideoPlaylistManager();
    bool init();
    
    void setPlaylist(const HQCarouselObjectRef &hqCarouselObject);
    std::string getPlaylist();
    std::string getPlaylistForIosNativePlayer();
    HQContentItemObjectRef getContentItemDataForPlaylistElement(int elementNumber);
    void clearPlaylist();
    
private:
    HQCarouselObjectRef _storedPlaylist;
    
    const std::string kNoPlaylist = "noPlaylist";
};

NS_TZ_END

#endif
