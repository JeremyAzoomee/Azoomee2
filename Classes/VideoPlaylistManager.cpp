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

void VideoPlaylistManager::setPlaylist(std::vector<std::string> playlist)
{
    storedPlaylist = playlist;
}

std::string VideoPlaylistManager::getPlaylist()
{
    if(storedPlaylist.size() == 0) return "noPlaylist";
    
    std::string returnString;
    
    for(int i = 0; i < storedPlaylist.size(); i++)
    {
        if(i != 0) returnString += "|";
        returnString += storedPlaylist.at(i);
    }
    
    storedPlaylist.clear();
    
    return returnString;
}
