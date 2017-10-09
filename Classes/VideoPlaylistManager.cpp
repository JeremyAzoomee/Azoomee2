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
    _storedPlaylist = HQCarouselObject::create();
    return true;
}

void VideoPlaylistManager::setPlaylist(const HQCarouselObjectRef &playlist)
{
    _storedPlaylist = playlist;
}

void VideoPlaylistManager::clearPlaylist()
{
    _storedPlaylist->removeAllItemsFromCarousel();
}

std::string VideoPlaylistManager::getPlaylist()
{
    std::string returnString;
    
    if(_storedPlaylist->getContentItems().size() == 0) returnString = "noPlaylist";
    else
    {
        std::vector<std::map<std::string, std::string>> playlistElements;
        
        for(auto item : _storedPlaylist->getContentItems())
        {
            if(item->isEntitled())
            {
                std::map<std::string, std::string> elementToBeAdded;
                elementToBeAdded["uri"] = item->getUri();
                elementToBeAdded["image"] = item->getImagePath();
                elementToBeAdded["title"] = item->getTitle();
                
                playlistElements.push_back(elementToBeAdded);
            }
        }
        
        returnString = Azoomee::getJSONStringFromVectorOfMaps(playlistElements);
        
    }
    
    char *output;
    cocos2d::base64Encode((unsigned char *)returnString.c_str(), (unsigned int)returnString.length(), &output);
    
    return StringUtils::format("%s", output);
}

HQContentItemObjectRef VideoPlaylistManager::getContentItemDataForPlaylistElement(int elementNumber)
{
    HQContentItemObjectRef returnData = HQContentItemObject::create();
    if(elementNumber >= _storedPlaylist->getContentItems().size() || elementNumber < 0)
    {
        return returnData;
    }
    
    returnData = _storedPlaylist->getContentItems().at(elementNumber);
    returnData->setElementNumber(elementNumber);
    returnData->setImagePath("");
    
    return returnData;
}

NS_AZOOMEE_END
