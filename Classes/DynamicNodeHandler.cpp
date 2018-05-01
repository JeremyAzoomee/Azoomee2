//
//  DynamicNodeHandler.cpp
//  azoomee2
//
//  Created by Macauley on 25/09/2017.
//
//

#include "DynamicNodeHandler.h"
#include "SignUpFlowController.h"
#include "IAPFlowController.h"
#include "AddChildFlowController.h"
#include "DynamicNodeDataInputStorage.h"
#include "RoutePaymentSingleton.h"
#include "FlowDataSingleton.h"
#include "IAPProductDataHandler.h"
#include <dirent.h>
#include <AzoomeeCommon/Data/Json.h>
#include <AzoomeeCommon/Data/Cookie/CookieDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Utils/DirectorySearcher.h>
#include <AzoomeeCommon/Utils/VersionChecker.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>

//#define USING_LOCAL_CTA_ASSETS YES

using namespace cocos2d;
NS_AZOOMEE_BEGIN
//-----start Popup group names
const std::string DynamicNodeHandler::kUpgradeGroup = "upgrade";

//-----end popup group names

static std::auto_ptr<DynamicNodeHandler> sDynamicNodeHandlerSharedInstance;

DynamicNodeHandler* DynamicNodeHandler::getInstance()
{
    if(!sDynamicNodeHandlerSharedInstance.get())
    {
        sDynamicNodeHandlerSharedInstance.reset(new DynamicNodeHandler());
        sDynamicNodeHandlerSharedInstance->init();
    }
    return sDynamicNodeHandlerSharedInstance.get();
}

DynamicNodeHandler::~DynamicNodeHandler(void)
{
    
}

bool DynamicNodeHandler::init(void)
{
    const std::string& ctaPath = getCTADirectoryPath();
    if(!FileUtils::getInstance()->isDirectoryExist(ctaPath))
    {
        FileUtils::getInstance()->createDirectory(ctaPath);
    }
    return true;
}

void DynamicNodeHandler::createDynamicNodeById(const std::string& uniqueId)
{
    //local device folder
    const std::string& ctaPath = getCTADirectoryPath();
    const std::vector<std::string>& folders = DirectorySearcher::getInstance()->getFoldersInDirectory(ctaPath);
    
    for(const std::string& folder : folders)
    {
        const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getJsonFilesInDirectory(ctaPath + folder);
        for(const std::string& file : fileNames)
        {
            if(file == uniqueId)
            {
                AnalyticsSingleton::getInstance()->ctaWindowAppeared("N/A", uniqueId);
                createDynamicNodeFromFile(ctaPath + folder + "/" + file);
                return;
            }
        }
    }
    
}

void DynamicNodeHandler::createDynamicNodeByGroupId(const std::string& groupId)
{
    //local device folder
    const std::string& ctaPath = getCTADirectoryPath();
    const std::vector<std::string>& folders = DirectorySearcher::getInstance()->getFoldersInDirectory(ctaPath);
    
    for(const std::string& folder : folders)
    {
        if(folder == groupId)
        {
            const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getJsonFilesInDirectory(ctaPath + folder);
            
            int randomFileNameIndex = rand()%fileNames.size();
            AnalyticsSingleton::getInstance()->ctaWindowAppeared(groupId, fileNames[randomFileNameIndex]);
            createDynamicNodeFromFile(ctaPath + folder + "/" + fileNames[randomFileNameIndex]);
            return;
                
        }
    }

}

void DynamicNodeHandler::createDynamicNodeByIdWithParams(const std::string& uniqueId, const std::string& params)
{
    //local device folder
    const std::string& ctaPath = getCTADirectoryPath();
    const std::vector<std::string>& folders = DirectorySearcher::getInstance()->getFoldersInDirectory(ctaPath);
    
    for(const std::string& folder : folders)
    {
        const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getJsonFilesInDirectory(ctaPath + folder);
        for(const std::string& file : fileNames)
        {
            if(file == uniqueId)
            {
                AnalyticsSingleton::getInstance()->ctaWindowAppeared("N/A", uniqueId);
                createDynamicNodeFromFileWithParams(ctaPath + folder + "/" + file, params);
                return;
            }
        }
    }
    
}

void DynamicNodeHandler::createDynamicNodeByGroupIdWithParams(const std::string& groupId, const std::string& params)
{
    //local device folder
    const std::string& ctaPath = getCTADirectoryPath();
    const std::vector<std::string>& folders = DirectorySearcher::getInstance()->getFoldersInDirectory(ctaPath);
    
    for(const std::string& folder : folders)
    {
        if(folder == groupId)
        {
            const std::vector<std::string>& fileNames = DirectorySearcher::getInstance()->getJsonFilesInDirectory(ctaPath + folder);
            
            int randomFileNameIndex = rand()%fileNames.size();
            AnalyticsSingleton::getInstance()->ctaWindowAppeared(groupId, fileNames[randomFileNameIndex]);
            createDynamicNodeFromFileWithParams(ctaPath + folder + "/" + fileNames[randomFileNameIndex], params);
            return;
            
        }
    }
    
}

