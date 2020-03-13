//
//  OnlineSafetySlide.h
//  azoomee2
//
//  Created by Macauley on 24/08/2018.
//

#ifndef OnlineSafetySlide_h
#define OnlineSafetySlide_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class OnlineSafetySlide : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    std::string _titleString;
    std::string _bodyString;
    std::string _imageFilename;
    
    cocos2d::Label* _titleText = nullptr;
    cocos2d::Label* _bodyText = nullptr;
    cocos2d::ui::ImageView* _image = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void setTitle(const std::string& text);
    void setBodyText(const std::string& text);
    void setImage(const std::string& imageFilename);
    
    CREATE_FUNC(OnlineSafetySlide);
    
};

NS_AZ_END

#endif /* OnlineSafetySlide_h */
