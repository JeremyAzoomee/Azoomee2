#ifndef AzoomeeCommon_Json_h
#define AzoomeeCommon_Json_h

#include "../Azoomee.h"

// Include some common use headers for using rapidjson library
#include <external/json/document.h>
#include <external/json/stringbuffer.h>
#include <external/json/writer.h>
#include <external/json/prettywriter.h>
#include <string>
#include <map>
#include <vector>


NS_AZOOMEE_BEGIN

/**
 * Defines a class that can be exported to Json
 */
struct JsonObjectRepresentation
{
    virtual rapidjson::Value toJson() const = 0;
};


// - Some convenience functions for converting values to Json using rapidjson

inline rapidjson::Value::AllocatorType& JsonDefaultAllocator()
{
    static rapidjson::Document sDefaultDoc;
    return sDefaultDoc.GetAllocator();
}

inline rapidjson::Value ToJson(const std::string& value)
{
    rapidjson::Value valueObj(value.c_str(), (rapidjson::SizeType)value.length(), JsonDefaultAllocator());
    return valueObj;
}

inline rapidjson::Value ToJson(int64_t value)
{
    rapidjson::Value valueObj(value);
    return valueObj;
}

inline rapidjson::Value ToJson(uint64_t value)
{
    rapidjson::Value valueObj(value);
    return valueObj;
}

inline rapidjson::Value ToJson(int value)
{
    rapidjson::Value valueObj(value);
    return valueObj;
}

inline rapidjson::Value ToJson(unsigned int value)
{
    rapidjson::Value valueObj(value);
    return valueObj;
}

inline rapidjson::Value ToJson(bool value)
{
    rapidjson::Value valueObj(value);
    return valueObj;
}

inline rapidjson::Value ToJson(double value)
{
    rapidjson::Value valueObj(value);
    return valueObj;
}

inline rapidjson::Value ToJson(float value)
{
    rapidjson::Value valueObj(value);
    return valueObj;
}

template<typename value_type>
inline rapidjson::Value ToJson(const std::map<std::string, value_type>& value)
{
    static rapidjson::Document sDefaultDoc;
    rapidjson::Value valueObj;
    valueObj.SetObject();
    for(typename std::map<std::string, value_type>::const_iterator it = value.begin(); it != value.end(); ++it)
    {
        valueObj.AddMember(ToJson(it->first), ToJson(it->second), JsonDefaultAllocator());
    }
    return valueObj;
}

template<typename value_type>
inline rapidjson::Value ToJson(const std::vector<value_type>& value)
{
    static rapidjson::Document sDefaultDoc;
    rapidjson::Value valueObj;
    valueObj.SetArray();
    for(auto entry : value)
    {
        valueObj.PushBack(ToJson(entry), JsonDefaultAllocator());
    }
    return valueObj;
}

inline std::string getStringFromJson(const std::string &keyName, const rapidjson::Document &jsonDocument)
{
    if(jsonDocument.HasParseError()) return "";
    
    if(jsonDocument.HasMember(keyName.c_str()))
        if(!jsonDocument[keyName.c_str()].IsNull())
            if(jsonDocument[keyName.c_str()].IsString())
                return jsonDocument[keyName.c_str()].GetString();
    
    return "";
}

inline std::string getStringFromJson(const std::string &keyName, const rapidjson::Value &jsonValue)
{
    if(jsonValue.HasMember(keyName.c_str()))
        if(!jsonValue[keyName.c_str()].IsNull())
            if(jsonValue[keyName.c_str()].IsString())
                return jsonValue[keyName.c_str()].GetString();
                
    return "";
}

inline bool getBoolFromJson(const std::string &keyName, const rapidjson::Document &jsonDocument)
{
    if(jsonDocument.HasParseError()) return "";
    
    if(jsonDocument.HasMember(keyName.c_str()))
        if(!jsonDocument[keyName.c_str()].IsNull())
            if(jsonDocument[keyName.c_str()].IsBool())
                return jsonDocument[keyName.c_str()].GetBool();
    
    return false;
}

inline bool getBoolFromJson(const std::string &keyName, const rapidjson::Value &jsonValue)
{
    if(jsonValue.HasMember(keyName.c_str()))
        if(!jsonValue[keyName.c_str()].IsNull())
            if(jsonValue[keyName.c_str()].IsBool())
                return jsonValue[keyName.c_str()].GetBool();
    
    return false;
}
NS_AZOOMEE_END

#endif
