//
//  OomeeMakerDelegate.h
//  azoomee2
//
//  Created by Macauley on 23/02/2018.
//

#ifndef OomeeMakerDelegate_h
#define OomeeMakerDelegate_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeOomeeMaker/AzoomeeOomeeMaker.h>
#include <string>
#include <memory>

NS_AZOOMEE_BEGIN

class OomeeMakerDelegate: public Azoomee::OomeeMaker::Delegate
{
public:
    static OomeeMakerDelegate* getInstance();
    
    virtual void onOomeeMakerNavigationBack() override;
    virtual void onOomeeMakerShareOomee(const std::string& filename) override;
};

NS_AZOOMEE_END


#endif /* OomeeMakerDelegate_h */
