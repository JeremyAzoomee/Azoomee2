#ifndef __HQSCENEELEMENT_SCENE_H__
#define __HQSCENEELEMENT_SCENE_H__

#include <cocos/cocos2d.h>
#include "network/HttpClient.h"
#include <AzoomeeCommon/Azoomee.h>
#include "HQSceneElementVisual.h"
#include "HQContentItemObject.h"

NS_AZOOMEE_BEGIN

class HQSceneElement : public cocos2d::Layer
{

public:
    CREATE_FUNC(HQSceneElement);
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void setCategory(std::string category);
    void setItemData(HQContentItemObject* itemData);
    void setElementRow(int rowNumber);
    void setElementIndex(int index);
    
    void addHQSceneElement();
    
private:
    std::string elementCategory;
    HQContentItemObject* elementItemData;
    int elementRowNumber;
    int elementIndex;
    
    HQSceneElementVisual* elementVisual;
    void addListenerToElement();
    void startUpElementDependingOnType();
    
    cocos2d::Point touchPoint;
    bool movedAway = false;
    bool iamtouched = false;
};

NS_AZOOMEE_END

#endif