void DynamicNodeHandler::startSignupFlow()
{
    _flowController = SignUpFlowController::create();
    createDynamicNodeByIdWithParams(_flowController->_flowEntryFile, DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
}

void DynamicNodeHandler::startIAPFlow()
{
    if(RoutePaymentSingleton::getInstance()->receiptDataFileExists())
    {
        if(!ParentDataProvider::getInstance()->isUserLoggedIn())
        {
            FlowDataSingleton::getInstance()->setSuccessFailPath(IAP_SUCCESS);
            handleSuccessFailEvent();
            return;
        }
        else
        {
            RoutePaymentSingleton::getInstance()->retryReceiptValidation();
            return;
        }
    }
    _flowController = IAPFlowController::create();
    createDynamicNodeByGroupIdWithParams(_flowController->_flowEntryFile, getJSONStringFromMap({
        {"iapPrice",IAPProductDataHandler::getInstance()->getHumanReadableProductPrice()}
    }));
}

void DynamicNodeHandler::startAddChildFlow()
{
    _flowController = AddChildFlowController::create();
    createDynamicNodeByIdWithParams(_flowController->_flowEntryFile, DynamicNodeDataInputStorage::getInstance()->getStorageAsJsonString());
}

void DynamicNodeHandler::handleSuccessFailEvent()
{
    switch(FlowDataSingleton::getInstance()->getSuccessFailPath())
    {
        case SUCCESS_FAIL_NONE:
            return;
            break;
        case IAP_SUCCESS:
            createDynamicNodeById("payment_success.json");
            break;
        case IAP_FAIL:
            createDynamicNodeById("payment_failed.json");
            break;
        case SIGNUP_SUCCESS: case SIGNUP_FAIL: case ADDCHILD_SUCCESS: case ADDCHILD_FAIL:
        {
            break;
        }
        case PREMIUM_NEW_ACCOUNT:
        {
            FlowDataSingleton::getInstance()->setFlowToSignUpNewProfile();
            createDynamicNodeById("payment_new_account.json");
            break;
        }
        case PREMIUM_EXISTING_ACCOUNT:
        {
            createDynamicNodeById("payment_existing_account.json");
            break;
        }
    }
    FlowDataSingleton::getInstance()->setSuccessFailPath(SUCCESS_FAIL_NONE);
}

void DynamicNodeHandler::getCTAFiles()
{
#ifdef USING_LOCAL_CTA_ASSETS
    removeCTAFiles();
    unzipBundleCTAFiles();
#else
    checkIfVersionChangedFromLastCTAPull();
    getCTAPackageJSON(ConfigStorage::getInstance()->getCTAPackageJsonURL());
#endif
}

rapidjson::Document DynamicNodeHandler::getLocalCTAPackageJSON()
{
    rapidjson::Document packageJson;
    if(isCTAPackageJSONExist())
    {
        const std::string& currentJsonPackage = FileUtils::getInstance()->getStringFromFile(getPackageJsonLocation());
        packageJson.Parse(currentJsonPackage.c_str());
    }
    return packageJson;
}

bool DynamicNodeHandler::isCTAPackageJSONExist()
{
    return FileUtils::getInstance()->isFileExist(getPackageJsonLocation());
}

void DynamicNodeHandler::checkIfVersionChangedFromLastCTAPull()
{
    const std::string& lastPullAppVersionFile = getLastPullAppVersionFilePath();
    if(!FileUtils::getInstance()->isFileExist(lastPullAppVersionFile))
    {
        FileUtils::getInstance()->writeStringToFile(ConfigStorage::getInstance()->getVersionNumber(), lastPullAppVersionFile);
        return;
    }
    
    const std::string& lastPullAppVersion = FileUtils::getInstance()->getStringFromFile(lastPullAppVersionFile);
    
    if(!azoomeeVersionEqualsVersionNumber(lastPullAppVersion))
    {
        removeCTAFiles();
        FileUtils::getInstance()->removeFile(getPackageJsonLocation());
    }
}

void DynamicNodeHandler::getCTAPackageJSON(const std::string& url)
{
    _fileDownloader = FileDownloader::create();
    _fileDownloader->setDelegate(this);
    _fileDownloader->downloadFileFromServer(url,_kJsonTag);
}

void DynamicNodeHandler::getCTAPackageZip(const std::string& url)
{
    _fileDownloader = FileDownloader::create();
    _fileDownloader->setDelegate(this);
    _fileDownloader->downloadFileFromServer(url,_kZipTag);
}

std::string DynamicNodeHandler::getPackageJsonLocation() const
{
    return getCTADirectoryPath() + "package.json";
}

std::string DynamicNodeHandler::getCTADirectoryPath() const
{
    return FileUtils::getInstance()->getWritablePath() + "DCDECache/";
}

std::string DynamicNodeHandler::getBundledAssetsPath() const
{
    return "res/cta_assets/CTAFiles.zip";
}

std::string DynamicNodeHandler::getLastPullAppVersionFilePath() const
{
    return getCTADirectoryPath() + "lastPullAppVersion.txt";
}

bool DynamicNodeHandler::unzipCTAFiles(const char *zipPath, const char *dirpath, const char *passwd)
{
    FileZipUtil::getInstance()->asyncUnzip(zipPath, dirpath, "", this);

    return true;
}

bool DynamicNodeHandler::unzipBundleCTAFiles()
{
    const std::string& basePath = getCTADirectoryPath();
    const std::string& targetPath = getBundledAssetsPath();
    const std::string& destinationPath = basePath + "CTAFiles.zip";
    const std::string& zipFile = FileUtils::getInstance()->getStringFromFile(targetPath);
    
    FileUtils::getInstance()->writeStringToFile(zipFile, destinationPath);

    FileZipUtil::getInstance()->asyncUnzip(destinationPath,basePath, "", this);

    return true;
}

void DynamicNodeHandler::removeCTAFiles()
{
    const std::string& baseLocation = getCTADirectoryPath();
    const std::vector<std::string>& CTAFolders = DirectorySearcher::getInstance()->getFoldersInDirectory(baseLocation);
    for(const std::string& folder : CTAFolders)
    {
        if(folder.size() > 2)
        {
            FileUtils::getInstance()->removeDirectory(baseLocation + folder);
        }
    }
    
}

void DynamicNodeHandler::createDynamicNodeFromFile(const std::string &file)
{
    Node* cta = DynamicNodeCreator::getInstance()->createCTAFromFile(file);
    Director::getInstance()->getRunningScene()->addChild(cta);
}

void DynamicNodeHandler::createDynamicNodeFromFileWithParams(const std::string &file, const std::string& params)
{
    Node* cta = DynamicNodeCreator::getInstance()->createCTAFromFileWithParams(file, params);
    Director::getInstance()->getRunningScene()->addChild(cta);
}

void DynamicNodeHandler::onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath)
{
    FileUtils::getInstance()->removeFile(zipPath);
}

