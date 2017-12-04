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

namespace Azoomee
{

    class FileZipDelegate
    {
    public:
        virtual void onAsyncUnzipComplete(bool success, std::string zipPath, std::string dirpath) = 0;
    };
    
    class FileZipUtil : public cocos2d::Ref
    {
    private:
        void unzipWithDelageteCallback(std::string zipPath,std::string dirpath,std::string passwd, FileZipDelegate* delegate);
    public:
        static FileZipUtil* getInstance(void);
        virtual ~FileZipUtil();
    
        bool unzip(const char *zipPath,const char *dirpath,const char *passwd);
        void asyncUnzip(std::string zipPath,std::string dirpath,std::string passwd, FileZipDelegate* delegate);
        
    };
}

#endif /* FileZipUtil_h */
