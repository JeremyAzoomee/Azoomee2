//
//  FileZipUtil.h
//  AzoomeeCommon
//
//  Created by Macauley on 28/09/2017.
//
//

#ifndef FileZipUtil_h
#define FileZipUtil_h

#include "cocos2d.h"
#include "external/unzip/unzip.h"
#include "../Tinizine.h"

NS_TZ_BEGIN

    class FileZipDelegate
    {
    public:
        virtual void onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath) = 0;
    };
    
    class FileZipUtil : public cocos2d::Ref
    {
    private:
        void unzipWithDelageteCallback(std::string zipPath, std::string dirpath, std::string passwd, FileZipDelegate* delegate);
    public:
        static FileZipUtil* getInstance(void);
        virtual ~FileZipUtil();
    
        bool unzip(const char *zipPath,const char *dirpath,const char *passwd);
        void asyncUnzip(const std::string& zipPath,const std::string& dirpath,const std::string& passwd, FileZipDelegate* delegate);
        
    };
NS_TZ_END

#endif /* FileZipUtil_h */
