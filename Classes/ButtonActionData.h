//
//  ButtonActionData.h
//  azoomee2
//
//  Created by Macauley on 25/09/2017.
//
//

#ifndef ButtonActionData_h
#define ButtonActionData_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_BEGIN

class ButtonActionData;
typedef std::shared_ptr<ButtonActionData> ButtonActionDataRef;

class ButtonActionData
{
private:
    std::string _type;
    std::map<std::string, std::string> _params;
    ButtonActionData();
public:
    
    
    static ButtonActionDataRef createWithJson(const rapidjson::Value& actionData);
    
    std::string getType();
    std::map<std::string,std::string> getParams();
    //overide function for setting parameters manually
    void setParams(const std::map<std::string, std::string>& updatedParams);
    std::string getParamForKey(const std::string& key);
    
};

NS_AZOOMEE_END

#endif /* ButtonActionData_h */
