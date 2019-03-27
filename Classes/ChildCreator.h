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
#include <AzoomeeCommon/Data/Child/Child.h>

NS_AZOOMEE_BEGIN

class ChildCreator;
typedef std::shared_ptr<ChildCreator> ChildCreatorRef;

class ChildCreator
{
private:
    std::string _childName = "";
    int _age = -1;
    int _oomeeNum = 0;
    bool _firstTime = false;
    std::string _createdChildId = "";
    HttpRequestCreatorResponseDelegate* _delegate = nullptr;
    
public:
    static ChildCreatorRef create();
    
    void setChildName(const std::string& childName);
    void setAge(int age);
    void setFirstTime(bool firstTime);
    void setCreatedChildId(const std::string& childId);
    
    std::string getName() const;
    int getAge() const;
    int getOomeeNum() const;
    std::string getCreatedChildId() const;
    
    void setHttpRespnseDelegate(HttpRequestCreatorResponseDelegate* delegate);
    
    bool addChild();
	bool updateChild(const ChildRef& child);
};

NS_AZOOMEE_END

#endif /* ChildCreator_h */
