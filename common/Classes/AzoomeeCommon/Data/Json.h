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
#include <cocos2d.h>


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

inline std::string getStringFromJson(const std::string &keyName, const rapidjson::Document &jsonDocument, const std::string& defaultReturn = "")
{
    if(jsonDocument.HasParseError())
    {
        return defaultReturn;
    }
    
    if((jsonDocument.HasMember(keyName.c_str()))&&(!jsonDocument[keyName.c_str()].IsNull())&&(jsonDocument[keyName.c_str()].IsString()))
    {
        return jsonDocument[keyName.c_str()].GetString();
    }
    
    return defaultReturn;
}

inline std::string getStringFromJson(const std::string &keyName, const rapidjson::Value &jsonValue, const std::string& defaultReturn = "")
{
    if((jsonValue.HasMember(keyName.c_str()))&&(!jsonValue[keyName.c_str()].IsNull())&&(jsonValue[keyName.c_str()].IsString()))
    {
        return jsonValue[keyName.c_str()].GetString();
    }
                
    return defaultReturn;
}

inline bool getBoolFromJson(const std::string &keyName, const rapidjson::Document &jsonDocument, bool defaultReturn = false)
{
    if(jsonDocument.HasParseError())
    {
        return defaultReturn;
    }
    
    if((jsonDocument.HasMember(keyName.c_str()))&&(!jsonDocument[keyName.c_str()].IsNull())&&(jsonDocument[keyName.c_str()].IsBool()))
    {
        return jsonDocument[keyName.c_str()].GetBool();
    }
    
    return defaultReturn;
}

inline bool getBoolFromJson(const std::string &keyName, const rapidjson::Value &jsonValue, bool defaultReturn = false)
{
    if((jsonValue.HasMember(keyName.c_str()))&&(!jsonValue[keyName.c_str()].IsNull())&&(jsonValue[keyName.c_str()].IsBool()))
    {
        return jsonValue[keyName.c_str()].GetBool();
    }
    
    return defaultReturn;
}

inline cocos2d::Vec2 getVec2FromJson(const std::string &keyName, const rapidjson::Value &jsonValue, const cocos2d::Vec2& defaultReturn = cocos2d::Vec2(0,0))
{
    if(jsonValue.HasMember(keyName.c_str()) && jsonValue[keyName.c_str()].Size() == 2 && jsonValue[keyName.c_str()][0].IsFloat() && jsonValue[keyName.c_str()][1].IsFloat())
    {
        return cocos2d::Vec2(jsonValue[keyName.c_str()][0].GetFloat(), jsonValue[keyName.c_str()][1].GetFloat());
    }
    return defaultReturn;
}

inline int getIntFromJson(const std::string &keyName, const rapidjson::Document &jsonDocument, int defaultReturn = INT_MAX)
{
    if(jsonDocument.HasParseError())
    {
        return defaultReturn;
    }
    
    if(jsonDocument.HasMember(keyName.c_str()) && jsonDocument[keyName.c_str()].IsInt())
    {
        return jsonDocument[keyName.c_str()].GetInt();
    }
    return defaultReturn;
}

inline int getIntFromJson(const std::string &keyName, const rapidjson::Value &jsonValue, int defaultReturn = INT_MAX)
{
    if(jsonValue.HasMember(keyName.c_str()) && jsonValue[keyName.c_str()].IsInt())
    {
        return jsonValue[keyName.c_str()].GetInt();
    }
    return defaultReturn;
}

inline cocos2d::Color4B getColor4BFromJson(const std::string &keyName, const rapidjson::Value &jsonValue, const cocos2d::Color4B& defaultReturn = cocos2d::Color4B())
{
    if(jsonValue.HasMember(keyName.c_str()) && jsonValue[keyName.c_str()].Size() == 4 && jsonValue[keyName.c_str()][0].IsInt() && jsonValue[keyName.c_str()][1].IsInt() && jsonValue[keyName.c_str()][2].IsInt() && jsonValue[keyName.c_str()][3].IsInt())
    {
        return cocos2d::Color4B(jsonValue[keyName.c_str()][0].GetInt(), jsonValue[keyName.c_str()][1].GetInt(), jsonValue[keyName.c_str()][2].GetInt(), jsonValue[keyName.c_str()][3].GetInt());
    }
    return defaultReturn;
}

inline std::vector<std::string> getStringArrayFromJson(const rapidjson::Value& jsonValue)
{
    std::vector<std::string> returnValue;
    
    if(jsonValue.IsNull() || !jsonValue.IsArray())
    {
        return returnValue;
    }
    
    for(rapidjson::SizeType i = 0; i < jsonValue.Size(); i++)
    {
        if(!jsonValue[i].IsNull() && jsonValue[i].IsString())
        {
            returnValue.push_back(jsonValue[i].GetString());
        }
    }
    
    return returnValue;
}

inline std::map<std::string, std::string> getStringMapFromJson(const rapidjson::Value& jsonValue)
{
    std::map<std::string, std::string> returnValue;
 
    if(jsonValue.IsNull())
    {
        return returnValue;
    }
    
    rapidjson::Value::ConstMemberIterator M;
    
    for (M=jsonValue.MemberBegin(); M!=jsonValue.MemberEnd(); M++)
    {
        if(!M->name.IsNull() && M->name.IsString() && !M->value.IsNull() && M->value.IsString())
        {
            std::string name = M->name.GetString();
            std::string value = M->value.GetString();
            
            returnValue[name] = value;
        }
    }
    
    return returnValue;
}

inline std::map<std::string, std::string> getStringMapFromJsonString(const std::string& jsonString)
{
    std::map<std::string, std::string> returnValue;
    
    rapidjson::Document jsonDocument;
    jsonDocument.Parse(jsonString.c_str());
    
    if(jsonDocument.HasParseError())
    {
        return returnValue;
    }
    
    rapidjson::Document::MemberIterator M;
    
    for (M=jsonDocument.MemberBegin(); M!=jsonDocument.MemberEnd(); M++)
    {
        if(!M->name.IsNull() && M->name.IsString() && !M->value.IsNull() && M->value.IsString())
        {
            std::string key = M->name.GetString();
            std::string value = M->value.GetString();
            
            returnValue[key] = value;
        }
    }
    
    return returnValue;
}


NS_AZOOMEE_END

#endif
