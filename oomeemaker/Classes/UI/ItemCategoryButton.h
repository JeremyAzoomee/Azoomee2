//
//  ItemCategoryButton.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 16/02/2018.
//

#ifndef ItemCategoryButton_h
#define ItemCategoryButton_h

#include "../AzoomeeOomeeMaker.h"
#include "../DataObjects/ItemCategory.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_OM_BEGIN

class ItemCategoryButton : public cocos2d::ui::Button
{
    typedef cocos2d::ui::Button Super;
    typedef std::function<void(const ItemCategoryRef&)> ItemSelectedCallback;
private:
    ItemCategoryRef _categoryData;
    ItemSelectedCallback _itemSelectedCallback = nullptr;
public:
    
    void setCategoryData(const ItemCategoryRef& categoryData);
    ItemCategoryRef getCategoryData() const;
    
    void setItemSelectedCallback(const ItemSelectedCallback& callback);
    
    CREATE_FUNC(ItemCategoryButton);
};

NS_AZOOMEE_OM_END

#endif /* ItemCategoryButton_h */
