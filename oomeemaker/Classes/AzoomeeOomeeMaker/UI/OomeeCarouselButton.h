//
//  OomeeCarouselButton.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 16/03/2018.
//

#ifndef OomeeCarouselButton_h
#define OomeeCarouselButton_h

#include "../AzoomeeOomeeMaker.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/LazyLoadingButton.h>

NS_AZOOMEE_OM_BEGIN

class OomeeCarouselButton : public LazyLoadingButton
{
    typedef LazyLoadingButton Super;
private:
    cocos2d::ui::Button* _deleteButton = nullptr;
    cocos2d::ui::Button* _editButton = nullptr;
    cocos2d::ui::Button* _makeAvatarButton = nullptr;
    cocos2d::ui::Button* _shareButton = nullptr;
    bool _inFocus = false;
    std::string _oomeeFileName;
    
    void animateButtonsIn(float duration);
    void animateButtonsOut(float duration);
    
    
public:
    
    virtual bool init() override;
    
    void setOomeeData(const std::string& filename);
    
    void setInFocus(bool inFocus);
    
    
    CREATE_FUNC(OomeeCarouselButton);
};

NS_AZOOMEE_OM_END


#endif /* OomeeCarouselButton_h */
