#ifndef __HQSCENEELEMENT_SCENE_H__
#define __HQSCENEELEMENT_SCENE_H__

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "network/HttpClient.h"
#include <AzoomeeCommon/Azoomee.h>
#include "HQSceneElementVisual.h"
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>

NS_AZOOMEE_BEGIN

class HQSceneElement : public cocos2d::ui::Button
{
    typedef std::function<void(const HQContentItemObjectRef&)> HQElementButtonCallback;
	typedef cocos2d::ui::Button Super;
public:
    CREATE_FUNC(HQSceneElement);
    virtual bool init();
    
    void setCategory(const std::string &category);
    void setItemData(const HQContentItemObjectRef &itemData);
    void setElementRow(int rowNumber);
    void setElementIndex(int index);
    void setManualSizeMultiplier(float multiplier);
    void setMargin(float margin);
    void deleteButtonVisible(bool visible);
    void setDeleteButtonCallback(const HQElementButtonCallback& callback);
	void setTouchCallback(const HQElementButtonCallback& callback);
	
	int getElementRow() const;
	int getElementIndex() const;
	
	void setTouchDisabled(bool disabled);
	void enableHighlight(bool enable);
	
    void addHQSceneElement();
    
private:
    std::string _elementCategory;
    HQContentItemObjectRef _elementItemData;
    int _elementRowNumber;
    int _elementIndex;
    float _manualSizeMultiplier = 0.0f;
    float _margin = 0.0f;
    bool _showDeleteButton = false;
	
	bool _touchDisabled = false;
	bool _showHighlight = false;
    
    HQSceneElementVisual* _elementVisual = nullptr;
    cocos2d::ui::Button* _deleteButton = nullptr;
    HQElementButtonCallback _deleteButtonCallback = nullptr;
	HQElementButtonCallback _touchCallback = nullptr;
    
    cocos2d::ui::Button* createDeleteButton();
    
    void startUpElementDependingOnType();
    
    cocos2d::Point _touchPos;
};

NS_AZOOMEE_END

#endif
