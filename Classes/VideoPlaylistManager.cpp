#include "VideoPlaylistManager.h"
#include "HQDataProvider.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>

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
    _storedPlaylist = MutableHQCarouselObject::create();
    return true;
}

void VideoPlaylistManager::setPlaylist(const HQCarouselObjectRef &playlist)
{
    _storedPlaylist = playlist;
}

void VideoPlaylistManager::clearPlaylist()
{
    _storedPlaylist = MutableHQCarouselObject::create();
}

std::string VideoPlaylistManager::getPlaylist()
{
    std::string returnString;
    
    if(_storedPlaylist->getContentItems().size() == 0)
    {
        returnString = kNoPlaylist;
    }
    else
    {
        std::vector<std::map<std::string, std::string>> playlistElements;
        
        for(auto item : _storedPlaylist->getContentItems())
        {
            if(item->isEntitled()&&(item->getType() == "AUDIO" || item->getType() == "VIDEO"))
            {
                std::map<std::string, std::string> elementToBeAdded;
                
                std::string itemUri = item->getUri();
                itemUri = replaceAll(itemUri, "{sessionId}", ChildManager::getInstance()->getParentOrChildCdnSessionId());
                
                elementToBeAdded["uri"] = itemUri;
                elementToBeAdded["image"] = HQDataProvider::getInstance()->getThumbnailUrlForItem(item->getContentItemId());
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

std::string VideoPlaylistManager::getPlaylistForIosNativePlayer()
{
    std::string returnString;
    
    if(_storedPlaylist->getContentItems().size() == 0)
    {
        return kNoPlaylist;
    }
    
    for(auto item : _storedPlaylist->getContentItems())
    {
        if(item->isEntitled()&&(item->getType() == "AUDIO" || item->getType() == "VIDEO"))
        {
            if(returnString.length() > 0)
            {
                returnString += "|";
            }
            
            std::string itemUri = item->getUri();
            itemUri = replaceAll(itemUri, "{sessionId}", ChildManager::getInstance()->getParentOrChildCdnSessionId());
            returnString += itemUri;
        }
    }
    
    
    return returnString;
}

HQContentItemObjectRef VideoPlaylistManager::getContentItemDataForPlaylistElement(int elementNumber)
{
    if(elementNumber >= _storedPlaylist->getContentItems().size() || elementNumber < 0)
    {
        return MutableHQContentItemObject::create();
    }
    
    return _storedPlaylist->getContentItems().at(elementNumber);
}

NS_AZOOMEE_END
