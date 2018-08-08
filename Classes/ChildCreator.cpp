//
//  ChildCreator.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 03/07/2018.
//

#include "ChildCreator.h"
#include "FlowDataSingleton.h"
#include <AzoomeeCommon/Input/TextInputChecker.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Utils/TimeFunctions.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/API/API.h>


using namespace cocos2d;

NS_AZOOMEE_BEGIN

ChildCreatorRef ChildCreator::create()
{
    return std::make_shared<ChildCreator>();
}

void ChildCreator::setChildName(const std::string& childName)
{
    _childName = childName;
}

void ChildCreator::setAge(int age)
{
    _age = age;
}

void ChildCreator::setFirstTime(bool firstTime)
{
    _firstTime = firstTime;
}

std::string ChildCreator::getName() const
{
    return _childName;
}

int ChildCreator::getAge() const
{
    return _age;
}

int ChildCreator::getOomeeNum() const
{
    return _oomeeNum;
}

void ChildCreator::setHttpRespnseDelegate(Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    _delegate = delegate;
}

bool ChildCreator::addChild()
{
    if(_age <= 0 || !isValidChildName(_childName.c_str()))
    {
        return false;
    }
    
    int year = birthYearFromAge(_age);
    
    const std::string& DOB = StringUtils::format("%04d-%02d-%02d",year,1,1);
    const std::string& gender = "MALE";
    if(!isDate(1, 1, year))
    {
        return false;
    }
    _oomeeNum = RandomHelper::random_int(0, 4);
    AnalyticsSingleton::getInstance()->childProfileCreatedEvent(_age);
    
    const std::string& oomeeUrl = ConfigStorage::getInstance()->getUrlForOomee(_oomeeNum);
    HttpRequestCreator* request = API::RegisterChildRequest(_childName, gender, DOB, oomeeUrl, _delegate);
    
    request->execute();
    
    return true;
}

NS_AZOOMEE_END
