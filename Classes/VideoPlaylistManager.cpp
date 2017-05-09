#include "VideoPlaylistManager.h"

using namespace cocos2d;
//using namespace Azoomee;


static VideoPlaylistManager *_sharedVideoPlaylistManager = NULL;

VideoPlaylistManager* VideoPlaylistManager::getInstance()
{
    if (! _sharedVideoPlaylistManager)
    {
        _sharedVideoPlaylistManager = new VideoPlaylistManager();
        _sharedVideoPlaylistManager->init();
    }
    
    return _sharedVideoPlaylistManager;
}

VideoPlaylistManager::~VideoPlaylistManager(void)
{
}

bool VideoPlaylistManager::init(void)
{
    return true;
}

void VideoPlaylistManager::setPlaylist(std::string playlist)
{
    storedPlaylist = playlist;
}

std::string VideoPlaylistManager::getPlaylist()
{
    if(storedPlaylist.length() == 0) return "noPlaylist";
    
    char *output;
    cocos2d::base64Encode((unsigned char *)storedPlaylist.c_str(), (unsigned int)storedPlaylist.length(), &output);
    
    return StringUtils::format("%s", output);
}
