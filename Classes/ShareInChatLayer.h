//
//  ShareInChatLayer.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 18/11/2019.
//

#ifndef ShareInChatLayer_h
#define ShareInChatLayer_h

#include "AzoomeeChat/ChatAPI.h"
#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include <AzoomeeCommon/UI/Scene.h>
#include "FriendsList.h"

NS_AZOOMEE_BEGIN

class ShareInChatLayer : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
public:
    enum ShareType {ART, CONTENT, OOMEE};
    typedef std::function<void(void)> OnCompleteCallbackCallback;
private:
    
    static const float kSavedImgWidth;
    
    cocos2d::ui::Layout* _popupBody = nullptr;
    cocos2d::LayerGradient* _bgGradient = nullptr;
    RoundedRectSprite* _bgPattern = nullptr;
    RoundedRectSprite* _bgColour = nullptr;
    
    cocos2d::ui::Layout* _bodyLayout = nullptr;
    cocos2d::ui::Button* _closeButton = nullptr;
    cocos2d::ui::Layout* _imageHolder = nullptr;
    RoundedRectSprite* _contentImage = nullptr;
    DynamicText* _shareMessage = nullptr;
    
    cocos2d::ui::ListView* _friendListScroll = nullptr;
    FriendsList* _friendsList = nullptr;
    
    ShareType _shareType = ShareType::ART;
    
    OnCompleteCallbackCallback _callback = nullptr;
    
    std::string _imgFilename = "";
    std::string _contentId = "";
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setShareType(const ShareType& shareType);
    void setContentImage(const std::string& imgFilename);
    void setContentId(const std::string& contentId);
    
    void setOnCompleteCallback(const OnCompleteCallbackCallback& callback);
    
    CREATE_FUNC(ShareInChatLayer);
};

NS_AZOOMEE_END

#endif /* ShareInChatLayer_h */
