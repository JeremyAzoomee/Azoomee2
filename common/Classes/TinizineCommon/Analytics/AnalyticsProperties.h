//
//  AnalyticsProperties.h
//  TinizineCommon
//
//  Created by Macauley on 04/01/2018.
//

#ifndef AnalyticsProperties_h
#define AnalyticsProperties_h

#include <cocos/cocos2d.h>
#include "../Tinizine.h"
#include <time.h>

NS_TZ_BEGIN

class AnalyticsProperties : public cocos2d::Ref
{
private:
    std::map<std::string, std::string> _storedContentItemProperties;
    std::map<std::string, std::string> _storedGeneralProperties;
    
    std::string _ctaWindowNodeId = "";
    std::string _ctaWindowGroupId = "";
    std::string _ctaSourceButton = "";
    std::string _ctaSourceContentId = "";
    std::string _ctaSourceMediaType = "";
    std::string _ctaRecommendedContentId = "";
    
    std::string _OSManufacturer;
    
    time_t _timeOpenedContent;
    
public:
    static AnalyticsProperties* getInstance(void);
    virtual ~AnalyticsProperties();
    bool init(void);
    
    void setStoredContentItemProperties(const std::map<std::string, std::string>& properties);
    void addPropertyToStoredContentItemProperties(const std::string& key, const std::string& value);
    std::map<std::string, std::string> getStoredContentItemProperties() const;
    
    void addGeneralProperty(const std::string& key, const std::string& value);
    std::map<std::string, std::string> getStoredGeneralProperties() const;
    
    void updateEpochTime();
    
    void setCtaWindowNodeId(const std::string& nodeId);
    std::string getCtaWindowNodeId() const;
    
    void setCtaWindowGroupId(const std::string& groupId);
    std::string getCtaWindowGroupId() const;
    
    void setCtaSourceButton(const std::string& buttonId);
    std::string getCtaSourceButton() const;
    
    void setCtaSourceContentId(const std::string& contentId);
    std::string getCtaSourceContentId() const;
    
    void setCtaSourceMediaType(const std::string& mediaType);
    std::string getCtaSourceMediaType() const;
    
    void setCtaRecommendedContentId(const std::string& recommendedContentId);
    std::string getCtaRecommendedContentId() const;
    
    void setOSManufacturer(const std::string& osManufacturer);
    std::string getOSManufacturer() const;
    
    void setTimeOpenedContent(time_t timeOpenedContent);
    time_t getTimeOpenedContent() const;
	
	static const std::string kSecondsInContentKey;
};

NS_TZ_END

#endif /* AnalyticsProperties_h */
