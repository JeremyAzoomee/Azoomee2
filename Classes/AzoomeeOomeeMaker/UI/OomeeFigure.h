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
#include "../DataObjects/OomeeFigureData.h"
#include "OomeeBody.h"
#include "OomeeAccessory.h"
#include <TinizineCommon/UI/CCSpriteWithHue.h>
#include <cocos/cocos2d.h>

USING_NS_TZ

NS_AZ_OM_BEGIN

enum class AccessoryAction {ADD, REMOVE};

struct OomeeDataSnapshot
{
    OomeeRef _oomeeData;
    std::vector<OomeeItemRef> _accessoryData;
};

class OomeeFigure : public cocos2d::Node
{
    typedef cocos2d::Node Super;
private:
	
	std::string _figureId = "";
    bool _selected = false;
	
    bool _isEditable = true;
    OomeeRef _oomeeData = nullptr;

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
    
    bool initWithOomeeFigureData(const OomeeFigureDataRef& data);
    
    void setOomeeData(const OomeeRef& oomeeData);
    OomeeRef getOomeeData() const;
    
    std::vector<std::string> getAccessoryIds() const;
	
	OomeeFigureDataRef getFigureData() const;
	
    void addAccessory(const OomeeItemRef& oomeeItem);
    void removeAccessory(const std::string anchorPoint);
    
    void resetOomee();
    
    void setEditable(bool isEditable);
    
    void saveSnapshotImage(const std::string& filepath);
    
    cocos2d::Vec2 getWorldPositionForAnchorPoint(const std::string& anchorPoint);
    cocos2d::Vec2 getLocalPositionForAnchorPoint(const std::string& anchorPoint);
    
    OomeeColourRef getColour() const;
    
    float getHue() const;
    void setHue(float hue);
    
    void setFigureId(const std::string& figureId);
    std::string getFigureId() const;
    void setSelected(bool selected);
    bool isSelected() const;
    
    void undoLastAction();
    int getUndoStackSize() const;
    
    CREATE_FUNC(OomeeFigure);
    
    static bool touchOnSpritePixelPerfect(cocos2d::Touch* touch, cocos2d::Sprite* targetSprite);
};

NS_AZ_OM_END

#endif /* OomeeFigure_h */
