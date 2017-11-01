//
//  PatternFileStorage.h
//  azoomee2_ArtApp
//
//  Created by Macauley on 19/10/2017.
//
//

#ifndef PatternFileStorage_h
#define PatternFileStorage_h

#include "AzoomeeArtApp.h"
#include <map>
#include <memory>

NS_AZOOMEE_AA_BEGIN

class PatternFileStorage
{
private:
    std::map<std::string,std::pair<std::string,std::string>> _fileStore;
    
    void init();
    
public:
    
    static PatternFileStorage* getInstance();
    
    std::string getTransparantFileByName(const std::string& name) const;
    std::string getNormalFileByName(const std::string& name) const;
};

NS_AZOOMEE_AA_END
#endif /* PatternFileStorage_h */
