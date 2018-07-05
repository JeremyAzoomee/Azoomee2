//
//  ChildCreator.h
//  azoomee2
//
//  Created by Macauley on 03/07/2018.
//

#ifndef ChildCreator_h
#define ChildCreator_h

#include <memory>
#include <string>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>

NS_AZOOMEE_BEGIN

class ChildCreator;
typedef std::shared_ptr<ChildCreator> ChildCreatorRef;

class ChildCreator
{
private:
    std::string _childName = "";
    int _age = -1;
    HttpRequestCreatorResponseDelegate* _delegate = nullptr;
    
public:
    static ChildCreatorRef create();
    
    void setChildName(const std::string& childName);
    void setAge(int age);
    
    std::string getName() const;
    int getAge() const;
    
    void setHttpRespnseDelegate(HttpRequestCreatorResponseDelegate* delegate);
    
    bool addChild();
};

NS_AZOOMEE_END

#endif /* ChildCreator_h */
