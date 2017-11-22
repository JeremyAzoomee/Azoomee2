#ifndef __HQSCENEELEMENT_SCENE_H__
#define __HQSCENEELEMENT_SCENE_H__

#include <cocos/cocos2d.h>
#include "network/HttpClient.h"
#include <AzoomeeCommon/Azoomee.h>
#include "HQSceneElementVisual.h"
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>

NS_AZOOMEE_BEGIN

class HQSceneElement : public cocos2d::Layer
{

public:
    CREATE_FUNC(HQSceneElement);
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void setCategory(const std::string &category);
    void setItemData(const HQContentItemObjectRef &itemData);
    void setElementRow(int rowNumber);
    void setElementIndex(int index);
    void setManualSizeMultiplier(float multiplier);
    void setMargin(float margin);
    
    void addHQSceneElement();
    
private:
    std::string _elementCategory;
    HQContentItemObjectRef _elementItemData;
    int _elementRowNumber;
    int _elementIndex;
    float _manualSizeMultiplier = 0.0f;
    float _margin = 0.0f;
    
    HQSceneElementVisual* _elementVisual;
    
    void addListenerToElement();
    void startUpElementDependingOnType();
    
    cocos2d::Point _touchPoint;
    bool _movedAway = false;
    bool _iamtouched = false;
};

NS_AZOOMEE_END

#endif
