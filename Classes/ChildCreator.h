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

NS_AZOOMEE_BEGIN

class ChildCreator;
typedef std::shared_ptr<ChildCreator> ChildCreatorRef;

class ChildCreator
{
    std::string _childName = "";
    int _age = -1;
    
public:
    static ChildCreatorRef create();
    
    void setChildName(const std::string& childName);
    void setAge(int age);
    
    bool addChild();
};

NS_AZOOMEE_END

#endif /* ChildCreator_h */
