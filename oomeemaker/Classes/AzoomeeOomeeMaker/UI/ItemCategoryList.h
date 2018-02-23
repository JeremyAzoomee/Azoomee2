//
//  ItemCategoryList.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 16/02/2018.
//

#ifndef ItemCategoryList_h
#define ItemCategoryList_h

#include "../AzoomeeOomeeMaker.h"
#include "../DataObjects/ItemCategory.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_OM_BEGIN

class ItemCategoryList : public cocos2d::ui::ListView
{
    typedef cocos2d::ui::ListView Super;
    typedef std::function<void(const ItemCategoryRef&)> ItemSelectedCallback;
private:
    std::vector<ItemCategoryRef> _categoryList;
    ItemSelectedCallback _itemSelectedCallback = nullptr;
protected:
    virtual void doLayout() override;
    
public:
    
    void setCategories(const std::vector<ItemCategoryRef>& categoryList);
    void setItemSelectedCallback(const ItemSelectedCallback& callback);
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
    
    CREATE_FUNC(ItemCategoryList);
};

NS_AZOOMEE_OM_END


#endif /* ItemCategoryList_h */
