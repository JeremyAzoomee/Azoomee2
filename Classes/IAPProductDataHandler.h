#ifndef IAPProductDataHandler_h
#define IAPProductDataHandler_h

#include "cocos2d.h"
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN
    
    class IAPProductDataHandler : public cocos2d::Ref
    {
    private:
        IAPProductDataHandler();
        
        std::string _productPriceHumanReadable = "";
        int _introductoryMonths = 0;
        float _introductoryPrice = 0.0f;
        float _productPrice = 0.0f;
        int _trialPeriodDays = 0;
        std::string _priceCurrency = "";
        
    public:
        static const std::string& kDefaultPrice;
        
        static IAPProductDataHandler* getInstance(void);
        virtual ~IAPProductDataHandler();
        
        void fetchProductData();
        
        void setHumanReadableProductPrice(const std::string &prductPriceHumanReadable);
        void setIntroductoryMonths(int introductoryMonths);
        void setIntroductoryPrice(float introductoryPrice);
        void setProductPrice(float productPrice);
        void setPriceCurrency(const std::string &currency);
        void setTrialPeriodDays(int trialPeriodDays);
        
        std::string getHumanReadableProductPrice() const;
        int getIntroductoryMonths();
        float getIntroductoryPrice();
        float getProductPrice();
        std::string getPriceCurrency() const;
        int getTrialPeriodDays();
        
    };

NS_AZOOMEE_END

#endif