void DynamicNodeHandler::jsonDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode)
{
    if (responseCode == 200)
    {
        const std::string& targetPath = getPackageJsonLocation();
        rapidjson::Document newPackageJSON;
        newPackageJSON.Parse(fileString.c_str());
        
        if(!newPackageJSON.HasParseError())
        {
            if(isCTAPackageJSONExist())
            {
                rapidjson::Document oldPackageJSON = getLocalCTAPackageJSON(); //dont need to error check as wont make it on to device if invalid
                
                if(oldPackageJSON["currentVersion"].GetInt() < newPackageJSON["currentVersion"].GetInt() && azoomeeMeetsVersionRequirement(newPackageJSON["minAzoomeeVersion"].GetString()))
                {
                    FileUtils::getInstance()->writeStringToFile(fileString, targetPath);
                    getCTAPackageZip(getStringFromJson("uri", newPackageJSON));
                    return;
                }
            }
            else
            {
                if(azoomeeMeetsVersionRequirement(newPackageJSON["minAzoomeeVersion"].GetString()))
                {
                    FileUtils::getInstance()->writeStringToFile(fileString, targetPath);
                    getCTAPackageZip(getStringFromJson("uri", newPackageJSON));
                    return;
                }
                else
                {
                    unzipBundleCTAFiles();
                    return;
                }
            }
        }
    }
    else
    {
        if(!isCTAPackageJSONExist())
        {
            unzipBundleCTAFiles();
        }
    }
}

void DynamicNodeHandler::zipDownloadComplte(const std::string& fileString, const std::string& tag, long responseCode)
{
    if(responseCode == 200)
    {
        const std::string& basePath = getCTADirectoryPath();
        const std::string& targetPath = basePath + "CTAFiles.zip";
        FileUtils::getInstance()->writeStringToFile(fileString, targetPath);
        removeCTAFiles();
        unzipCTAFiles(targetPath.c_str(), basePath.c_str(), nullptr);
        FileUtils::getInstance()->writeStringToFile(ConfigStorage::getInstance()->getVersionNumber(), getLastPullAppVersionFilePath());
    }
    else
    {
        unzipBundleCTAFiles();
    }
}

void DynamicNodeHandler::setFlowController(DynamicNodeFlowControllerRef flowController)
{
    _flowController = flowController;
}

DynamicNodeFlowControllerRef DynamicNodeHandler::getFlowController()
{
    return _flowController;
}

// Delegate functions

void DynamicNodeHandler::onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode)
{
    // json file download return
    if(tag == _kJsonTag)
    {
        jsonDownloadComplete(fileString, tag, responseCode);
    }
    // zip file return
    else if(tag == _kZipTag)
    {
        zipDownloadComplte(fileString, tag, responseCode);
    }
}

NS_AZOOMEE_END
