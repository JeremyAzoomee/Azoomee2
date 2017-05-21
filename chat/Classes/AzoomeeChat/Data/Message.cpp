#include "Message.h"
#include <cocos/cocos2d.h>
#include "../ChatAPI.h"

using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN

// TODO: Move these methods into AzoomeeCommon
unsigned char to_hex(unsigned char x)
{
    return x + (x > 9 ? ('A'-10) : '0');
}

const std::string urlencode(const std::string& s)
{
    std::ostringstream os;
    
    for(std::string::const_iterator ci = s.begin(); ci != s.end(); ++ci)
    {
        if((*ci >= 'a' && *ci <= 'z') ||
           (*ci >= 'A' && *ci <= 'Z') ||
           (*ci >= '0' && *ci <= '9'))
        { // allowed
            os << *ci;
        }
        else if( *ci == ' ')
        {
            os << '+';
        }
        else
        {
            os << '%' << to_hex(*ci >> 4) << to_hex(*ci % 16);
        }
    }
    
    return os.str();
}

unsigned char from_hex(unsigned char ch)
{
    if(ch <= '9' && ch >= '0')
        ch -= '0';
    else if(ch <= 'f' && ch >= 'a')
        ch -= 'a' - 10;
    else if(ch <= 'F' && ch >= 'A')
        ch -= 'A' - 10;
    else
        ch = 0;
    return ch;
}

const std::string urldecode(const std::string& str)
{
    using namespace std;
    string result;
    string::size_type i;
    for(i = 0; i < str.size(); ++i)
    {
        if(str[i] == '+')
        {
            result += ' ';
        }
        else if(str[i] == '%' && str.size() > i+2)
        {
            const unsigned char ch1 = from_hex(str[i+1]);
            const unsigned char ch2 = from_hex(str[i+2]);
            const unsigned char ch = (ch1 << 4) | ch2;
            result += ch;
            i += 2;
        }
        else
        {
            result += str[i];
        }
    }
    return result;
}

MessageRef Message::createFromJson(const rapidjson::Value& json)
{
    // Parse the JSON
    if(!json.IsObject())
    {
        // Invalid JSON
        return MessageRef();
    }
    
    // Example response:
    //{
    //    "type":"TEXT",
    //    "status":"ACTIVE",
    //    "params":{
    //        "text":"Hello!"
    //    },
    //    "senderId":"9d2bb8a5-ab3c-41ce-bfca-bcf2c9e4a3cb",
    //    "recipientId":"20bc86d3-4d15-4e6a-9021-77abeb9a8798",
    //    "timestamp":1493395678099,
    //    "id":"21cb24ba-44b5-435e-b5f2-7d87a9c13ea3"
    //}
    
    const std::string& messageId = json["id"].GetString();
    const std::string& messageType = json["type"].GetString();
    const std::string& senderId = json["senderId"].GetString();
    const std::string& recipientId = json["recipientId"].GetString();
    uint64_t timestamp = json["timestamp"].GetUint64();
    const std::string& status = json["status"].GetString();
    bool moderated = (status == "MODERATED");
    std::string messageText;
    
    // Active message
    if(status == "ACTIVE")
    {
        // Check message type for other information
        if(messageType == "TEXT")
        {
            const auto& params = json["params"];
            messageText = params["text"].GetString();
            
            // Decode the string
            messageText = urldecode(messageText);
        }
        else
        {
            messageText = StringUtils::format("<unsupported message type: %s>", messageType.c_str());
        }
    }
    else if(moderated)
    {
        // TODO: Remove this, it should be displayed by UI logic only
        messageText = "Message deleted";
    }
    else
    {
        // Ignore this message
        return MessageRef();
    }
    
    MessageRef messageData(new Message());
    messageData->_messageId = messageId;
    messageData->_messageType = messageType;
    messageData->_senderId = senderId;
    messageData->_recipientId = recipientId;
    messageData->_timestamp = timestamp;
    messageData->_messageText = messageText;
    messageData->_moderated = moderated;
    return messageData;
}

MessageRef Message::createTextMessage(const std::string& text)
{
    MessageRef messageData(new Message());
    messageData->_messageType = "TEXT";
    messageData->_messageText = text;
    return messageData;
}

Message::Message()
{
}

std::string Message::messageId() const
{
    return _messageId;
}

std::string Message::messageType() const
{
    return _messageType;
}

std::string Message::messageText() const
{
    return _messageText;
}

std::string Message::senderId() const
{
    return _senderId;
}

std::string Message::senderName() const
{
    return ChatAPI::getInstance()->getProfileName(_senderId);
}

std::string Message::recipientId() const
{
    return _recipientId;
}

std::string Message::recipientName() const
{
    return ChatAPI::getInstance()->getProfileName(_recipientId);
}

uint64_t Message::timestamp() const
{
    return _timestamp;
}

bool Message::moderated() const
{
    return _moderated;
}

#pragma mark - JsonObjectRepresentation

rapidjson::Value Message::toJson() const
{
    auto& allocator = JsonDefaultAllocator();
    
    // Create initial object with strings
    std::map<std::string, std::string> stringValues = {
        { "type", _messageType },
        { "id", _messageId },
        { "senderId", _senderId },
        { "recipientId", _recipientId }
    };
    rapidjson::Value json = ToJson(stringValues);
    
    // Add other values
    if(_messageType == "TEXT")
    {
        std::map<std::string, std::string> params = {
            { "text", _messageText }
        };
        json.AddMember("params", ToJson(params), allocator);
    }
    if(_timestamp > 0)
    {
        json.AddMember("timestamp", ToJson(_timestamp), allocator);
    }
    
    return json;
}

NS_AZOOMEE_CHAT_END
