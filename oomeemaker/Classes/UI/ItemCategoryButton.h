//
//  ItemCategoryButton.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 16/02/2018.
//

#ifndef ItemCategoryButton_h
#define ItemCategoryButton_h

#include "../AzoomeOomeeMaker.h"
#include "../DataObjects/ItemCategory.h"
#include <cocos2d/cocos2d.h>
#include <cocos2d/UI/CocosGUI.h>

NS_AZOOMEE_OM_BEGIN

class ItemCategoryButton : public cocos2d::ui::Button
{
    typedef cocos2d::ui::Button Super;
private:
    ItemCategoryRef _categoryData;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void setItemData(const ItemCategoryRef& itemdata);
    
    CREATE_FUNC(ItemCategoryButton);
};

#endif /* ItemCategoryButton_h */
