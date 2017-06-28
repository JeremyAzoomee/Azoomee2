#ifndef __VIDEOPLAYLISTMANAGER_H__
#define __VIDEOPLAYLISTMANAGER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class VideoPlaylistManager : public cocos2d::Ref
{
    
public:
    static VideoPlaylistManager* getInstance(void);
    virtual ~VideoPlaylistManager();
    bool init(void);
    
    void setPlaylist(std::vector<std::map<std::string, std::string>> playlistElements);
    std::string getPlaylist();
    std::map<std::string, std::string> getContentItemDataForPlaylistElement(int elementNumber);
    void clearPlaylist();
    
private:
    std::vector<std::map<std::string, std::string>> storedPlaylist;
};

NS_AZOOMEE_END

#endif
