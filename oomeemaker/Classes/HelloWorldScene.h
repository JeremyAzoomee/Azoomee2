#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "DataObjects/OomeeItem.h"
#include "DataObjects/ItemCategory.h"
#include "UI/OomeeFigure.h"
#include "UI/OomeeItemList.h"

using namespace Azoomee::OomeeMaker;

class HelloWorld : public cocos2d::Layer
{
private:
    OomeeFigure* _oomee = nullptr;
    OomeeItemList* _itemList = nullptr;
    
    void addAccessoryToOomee(const OomeeItemRef& data);
    void setItemsListForCategory(const ItemCategoryRef& data);
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    virtual void onEnterTransitionDidFinish() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
