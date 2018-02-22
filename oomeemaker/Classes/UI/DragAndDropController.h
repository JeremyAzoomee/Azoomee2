//
//  DragAndDropController.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 20/02/2018.
//

#ifndef DragAndDropController_h
#define DragAndDropController_h

#include "../AzoomeeOomeeMaker.h"
#include "../DataObjects/OomeeItem.h"
#include "OomeeFigure.h"

NS_AZOOMEE_OM_BEGIN

class DragAndDropController
{
private:
    cocos2d::Sprite* _itemSprite = nullptr;
    OomeeItemRef _itemData = nullptr;
    cocos2d::EventListenerTouchOneByOne* _touchListener = nullptr;
    OomeeFigure* _oomeeFigure = nullptr;
    cocos2d::Node* _listenerTargetNode = nullptr;
    
public:
    static DragAndDropController* getInstance();
    virtual ~DragAndDropController();
    void init();
    
    void setItemData(const OomeeItemRef& data);
    void setTargetOomee(OomeeFigure* oomeeFigure);
    void attachToScene(cocos2d::Scene* currentScene);
};

NS_AZOOMEE_OM_END

#endif /* DragAndDropController_h */
