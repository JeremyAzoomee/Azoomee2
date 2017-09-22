#include "VideoPlaylistManager.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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

void VideoPlaylistManager::setPlaylist(HQCarouselObject playlist)
{
    storedPlaylist = playlist;
}

void VideoPlaylistManager::clearPlaylist()
{
    //HQTODO - clear playlist
}

std::string VideoPlaylistManager::getPlaylist()
{
    std::string returnString;
    
    if(storedPlaylist.getContentItems()->size() == 0) returnString = "noPlaylist";
    else
    {
        std::vector<std::map<std::string, std::string>> playlistElements;
        
        for(int i = 0; i < storedPlaylist.getContentItems()->size(); i++)
        {
            std::map<std::string, std::string> elementToBeAdded;
            elementToBeAdded["uri"] = storedPlaylist.getContentItems()->at(i)->getUri();
            elementToBeAdded["image"] = storedPlaylist.getContentItems()->at(i)->getImagePath();
            elementToBeAdded["title"] = storedPlaylist.getContentItems()->at(i)->getTitle();
            
            playlistElements.push_back(elementToBeAdded);
        }
        
        returnString = Azoomee::getJSONStringFromVectorOfMaps(playlistElements);
        
    }
    
    char *output;
    cocos2d::base64Encode((unsigned char *)returnString.c_str(), (unsigned int)returnString.length(), &output);
    
    return StringUtils::format("%s", output);
}

HQContentItemObject VideoPlaylistManager::getContentItemDataForPlaylistElement(int elementNumber)
{
    HQContentItemObject returnData = HQContentItemObject::create();
    if(elementNumber >= storedPlaylist.getContentItems()->size() || elementNumber < 0) return returnData;
    
    returnData = *storedPlaylist.getContentItems()->at(elementNumber);
    returnData.setImagePath("");
    
    return returnData;
}

NS_AZOOMEE_END
