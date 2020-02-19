//
//  SettingsMessageBoxDeleteChild.h
//  azoomee2
//
//  Created by Macauley on 07/09/2018.
//

#ifndef SettingsMessageBoxDeleteChild_h
#define SettingsMessageBoxDeleteChild_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Tinizine.h>
#include "SettingsMessageBox.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class SettingsMessageBoxDeleteChild : public SettingsMessageBox
{
    typedef SettingsMessageBox Super;
private:
    SettingsMessageBoxButton* _cancelButton = nullptr;
    SettingsMessageBoxButton* _deleteButton = nullptr;
    cocos2d::Label* _heading = nullptr;
    
public:
    
    virtual void onEnter() override;
    
    CREATE_FUNC(SettingsMessageBoxDeleteChild);
};

NS_AZ_END


#endif /* SettingsMessageBoxDeleteChild_h */
