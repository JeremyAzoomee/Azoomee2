#include "ForceUpdateSingleton.h"
#include "BackEndCaller.h"

#include <external/json/document.h>
#include <AzoomeeCommon/Utils/VersionChecker.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "ForceUpdateAppLockScene.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static ForceUpdateSingleton *_sharedForceUpdateSingleton = NULL;
const std::string forceUpdateDirectory = "updateData/";
const std::string forceUpdateFileSubPath = forceUpdateDirectory + "updateData.json";
const int timeIntervalForRemoteFileDownloadInSeconds = 259200; //we check for new remote file every 3rd day

ForceUpdateSingleton* ForceUpdateSingleton::getInstance()
{
    if (! _sharedForceUpdateSingleton)
    {
        _sharedForceUpdateSingleton = new ForceUpdateSingleton();
        _sharedForceUpdateSingleton->init();
    }
    
    return _sharedForceUpdateSingleton;
}

ForceUpdateSingleton::~ForceUpdateSingleton(void)
{
}

bool ForceUpdateSingleton::init(void)
{
    writablePath = FileUtils::getInstance()->getWritablePath();
    return true;
}

void ForceUpdateSingleton::doForceUpdateLogic() //part 1, we check if we have local file. If we have, we go to part 2 (onForceUpdateLogicHasLocalFile(), otherwise we start downloading the file, writing it to disk and getting back to the same method)
{
    if(remoteForceUpdateDataDownloadRequired()) getRemoteForceUpdateData();
    else onForceUpdateLogicHasLocalFile();
}

void ForceUpdateSingleton::onForceUpdateLogicHasLocalFile()
{
    if(isNotificationRequired())
    {
        if(isAppCloseRequired())
        {
            Director::getInstance()->replaceScene(ForceUpdateAppLockScene::createScene());
        }
        else
        {
            std::vector<std::string> buttonNames = {"OK", "Update"};
            MessageBox::createWith("Update recommended", "You should update to the\nlatest version of Azoomee.\nAsk a grown-up to help you.", buttonNames, this);
        }
    }
}

bool ForceUpdateSingleton::remoteForceUpdateDataDownloadRequired()
{
    if(!FileUtils::getInstance()->isFileExist(writablePath + forceUpdateFileSubPath))
    {
        createUpdateDirectory();
        return true;
    }
    
    std::map<std::string, std::string> localForceUpdateDataMap = getMapFromForceUpdateJsonData(FileUtils::getInstance()->getStringFromFile(writablePath + forceUpdateFileSubPath));
    
    if(localForceUpdateDataMap["timeStamp"] == "") return true;
    if(time(NULL) - atoi(localForceUpdateDataMap["timeStamp"].c_str()) >= timeIntervalForRemoteFileDownloadInSeconds) return true;
    
    return false;
}

void ForceUpdateSingleton::createUpdateDirectory()
{
    if(!FileUtils::getInstance()->isDirectoryExist(writablePath + forceUpdateDirectory))
    {
        FileUtils::getInstance()->createDirectory(writablePath + forceUpdateDirectory);
    }
}

void ForceUpdateSingleton::getRemoteForceUpdateData()
{
    BackEndCaller::getInstance()->getForceUpdateData();
}

void ForceUpdateSingleton::onForceUpdateDataReceived(const std::string &responseString)
{
    if(parseAndSaveForceUpdateData(responseString))
    {
        onForceUpdateLogicHasLocalFile();
    }
}

bool ForceUpdateSingleton::parseAndSaveForceUpdateData(const std::string &jsonString)
{
    std::map<std::string, std::string> forceUpdateData = getMapFromForceUpdateJsonData(jsonString);
    forceUpdateData["timeStamp"] = StringUtils::format("%ld", time(NULL));
    std::string jsonStringToBeWritten = getJSONStringFromMap(forceUpdateData);
    
    FileUtils::getInstance()->writeStringToFile(jsonStringToBeWritten, writablePath + forceUpdateFileSubPath);
    
    return true;
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
    return getMapFromForceUpdateJsonData(FileUtils::getInstance()->getStringFromFile(writablePath + forceUpdateFileSubPath)).at("acceptedMinAzoomeeVersion");
}

std::string ForceUpdateSingleton::getNotifiedMinAzoomeeVersion()
{
    return getMapFromForceUpdateJsonData(FileUtils::getInstance()->getStringFromFile(writablePath + forceUpdateFileSubPath)).at("notifiedMinAzoomeeVersion");
}

std::map<std::string, std::string> ForceUpdateSingleton::getMapFromForceUpdateJsonData(const std::string &forceUpdateJsonData)
{
    std::map<std::string, std::string> returnData;
    
    rapidjson::Document forceUpdateDataObject;
    forceUpdateDataObject.Parse(forceUpdateJsonData.c_str());
    
    if(forceUpdateDataObject.HasParseError()) return returnData;
    
    rapidjson::Value::MemberIterator M;
    const char *key;
    
    for (M=forceUpdateDataObject.MemberBegin(); M!=forceUpdateDataObject.MemberEnd(); M++)
    {
        key   = M->name.GetString();
        
        if(!forceUpdateDataObject[key].IsNull())
        {
            if(forceUpdateDataObject[key].IsString())
            {
                returnData[key] = std::string(forceUpdateDataObject[key].GetString());
            }
        }
    }
    
    return returnData;
}

std::string ForceUpdateSingleton::getUpdateUrlFromFile()
{
    std::map<std::string, std::string> forceUpdateData = getMapFromForceUpdateJsonData(FileUtils::getInstance()->getStringFromFile(writablePath + forceUpdateFileSubPath));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "getOSBuildManufacturer", "()Ljava/lang/String;");
    jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
    const char *resultCStr = t.env->GetStringUTFChars(str, NULL);
    std::string resultStr(resultCStr);
    
    t.env->DeleteLocalRef(t.classID);
    t.env->DeleteLocalRef(str);
    
    if (resultStr == "Amazon")
    {
        return forceUpdateData["amazonUpdateURL"];
    }
    else
    {
        return forceUpdateData["androidUpdateURL"];
    }
#else
    return forceUpdateData["iosUpdateURL"];
#endif
}

void ForceUpdateSingleton::MessageBoxButtonPressed(std::string messageBoxTitle, std::string buttonTitle)
{
    if(buttonTitle == "Update") Application::getInstance()->openURL(getUpdateUrlFromFile());
}


NS_AZOOMEE_END
