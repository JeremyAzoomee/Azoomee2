//
//  AnonChildHandler.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 24/01/2019.
//

#include "AnonChildHandler.h"
#include "ChildDataStorage.h"
#include "../ConfigStorage.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<AnonChildHandler> sAnonChildHandlerSharedInstance;

AnonChildHandler* AnonChildHandler::getInstance(void)
{
	if(!sAnonChildHandlerSharedInstance.get())
	{
		sAnonChildHandlerSharedInstance.reset(new AnonChildHandler());
		sAnonChildHandlerSharedInstance->init();
	}
	return sAnonChildHandlerSharedInstance.get();
}

AnonChildHandler::~AnonChildHandler()
{
	
}
bool AnonChildHandler::init(void)
{
	return true;
}

void AnonChildHandler::loginAnonChild(const std::string& loginDataStr)
{
	const std::string& localChildPath = FileUtils::getInstance()->getWritablePath() + "anonLocalChild/childData.json";
	if(!FileUtils::getInstance()->isFileExist(localChildPath))
	{
		return;
	}
	
	auto childDataStorage = ChildDataStorage::getInstance();
	
	rapidjson::Document childData;
	childData.Parse(FileUtils::getInstance()->getStringFromFile(localChildPath).c_str());
	if(childData.HasParseError())
	{
		return;
	}
	
	ChildRef child = Child::createWithJson(childData);
	
	rapidjson::Document loginData;
	loginData.Parse(loginDataStr.c_str());
	if(loginData.HasParseError())
	{
		return;
	}
	
	child->parseLoginData(loginData);
	
	child->setId(getStringFromJson("id",loginData));
	childDataStorage->setLoggedInChild(child);
	childDataStorage->setChildLoggedIn(true);
}

bool AnonChildHandler::localAnonChildExists() const
{
	return FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + "anonLocalChild/childData.json");
}
ChildRef AnonChildHandler::getLocalAnonChild() const
{
	const std::string& localChildPath = FileUtils::getInstance()->getWritablePath() + "anonLocalChild/childData.json";
	if(!FileUtils::getInstance()->isFileExist(localChildPath))
	{
		return Child::create();
	}
	
	rapidjson::Document childData;
	childData.Parse(FileUtils::getInstance()->getStringFromFile(localChildPath).c_str());
	if(childData.HasParseError())
	{
		return Child::create();
	}
	
	return Child::createWithJson(childData);
}

void AnonChildHandler::registerAnonChildToUser(const RegisterAnonChildCallback& callback)
{
	if(!localAnonChildExists())
	{
		if(callback)
		{
			callback(false);
		}
		return;
	}
	
	const ChildRef& child = getLocalAnonChild();
	
	HttpRequestCreator* request = API::RegisterChildRequest(child->getProfileName(), child->getSex(), child->getDOB(), ConfigStorage::getInstance()->getUrlForOomee(0), this);
	request->execute();
	
	_registerAnonChildCallback = callback;
}

//delegate functions
void AnonChildHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagRegisterChild)
	{
		
	}
	if(requestTag == API::TagUpdateChildAvatar)
	{
		if(_registerAnonChildCallback)
		{
			_registerAnonChildCallback(true);
		}
		_registerAnonChildCallback = nullptr;
	}
}
void AnonChildHandler::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	if(_registerAnonChildCallback)
	{
		_registerAnonChildCallback(false);
	}
	_registerAnonChildCallback = nullptr;
}

NS_AZOOMEE_END
