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
#include "ItemCategoryList.h"
#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ConfirmCancelMessageBox.h>

NS_AZOOMEE_OM_BEGIN

class OomeeMakerScene : public cocos2d::Scene, ConfirmCancelMessageBoxDelegate
{
    typedef cocos2d::Scene Super;
private:
    static const std::string kColourCategoryName;
    
    static const std::string kSavePopupId;
    static const std::string kResetPopupId;
    
    cocos2d::EventListenerCustom* _oomeeSavedEventListener = nullptr;
    
    cocos2d::Layer* _contentLayer = nullptr;
    
    std::string _filename;
    bool _newOomee = false;
    
    OomeeFigure* _oomee = nullptr;
    OomeeItemList* _itemList = nullptr;
    ItemCategoryList* _categoryList = nullptr;
    
    cocos2d::ui::Button* _undoButton = nullptr;
	cocos2d::ui::Button* _makeAvatarButton = nullptr;
	cocos2d::ui::Button* _exitButton = nullptr;
	cocos2d::ui::Button* _resetOomeeButton = nullptr;
	
    cocos2d::ui::Slider* _itemSlider = nullptr;
    
    void addAccessoryToOomee(const OomeeItemRef& data);
    void setItemsListForCategory(const ItemCategoryRef& data);
    
    void saveAndExit();
    void saveOomeeFiles();
    
    void addBGLayer();
    
    void makeAvatar();
    void undo();
    void resetOomee();
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
	virtual void onExit() override;
    
    void setFilename(const std::string& filename);
    void setIsNewOomee(bool newOomee);
    void displayMadeAvatarNotification();
    
    CREATE_FUNC(OomeeMakerScene);
    
    //delegate functions
    virtual void onConfirmPressed(ConfirmCancelMessageBox* pSender) override;
    virtual void onCancelPressed(ConfirmCancelMessageBox* pSender) override;
};

NS_AZOOMEE_OM_END

#endif /* OomeeMakerScene_h */
