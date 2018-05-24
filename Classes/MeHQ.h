//
//  MeHQ.h
//  azoomee2
//
//  Created by Macauley on 22/03/2018.
//

#ifndef MeHQ_h
#define MeHQ_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "HQSceneArtsApp.h"
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>

NS_AZOOMEE_BEGIN

class MeHQ : public cocos2d::Layer
{
    typedef cocos2d::Layer Super;
    typedef std::function<void()> RefreshLayoutCallback;
private:
    cocos2d::ui::ListView* _contentListView = nullptr;
    
    std::string _hqCategory = "ME HQ";
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(MeHQ);
    
    void refreshFavouritesLayout();
    void refreshMessagesLayout();
};

NS_AZOOMEE_END


#endif /* MeHQ_h */
