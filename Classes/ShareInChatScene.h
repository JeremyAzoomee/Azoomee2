//
//  ShareInChatScene.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 19/11/2019.
//

#ifndef ShareInChatScene_h
#define ShareInChatScene_h

#include <AzoomeeCommon/UI/Scene.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "ShareInChatLayer.h"

NS_AZOOMEE_BEGIN

class ShareInChatScene : public Azoomee::Scene
{
    typedef Azoomee::Scene Super;
private:
    
    ShareInChatLayer* _shareLayer = nullptr;
    ShareInChatLayer::ShareType _shareType = ShareInChatLayer::ShareType::ART;
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setShareType(const ShareInChatLayer::ShareType& type);
    
    CREATE_FUNC(ShareInChatScene);
};

NS_AZOOMEE_END

#endif /* ShareInChatScene_h */
