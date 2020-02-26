//
//  ProductLayout.h
//  Azoomee
//
//  Created by Macauley on 09/07/2019.
//

#ifndef ProductLayout_h
#define ProductLayout_h

#include <AzoomeeCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include <AzoomeeCommon/UI/TextButton.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

enum class ProductLayoutType {PASS, SUBSCRIPTION, MULI_PASS};
enum class IAPAction {PURCHASE, RESTORE};

class ProductLayout : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
	typedef std::vector<std::pair<std::string, std::string>> ProductDataList; // [<desc,price>,<desc,price>,...]
	typedef std::function<void(IAPAction)> IAPActionCallback;
private:
	cocos2d::ui::Layout* _productBanner = nullptr;
	TextButton* _purchaseButton = nullptr;
	DynamicText* _restoreButton = nullptr;
	
	ProductLayoutType _type;
	
	ProductDataList _productData;
	
	IAPActionCallback _callback = nullptr;
	
	void setupProductBanner();
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void onSizeChanged() override;
	
	void setProductLayoutType(const ProductLayoutType& type);
	void setProductData(const ProductDataList& dataList);
	void setIapActionCallback(const IAPActionCallback& callback);
	
	CREATE_FUNC(ProductLayout);
};

NS_AZ_END

#endif /* ProductLayout_h */
