//
//  ShareInChatScene.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 19/11/2019.
//

#ifndef ShareInChatScene_h
#define ShareInChatScene_h

#include <TinizineCommon/UI/Scene.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "ShareInChatLayer.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ShareInChatScene : public TZ::Scene
{
    typedef TZ::Scene Super;
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

NS_AZ_END

#endif /* ShareInChatScene_h */
