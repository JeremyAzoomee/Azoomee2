#include "ForceUpdateSingleton.h"
#include "BackEndCaller.h"

#include <external/json/document.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Utils/VersionChecker.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Utils/DirUtil.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "ForceUpdateAppLockScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";

#endif

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const std::string ForceUpdateSingleton::kAcceptedMinAzVerID = "acceptedMinAzoomeeVersion";
const std::string ForceUpdateSingleton::kNotifiedMinAzVerID = "notifiedMinAzoomeeVersion";
const std::string ForceUpdateSingleton::kAcceptedMinAzVerVodaID = "acceptedMinAzoomeeVersionVodacom";
const std::string ForceUpdateSingleton::kNotifiedMinAzVerVodaID = "notifiedMinAzoomeeVersionVodacom";
const std::string ForceUpdateSingleton::kUpdateUrlAppleID = "iosUpdateURL";
const std::string ForceUpdateSingleton::kUpdateUrlGoogleID = "androidUpdateURL";
const std::string ForceUpdateSingleton::kUpdateUrlAmazonID = "amazonUpdateURL";
const std::string ForceUpdateSingleton::kUpdateUrlVodaID = "vodacomUpdateURL";

std::auto_ptr<ForceUpdateSingleton> _sharedForceUpdateSingleton;
const std::string &forceUpdateDirectory = "updateData/";
const std::string &forceUpdateFileSubPath = forceUpdateDirectory + "updateData.json";
const int timeIntervalForRemoteFileDownloadInSeconds = 3600; //we check for new remote file every hour

ForceUpdateSingleton* ForceUpdateSingleton::getInstance()
{
    if (! _sharedForceUpdateSingleton.get())
    {
        _sharedForceUpdateSingleton.reset(new ForceUpdateSingleton());
    }
    
    return _sharedForceUpdateSingleton.get();
}

ForceUpdateSingleton::~ForceUpdateSingleton(void)
{
}

ForceUpdateSingleton::ForceUpdateSingleton()
{
    writablePath = DirUtil::getCachesPath();
}

void ForceUpdateSingleton::setDelegate(ForceUpdateDelegate *delegate)
{
	_delegate = delegate;
}

void ForceUpdateSingleton::doForceUpdateLogic()
{
	if(remoteForceUpdateDataDownloadRequired())
	{
		ModalMessages::getInstance()->startLoading();
		_fileDownloader = FileDownloader::create();
		_fileDownloader->setEtag(getLocalEtag());
		_fileDownloader->setDelegate(this);
		
		std::string url = "https://versions.azoomee.com";
		
	#ifdef AZOOMEE_ENVIRONMENT_CI
		url = "http://versions.azoomee.ninja";
	#endif
		
		_fileDownloader->downloadFileFromServer(url);
	}
	else
	{
		onForceUpdateLogicHasLocalFile();
	}
}

bool ForceUpdateSingleton::remoteForceUpdateDataDownloadRequired()
{
	if(!FileUtils::getInstance()->isFileExist(writablePath + forceUpdateFileSubPath))
	{
		createUpdateDirectory();
		return true;
	}
	
	const std::map<std::string, std::string>& localForceUpdateDataMap = getMapFromForceUpdateJsonData(FileUtils::getInstance()->getStringFromFile(writablePath + forceUpdateFileSubPath));
	
	if(localForceUpdateDataMap.find("timeStamp") == localForceUpdateDataMap.end())
	{
		return true;
	}
	if(time(NULL) - atoi(localForceUpdateDataMap.at("timeStamp").c_str()) >= timeIntervalForRemoteFileDownloadInSeconds)
	{
		return true;
	}
	return false;
}

bool ForceUpdateSingleton::parseAndSaveForceUpdateData(const std::string &jsonString)
{
	    std::map<std::string, std::string> forceUpdateData = getMapFromForceUpdateJsonData(jsonString);
	    forceUpdateData["timeStamp"] = StringUtils::format("%ld", time(NULL));
	    const std::string &jsonStringToBeWritten = getJSONStringFromMap(forceUpdateData);
	
	    FileUtils::getInstance()->writeStringToFile(jsonStringToBeWritten, writablePath + forceUpdateFileSubPath);
	
	    return true;
}

void ForceUpdateSingleton::onForceUpdateLogicHasLocalFile()
{
	ModalMessages::getInstance()->stopLoading();
    if(isNotificationRequired())
    {
        if(isAppCloseRequired())
        {
			if(_delegate)
			{
				_delegate->onForceUpdateCheckFinished(ForceUpdateResult::LOCK);
			}
        }
        else
        {
			if(_delegate)
			{
				_delegate->onForceUpdateCheckFinished(ForceUpdateResult::NOTIFY);
			}
        }
    }
	else
	{
		if(_delegate)
		{
			_delegate->onForceUpdateCheckFinished(ForceUpdateResult::DO_NOTHING);
		}
	}
	_delegate = nullptr;
}

