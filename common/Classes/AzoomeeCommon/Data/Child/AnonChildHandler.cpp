//
//  AnonChildHandler.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 24/01/2019.
//

#include "AnonChildHandler.h"
#include "ChildDataStorage.h"
#include "../ConfigStorage.h"
#include "../../Utils/SessionIdManager.h"
#include "../../Utils/DirectorySearcher.h"

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
	
	const std::string& imageData = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->getWritablePath() + child->getAvatar());
	char* str = nullptr;
	base64Encode((unsigned char*)imageData.c_str(), (unsigned int)imageData.length(), &str);
	
	HttpRequestCreator* request = API::RegisterChildRequestWithAvatarData(child->getProfileName(), child->getSex(), child->getDOB(), str, this); // avatar url to be replaced with avatar img data
	request->execute();
	
	_registerAnonChildCallback = callback;
}

void AnonChildHandler::copyAnonUserData()
{
	const std::string& newArtFolder = FileUtils::getInstance()->getWritablePath() + ConfigStorage::kArtCacheFolder + _newChildId + "/";
	const std::string& anonArtFolder = FileUtils::getInstance()->getWritablePath() + ConfigStorage::kArtCacheFolder + _oldAnonId + "/";
	
	if(!FileUtils::getInstance()->isDirectoryExist(newArtFolder))
	{
		FileUtils::getInstance()->createDirectory(newArtFolder);
	}
	
	if(FileUtils::getInstance()->isDirectoryExist(anonArtFolder))
	{
		const auto& anonArtList = DirectorySearcher::getInstance()->getImagesInDirectory(anonArtFolder);
		for(const std::string& art : anonArtList)
		{
			FileUtils::getInstance()->writeStringToFile(FileUtils::getInstance()->getStringFromFile(anonArtFolder + art), newArtFolder + art);
		}
	}
	
	const std::string& newOomeeFolder = FileUtils::getInstance()->getWritablePath() + ConfigStorage::kOomeeMakerCacheFolder + _newChildId + "/";
	const std::string& anonOomeeFolder = FileUtils::getInstance()->getWritablePath() + ConfigStorage::kOomeeMakerCacheFolder + _oldAnonId + "/";
	
	if(!FileUtils::getInstance()->isDirectoryExist(newOomeeFolder))
	{
		FileUtils::getInstance()->createDirectory(newOomeeFolder);
	}
	
	if(FileUtils::getInstance()->isDirectoryExist(anonOomeeFolder))
	{
		const auto& anonOomeeImgList = DirectorySearcher::getInstance()->getImagesInDirectory(anonOomeeFolder);
		for(const std::string& oomee : anonOomeeImgList)
		{
			FileUtils::getInstance()->writeStringToFile(FileUtils::getInstance()->getStringFromFile(anonOomeeFolder + oomee), newOomeeFolder + oomee);
		}
		const auto& anonOomeeDataList = DirectorySearcher::getInstance()->getFilesInDirectoryWithExtention(anonOomeeFolder, ".oomee");
		for(const std::string& oomee : anonOomeeDataList)
		{
			FileUtils::getInstance()->writeStringToFile(FileUtils::getInstance()->getStringFromFile(anonOomeeFolder + oomee), newOomeeFolder + oomee);
		}
	}
	
	const std::string& newRecentlyPlayedFolder = FileUtils::getInstance()->getWritablePath() + "RecentlyPlayed/" + _newChildId + "/";
	const std::string& anonRecentlyPlayedFolder = FileUtils::getInstance()->getWritablePath() + "RecentlyPlayed/" + _oldAnonId + "/";
	
	if(!FileUtils::getInstance()->isDirectoryExist(newRecentlyPlayedFolder))
	{
		FileUtils::getInstance()->createDirectory(newRecentlyPlayedFolder);
	}
	
	if(FileUtils::getInstance()->isDirectoryExist(anonRecentlyPlayedFolder))
	{
		const auto& anonRecentlyPlayedList = DirectorySearcher::getInstance()->getFilesInDirectory(anonRecentlyPlayedFolder);
		for(const std::string& file : anonRecentlyPlayedList)
		{
			FileUtils::getInstance()->writeStringToFile(FileUtils::getInstance()->getStringFromFile(anonRecentlyPlayedFolder + file), newRecentlyPlayedFolder + file);
		}
	}
}

//delegate functions
void AnonChildHandler::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagRegisterChild)
	{
		rapidjson::Document data;
		data.Parse(body.c_str());
		if(!data.HasParseError())
		{
			_newChildId = getStringFromJson("id", data);
		}
		
		std::string deviceId = ConfigStorage::getInstance()->getDeviceAdvertisingId();
		
		if(deviceId == "")
		{
			deviceId = "SESSID:" + SessionIdManager::getInstance()->getCurrentSessionId();
		}
		HttpRequestCreator* request = API::AnonymousDeviceLoginRequest(deviceId, this);
		request->execute();
		
	}
	if(requestTag == API::TagAnonymousDeviceLogin)
	{
		rapidjson::Document data;
		data.Parse(body.c_str());
		if(!data.HasParseError())
		{
			_oldAnonId = getStringFromJson("id", data);
		}
		
		if(_oldAnonId != "" && _newChildId != "")
		{
			copyAnonUserData();
			_oldAnonId = "";
			_newChildId = "";
		}
		
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
