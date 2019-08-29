//
//  OomeeHQ.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 29/08/2019.
//

#ifndef OomeeHQ_h
#define OomeeHQ_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "HQPage.h"

NS_AZOOMEE_BEGIN

class OomeeHQ : public HQPage
{
    typedef HQPage Super;
private:
    cocos2d::ui::Layout* _oomeeLayout = nullptr;
    cocos2d::ui::Layout* _oomeeMakerButton = nullptr;
    cocos2d::ui::Layout* _shopButton = nullptr;
    cocos2d::ui::Layout* _artStudioLayout = nullptr;
    cocos2d::ui::Layout* _favouritesLayout = nullptr;
    cocos2d::ui::Layout* _offlineDropdown = nullptr;
    
    void createOomeeLayout();
    void createScrollViewContent();
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void update(float deltaT) override;
    
    CREATE_FUNC(OomeeHQ);
};

NS_AZOOMEE_END

#endif /* OomeeHQ_h */
