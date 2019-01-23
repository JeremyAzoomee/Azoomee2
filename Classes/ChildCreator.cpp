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

bool ChildCreator::addLocalAnonChild()
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
	
	const std::string& localChildPath = FileUtils::getInstance()->getWritablePath() + "anonLocalChild/";
	const std::string& avatarImgPath = "anonLocalChild/avatar.png";
	
	const std::string& dataStr = StringUtils::format("{\"profileName\":\"%s\",\"sex\":\"%s\",\"dob\":\"%s\",\"avatar\":\"%s\"}",_childName.c_str(),gender.c_str(),DOB.c_str(),avatarImgPath.c_str());
	
	if(!FileUtils::getInstance()->isDirectoryExist(localChildPath))
	{
		FileUtils::getInstance()->createDirectory(localChildPath);
	}
	
	FileUtils::getInstance()->writeStringToFile(dataStr, localChildPath + "childData.json");
	
	if(!FileUtils::getInstance()->isFileExist(avatarImgPath))
	{
		FileUtils::getInstance()->writeStringToFile(FileUtils::getInstance()->getStringFromFile("res/defaultOomees/oomee_11.png"), FileUtils::getInstance()->getWritablePath() + avatarImgPath);
		FileUtils::getInstance()->writeStringToFile(FileUtils::getInstance()->getStringFromFile("res/defaultOomees/oomee_11.oomee"), localChildPath + "avatar.oomee");
	}
	
	return true;
}

NS_AZOOMEE_END
