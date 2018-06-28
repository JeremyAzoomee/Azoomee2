//
//  OomeeFigure.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 16/02/2018.
//

#ifndef OomeeFigure_h
#define OomeeFigure_h

#include "../AzoomeeOomeeMaker.h"
#include "../DataObjects/Oomee.h"
#include "../DataObjects/OomeeItem.h"
#include "../DataObjects/OomeeColour.h"
#include "OomeeBody.h"
#include "OomeeAccessory.h"
#include <AzoomeeCommon/UI/CCSpriteWithHue.h>
#include <cocos/cocos2d.h>

NS_AZOOMEE_OM_BEGIN

enum class AccessoryAction {ADD, REMOVE};

struct OomeeDataSnapshot
{
    OomeeRef _oomeeData;
    OomeeColourRef _colourData;
    std::vector<OomeeItemRef> _accessoryData;
};

class OomeeFigure : public cocos2d::Node
{
    typedef cocos2d::Node Super;
private:
    
    bool _isEditable = true;
    OomeeRef _oomeeData = nullptr;
    OomeeColourRef _colour = nullptr;
    OomeeBody* _baseSprite = nullptr;
    std::map<std::string, OomeeAccessory*> _accessories;
    cocos2d::EventListenerTouchOneByOne* _touchListener = nullptr;
    float _hue = 0;
    
    bool _removingItem = false;
    
    std::vector<OomeeDataSnapshot> _undoStack;
    
    OomeeDataSnapshot getDataSnapshot();
    void loadDataSnapshot(const OomeeDataSnapshot& dataSnapshot);
    
    void dependancyCheck();
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    
    bool initWithOomeeFigureData(const rapidjson::Document& data);
    
    void setOomeeData(const OomeeRef& oomeeData);
    OomeeRef getOomeeData() const;
    
    std::vector<std::string> getAccessoryIds() const;
    
    void addAccessory(const OomeeItemRef& oomeeItem);
    void removeAccessory(const std::string anchorPoint);
    
    void setEditable(bool isEditable);
    
    void saveSnapshotImage(const std::string& filepath);
    
    cocos2d::Vec2 getWorldPositionForAnchorPoint(const std::string& anchorPoint);
    cocos2d::Vec2 getLocalPositionForAnchorPoint(const std::string& anchorPoint);
    
    void setColour(const OomeeColourRef& colour);
    OomeeColourRef getColour() const;
    
    float getHue() const;
    void setHue(float hue);
    
    void addDefaultAccessories();
    
    void undoLastAction();
    
    CREATE_FUNC(OomeeFigure);
};

NS_AZOOMEE_OM_END

#endif /* OomeeFigure_h */
