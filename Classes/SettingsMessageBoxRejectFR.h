//
//  SettingsMessageBoxRejectFR.h
//  azoomee2
//
//  Created by Macauley on 06/09/2018.
//

#ifndef SettingsMessageBoxRejectFR_h
#define SettingsMessageBoxRejectFR_h

#include "SettingsMessageBox.h"

NS_AZOOMEE_BEGIN

class SettingsMessageBoxRejectFR : public SettingsMessageBox
{
    typedef SettingsMessageBox Super;
private:
    SettingsMessageBoxButton* _cancelButton = nullptr;
    SettingsMessageBoxButton* _rejectButton = nullptr;
    cocos2d::Label* _heading = nullptr;
    
public:
    
    virtual void onEnter() override;
    
    CREATE_FUNC(SettingsMessageBoxRejectFR);
};

NS_AZOOMEE_END

#endif /* SettingsMessageBoxRejectFR_h */
