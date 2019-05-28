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
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include <string>
#include <memory>

NS_AZOOMEE_BEGIN

class OomeeMakerDelegate: public Azoomee::OomeeMaker::Delegate, HttpRequestCreatorResponseDelegate
{
public:
    static OomeeMakerDelegate* getInstance();
    
    virtual void onOomeeMakerNavigationBack() override;
    virtual void onOomeeMakerShareOomee(const std::string& filename) override;
    virtual void onOomeeMakerUpdateAvatar(const std::string& filename) override;
	
    virtual void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    virtual void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
	
	bool _oomeeMakerFromShop = false;
};

NS_AZOOMEE_END


#endif /* OomeeMakerDelegate_h */
