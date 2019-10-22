//
//  ChatHQ.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 22/10/2019.
//

#ifndef ChatHQ_h
#define ChatHQ_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon//UI/DynamicText.h>
#include "HQPage.h"

NS_AZOOMEE_BEGIN

class ChatHQ : public HQPage
{
    typedef HQPage Super;
private:
    cocos2d::LayerGradient* _topScrollGradient = nullptr;
    
    cocos2d::ui::Layout* _recentMessagesLayout = nullptr;
    cocos2d::ui::Layout* _friendsListLayout = nullptr;
    
    void createRecentMessages();
    void createFriendsList();
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(ChatHQ);
};

NS_AZOOMEE_END

#endif /* ChatHQ_h */
