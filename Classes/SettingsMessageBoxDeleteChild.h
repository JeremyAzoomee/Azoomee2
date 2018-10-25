//
//  SettingsMessageBoxDeleteChild.h
//  azoomee2
//
//  Created by Macauley on 07/09/2018.
//

#ifndef SettingsMessageBoxDeleteChild_h
#define SettingsMessageBoxDeleteChild_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include "SettingsMessageBox.h"

NS_AZOOMEE_BEGIN

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

NS_AZOOMEE_END


#endif /* SettingsMessageBoxDeleteChild_h */
