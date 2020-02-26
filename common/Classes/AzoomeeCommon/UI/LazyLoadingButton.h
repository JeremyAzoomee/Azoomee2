//
//  LazyLoadingButton.h
//  AzoomeeCommon
//
//  Created by Macauley on 01/03/2018.
//

#ifndef LazyLoadingButton_h
#define LazyLoadingButton_h

#include "../Tinizine.h"
#include "LazyLoadButtonOnScreenChecker.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_TZ_BEGIN

class LazyLoadingButton : public cocos2d::ui::Button
{
    typedef cocos2d::ui::Button Super;
private:
    std::string _placeholderImageFile;
    std::string _mainImageFile;
    LazyLoadButtonOnScreenChecker _onScreenChecker;
public:
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void setPlaceholderImage(const std::string& placeholderFilename);
    void setMainImage(const std::string& imageFilename);
    
    void loadMainImage();
    void loadPlaceholderImage();
    
    CREATE_FUNC(LazyLoadingButton);
    
};

NS_TZ_END



#endif /* LazyLoadingButton_h */
