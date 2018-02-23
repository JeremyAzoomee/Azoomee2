//
//  OomeeMakerLayer.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 21/02/2018.
//

#ifndef OomeeMakerScene_h
#define OomeeMakerScene_h

#include "../AzoomeeOomeeMaker.h"
#include "../DataObjects/OomeeItem.h"
#include "../DataObjects/ItemCategory.h"
#include "OomeeFigure.h"
#include "OomeeItemList.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_OM_BEGIN

class OomeeMakerScene : public cocos2d::Scene
{
    typedef cocos2d::Scene Super;
private:
    cocos2d::Layer* _contentLayer = nullptr;
    
    std::string _filename;
    OomeeFigure* _oomee = nullptr;
    OomeeItemList* _itemList = nullptr;
    
    void addAccessoryToOomee(const OomeeItemRef& data);
    void setItemsListForCategory(const ItemCategoryRef& data);
    
    void saveAndExit();
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    
    void setFilename(const std::string& filename);
    
    CREATE_FUNC(OomeeMakerScene);
};

NS_AZOOMEE_OM_END

#endif /* OomeeMakerScene_h */
