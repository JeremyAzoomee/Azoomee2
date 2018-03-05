//
//  ArtAppDelegate.h
//  azoomee2
//
//  Created by Macauley on 05/07/2017.
//
//

#ifndef ArtAppDelegate_h
#define ArtAppDelegate_h

#include <AzoomeeCommon/Azoomee.h>
#include "../ArtApp/Classes/AzoomeeArt/AzoomeeArtApp.h"
#include <string>
#include <memory>

NS_AZOOMEE_BEGIN

class ArtAppDelegate: public Azoomee::ArtApp::Delegate
{
public:
    static ArtAppDelegate* getInstance();
    
    virtual void onArtAppNavigationBack() override;
    virtual void onArtAppShareImage() override;
    virtual std::string getFileName() const override;
    virtual void setFileName(const std::string& fileName) override;
    void setSecondsSpentInArtApp(long seconds) override;
    long getSecondsSpentInArtApp() const;
    
private:
    long _secondsSpentInArtApp = 0;
    
};

NS_AZOOMEE_END

#endif /* ArtAppDelegate_h */
