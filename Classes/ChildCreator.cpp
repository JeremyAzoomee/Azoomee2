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
#include <AzoomeeCommon/Utils/TimeUtils.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/API/API.h>


using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string ChildCreator::kDefaultOomeeImgUrl = "https://media.azoomee.com/static/thumbs/oomee_11.png";

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

void ChildCreator::setCreatedChildId(const std::string &childId)
{
    _createdChildId = childId;
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

std::string ChildCreator::getCreatedChildId() const
{
    return _createdChildId;
}

void ChildCreator::setHttpRespnseDelegate(Azoomee::HttpRequestCreatorResponseDelegate *delegate)
{
    _delegate = delegate;
}

bool ChildCreator::addChild()
{
    if(_age <= 0 || !TextInputChecker::isValidChildName(_childName.c_str()))
    {
        return false;
    }
    
    int year = TimeUtils::birthYearFromAge(_age);
	
	if(!TextInputChecker::isDate(1, 1, year))
	{
		return false;
	}
	
    const std::string& DOB = StringUtils::format("%04d-%02d-%02d",year,1,1);
    const std::string& gender = "MALE";
	
    AnalyticsSingleton::getInstance()->childProfileCreatedEvent(_age);
    
    HttpRequestCreator* request = API::RegisterChildRequest(_childName, gender, DOB, kDefaultOomeeImgUrl, _delegate);
    
    request->execute();
    
    return true;
}

bool ChildCreator::updateChild(const ChildRef &child)
{
	if(_age <= 0 || !TextInputChecker::isValidChildName(_childName.c_str()))
	{
		return false;
	}
	
	int year = TimeUtils::birthYearFromAge(_age);

	if(!TextInputChecker::isDate(1, 1, year))
	{
		return false;
	}
	const std::string& DOB = StringUtils::format("%04d-%02d-%02d",year,1,1);
	
	const std::string& ownerId = UserAccountManager::getInstance()->getLoggedInParentId();

	AnalyticsSingleton::getInstance()->childProfileCreatedEvent(_age);
	
	HttpRequestCreator* request = API::UpdateChildRequest(child->getId(),_childName, child->getSex(), DOB, kDefaultOomeeImgUrl, ownerId, _delegate);
	request->execute();
	
	return true;
}

NS_AZOOMEE_END
