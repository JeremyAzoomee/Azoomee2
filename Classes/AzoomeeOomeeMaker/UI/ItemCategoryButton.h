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
#include <AzoomeeCommon/UI/LazyLoadingButton.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

USING_NS_TZ

NS_AZ_OM_BEGIN

class ItemCategoryButton : public LazyLoadingButton
{
    typedef LazyLoadingButton Super;
    typedef std::function<void(const ItemCategoryRef&)> ItemSelectedCallback;
private:
    ItemCategoryRef _categoryData;
    ItemSelectedCallback _itemSelectedCallback = nullptr;
public:
    
    void setCategoryData(const ItemCategoryRef& categoryData);
    ItemCategoryRef getCategoryData() const;
    
    void setSelected(bool selected);
    
    void setItemSelectedCallback(const ItemSelectedCallback& callback);
    
    CREATE_FUNC(ItemCategoryButton);
};

NS_AZ_OM_END

#endif /* ItemCategoryButton_h */
