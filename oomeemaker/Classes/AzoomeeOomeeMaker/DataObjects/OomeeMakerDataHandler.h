//
//  OomeeMakerDataHandler.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 15/02/2018.
//

#ifndef OomeeMakerDataHandler_h
#define OomeeMakerDataHandler_h

#include "../AzoomeeOomeeMaker.h"
#include "OomeeMakerDataStorage.h"
#include <AzoomeeCommon/Utils/FileDownloader.h>
#include <AzoomeeCommon/Utils/FileZipUtil.h>

NS_AZOOMEE_OM_BEGIN

class OomeeMakerDataHandler : public FileDownloaderDelegate, public FileZipDelegate
{
private:
    const std::string kBaseFolderName = "OomeeMaker/";
    
    FileDownloaderRef _fileDownloader = nullptr;
    OomeeMakerDataStorage* _dataStorage = nullptr;
    
    void getPackageJson();
    void getConfigDataZip();
    
    void parseOomeeData();
    void parseCategoryData();
    void parseOomeeItemData();
    
    std::vector<std::string> getConfigFilesForType(const std::string& listType) const;
    
public:
    static OomeeMakerDataHandler* getInstance();
    virtual ~OomeeMakerDataHandler();
    void init();
    
    void getConfigFilesIfNeeded();
    
    // Delegate functions
    void onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath);
    void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode);
};

NS_AZOOMEE_OM_END

#endif /* OomeeMakerDataHandler_h */
