#ifndef __VIDEOPLAYLISTMANAGER_H__
#define __VIDEOPLAYLISTMANAGER_H__

#include <cocos/cocos2d.h>

class VideoPlaylistManager : public cocos2d::Ref
{
    
public:
    static VideoPlaylistManager* getInstance(void);
    virtual ~VideoPlaylistManager();
    bool init(void);
    
    void setPlaylist(std::vector<std::string> playlist);
    std::string getPlaylist();
    
private:
    std::vector<std::string> storedPlaylist;
};

#endif
