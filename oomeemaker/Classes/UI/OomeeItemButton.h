//
//  OomeeItemButton.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 16/02/2018.
//

#ifndef OomeeItemButton_h
#define OomeeItemButton_h

#include "../AzoomeOomeeMaker.h"
#include "../DataObjects/OomeeItem.h"
#include <cocos2d/cocos2d.h>
#include <cocos2d/UI/CocosGUI.h>

NS_AZOOMEE_OM_BEGIN

class OomeeItemButton : public cocos2d::ui::Button
{
    typedef cocos2d::ui::Button Super;
private:
    OomeeItemRef _itemData;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void setItemData(const OomeeItemRef& itemdata);
    
    CREATE_FUNC(OomeeItemButton);
};

#endif /* OomeeItemButton_h */
