#include "VideoPlaylistManager.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>

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

void VideoPlaylistManager::setPlaylist(std::vector<std::map<std::string, std::string>> playlist)
{
    storedPlaylist = playlist;
}

std::string VideoPlaylistManager::getPlaylist()
{
    std::string returnString;
    
    if(storedPlaylist.size() == 0) returnString = "noPlaylist";
    else
    {
        std::vector<std::map<std::string, std::string>> playlistElements;
        
        for(int i = 0; i < storedPlaylist.size(); i++)
        {
            std::map<std::string, std::string> elementToBeAdded;
            elementToBeAdded["uri"] = storedPlaylist.at(i)["uri"];
            elementToBeAdded["image"] = storedPlaylist.at(i)["image"];
            elementToBeAdded["title"] = storedPlaylist.at(i)["title"];
            
            playlistElements.push_back(elementToBeAdded);
        }
        
        returnString = Azoomee::getJSONStringFromVectorOfMaps(playlistElements);
        
    }
        
    char *output;
    cocos2d::base64Encode((unsigned char *)returnString.c_str(), (unsigned int)returnString.length(), &output);
    
    return StringUtils::format("%s", output);
}

std::map<std::string, std::string> VideoPlaylistManager::getContentItemDataForPlaylistElement(int elementNumber)
{
    std::map<std::string, std::string> returnData;
    if(elementNumber > storedPlaylist.size() || elementNumber < 0) return returnData;
    
    returnData = storedPlaylist.at(elementNumber);
    returnData["image"].clear();
    
    return returnData;
}
