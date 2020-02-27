//
//  ArtAppDelegate.h
//  azoomee2
//
//  Created by Macauley on 05/07/2017.
//
//

#ifndef ArtAppDelegate_h
#define ArtAppDelegate_h

#include <TinizineCommon/Tinizine.h>
#include "../ArtApp/Classes/AzoomeeArt/AzoomeeArtApp.h"
#include <string>
#include <memory>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ArtAppDelegate: public AZ::ArtApp::Delegate
{
public:
    static ArtAppDelegate* getInstance();
    
    virtual void onArtAppNavigationBack() override;
    virtual void onArtAppShareImage() override;
    virtual std::string getFileName() const override;
    virtual void setFileName(const std::string& fileName) override;
	virtual bool isOffline() override;
    void setSecondsSpentInArtApp(long seconds) override;
    long getSecondsSpentInArtApp() const;
    
private:
    long _secondsSpentInArtApp = 0;
};

NS_AZ_END

#endif /* ArtAppDelegate_h */