void ForceUpdateSingleton::createUpdateDirectory()
{
    if(!FileUtils::getInstance()->isDirectoryExist(writablePath + forceUpdateDirectory))
    {
        FileUtils::getInstance()->createDirectory(writablePath + forceUpdateDirectory);
    }
}

std::string ForceUpdateSingleton::getLocalEtag() const
{
	const std::string& etagFilePath = writablePath + forceUpdateDirectory + "etag.txt";
	if(cocos2d::FileUtils::getInstance()->isFileExist(etagFilePath))
	{
		return cocos2d::FileUtils::getInstance()->getStringFromFile(etagFilePath);
	}
	return "";
}
void ForceUpdateSingleton::setLocalEtag(const std::string& etag)
{
	const std::string& etagFilePath = writablePath + forceUpdateDirectory + "etag.txt";
	cocos2d::FileUtils::getInstance()->writeStringToFile(etag, etagFilePath);
}

bool ForceUpdateSingleton::isNotificationRequired()
{
    return !azoomeeMeetsVersionRequirement(getAcceptedMinAzoomeeVersion()); //if acceptedMinAzoomeeVersion is not met, we need to at least notify, but check if close required.
}

bool ForceUpdateSingleton::isAppCloseRequired()
{
    return !azoomeeMeetsVersionRequirement(getNotifiedMinAzoomeeVersion()); //if not even the notifiedMinAzoomeeVersion is met, we need to close the app
}

std::string ForceUpdateSingleton::getAcceptedMinAzoomeeVersion()
{
	const auto& json = getMapFromForceUpdateJsonData(FileUtils::getInstance()->getStringFromFile(writablePath + forceUpdateFileSubPath));
#ifdef AZOOMEE_VODACOM_BUILD
	if(json.find(kAcceptedMinAzVerVodaID) != json.end())
	{
		return json.at(kAcceptedMinAzVerVodaID);
	}
#else
	if(json.find(kAcceptedMinAzVerID) != json.end())
	{
    	return json.at(kAcceptedMinAzVerID);
	}
#endif
	return "0.0.0";
}

std::string ForceUpdateSingleton::getNotifiedMinAzoomeeVersion()
{
	const auto& json = getMapFromForceUpdateJsonData(FileUtils::getInstance()->getStringFromFile(writablePath + forceUpdateFileSubPath));
#ifdef AZOOMEE_VODACOM_BUILD
	if(json.find(kNotifiedMinAzVerVodaID) != json.end())
	{
		return json.at(kNotifiedMinAzVerVodaID);
	}
#else
	if(json.find(kNotifiedMinAzVerID) != json.end())
	{
		return json.at(kNotifiedMinAzVerID);
	}
#endif
	return "0.0.0";
}

std::map<std::string, std::string> ForceUpdateSingleton::getMapFromForceUpdateJsonData(const std::string &forceUpdateJsonData)
{
    std::map<std::string, std::string> returnData;
    
    rapidjson::Document forceUpdateDataObject;
    forceUpdateDataObject.Parse(forceUpdateJsonData.c_str());
    
    if(forceUpdateDataObject.HasParseError()) return returnData;
    
    rapidjson::Value::MemberIterator M;
    
    for (M=forceUpdateDataObject.MemberBegin(); M!=forceUpdateDataObject.MemberEnd(); M++)
    {
        if(!M->value.IsNull())
        {
            if(M->value.IsString())
            {
                returnData[M->name.GetString()] = std::string(M->value.GetString());
            }
        }
    }
    
    return returnData;
}

std::string ForceUpdateSingleton::getUpdateUrlFromFile()
{
    const std::map<std::string, std::string> &forceUpdateData = getMapFromForceUpdateJsonData(FileUtils::getInstance()->getStringFromFile(writablePath + forceUpdateFileSubPath));
#ifdef AZOOMEE_VODACOM_BUILD
	if(forceUpdateData.find(kUpdateUrlVodaID) != forceUpdateData.end())
	{
		return forceUpdateData.at(kUpdateUrlVodaID);
	}
	return "";
#endif
		
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    std::string resultStr = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getOSBuildManufacturer");
    
    if (resultStr == "Amazon")
    {
        return forceUpdateData.at(kUpdateUrlAmazonID);
    }
    else
    {
        return forceUpdateData.at(kUpdateUrlGoogleID);
    }
#else
    return forceUpdateData.at(kUpdateUrlAppleID);
#endif
}

void ForceUpdateSingleton::onFileDownloadComplete(const std::string &fileString, const std::string &tag, long responseCode)
{
	if(responseCode == 200)
	{
		parseAndSaveForceUpdateData(fileString);
		setLocalEtag(_fileDownloader->getEtag());
	}
	onForceUpdateLogicHasLocalFile();
	
}

NS_AZOOMEE_END
