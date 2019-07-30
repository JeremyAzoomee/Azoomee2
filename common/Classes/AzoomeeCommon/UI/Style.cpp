//
//  Style.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 07/11/2018.
//

#include "Style.h"

NS_AZOOMEE_BEGIN

namespace Style
{

namespace Font
{

const char* const Regular()
{
	if(StringMgr::getInstance()->getLanguageID() == "gre")
	{
		return ArialRegular;
	}
	return SofiaRegular;
}
const char* const Medium()
{
	if(StringMgr::getInstance()->getLanguageID() == "gre")
	{
		return ArialRegular;
	}
	return SofiaMedium;
}
const char* const Bold()
{
	if(StringMgr::getInstance()->getLanguageID() == "gre")
	{
		return ArialBold;
	}
	return SofiaBold;
}
const char* const Input()
{
	if(StringMgr::getInstance()->getLanguageID() == "gre")
	{
		return ArialRegular;
	}
	return SofiaRegular;
}

const char* const RewardRegular()
{
	if(StringMgr::getInstance()->getLanguageID() == "gre")
	{
		return ArialBold;
	}
	return PassionOneRegular;
}
	
const char* const PoppinsRegular()
{
	if(StringMgr::getInstance()->getLanguageID() == "gre")
	{
		return ArialBold;
	}
	return poppinsRegular;
}
const char* const PoppinsMedium()
{
	if(StringMgr::getInstance()->getLanguageID() == "gre")
	{
		return ArialBold;
	}
	return poppinsMedium;
}
const char* const PoppinsBold()
{
	if(StringMgr::getInstance()->getLanguageID() == "gre")
	{
		return ArialBold;
	}
	return poppinsBold;
}
}
}

NS_AZOOMEE_END
