#include "Message.h"
#include <cocos/cocos2d.h>
#include "../ChatAPI.h"
#include "Sticker.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Net/Utils.h>

using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN

const char* const Message::MessageTypeText = "TEXT";
const char* const Message::MessageTypeSticker = "STICKER";
const char* const Message::MessageTypeArt = "ART";
const char* const Message::MessageTypeContent = "CONTENT";

/// A Message object must have at least these fields to be valid
const std::vector<std::string> kRequiredFields = { "id", "type", "status", "senderId", "recipientId"/*, "timestamp" */};

const std::map<std::string, std::string> Message::kSentMessageDescription = {
    {MessageTypeArt, "Sent a Picture"},
    {MessageTypeContent, "Sent a Link"},
    {MessageTypeSticker, "Sent a Sticker"}
};

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
    
    
    // Check for required fields
    for(const std::string& fieldName : kRequiredFields)
    {
        if(!json.HasMember(fieldName.c_str()))
        {
            // Invalid JSON
            return MessageRef();
        }
    }
    
    const std::string& messageId = json["id"].GetString();
    const std::string& messageType = json["type"].GetString();
    const std::string& senderId = json["senderId"].GetString();
    const std::string& recipientId = json["recipientId"].GetString();
    uint64_t timestamp = 0;
    if(json.HasMember("timestamp"))
    {
        timestamp = json["timestamp"].GetUint64();
    }
    const std::string& status = json["status"].GetString();
    bool moderated = (status == "MODERATED");
    std::string messageText;
    std::string stickerLocation;
    std::string artLocation;
    std::string contentId;
    
    // Active message
    if(status == "ACTIVE")
    {
        // Check message type for other information
        if(messageType == Message::MessageTypeText)
        {
            if(!json.HasMember("params"))
            {
                // Invalid JSON
                return MessageRef();
            }
            
            const auto& params = json["params"];
            
            if(!params.HasMember("text"))
            {
                // Invalid JSON
                return MessageRef();
            }
            messageText = params["text"].GetString();
            
            // Decode the string and trim whitespace
            messageText = Azoomee::trim(Azoomee::Net::urlDecode(messageText));
        }
        else if(messageType == Message::MessageTypeSticker)
        {
            if(!json.HasMember("params"))
            {
                // Invalid JSON
                return MessageRef();
            }
            
            const auto& params = json["params"];
            
            if(!params.HasMember("sticker_location"))
            {
                // Invalid JSON
                return MessageRef();
            }
            
            stickerLocation = Azoomee::trim(params["sticker_location"].GetString());
        }
        else if(messageType == Message::MessageTypeArt)
        {
            if(!json.HasMember("location"))
            {
                return MessageRef();
            }
            artLocation = json["location"].GetString();
            
        }
        else if(messageType == Message::MessageTypeContent)
        {
            if(!json.HasMember("params"))
            {
                // Invalid JSON
                return MessageRef();
            }
            
            const auto& params = json["params"];
            
            if(!params.HasMember("contentId"))
            {
                // Invalid JSON
                return MessageRef();
            }
            
            contentId = params["contentId"].GetString();
            
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
        // Ignore this message as it's a status we don't recognise
        return MessageRef();
    }
    
    MessageRef messageData(new Message());
    messageData->_messageId = messageId;
    messageData->_messageType = messageType;
    messageData->_senderId = senderId;
    messageData->_recipientId = recipientId;
    messageData->_timestamp = timestamp;
    messageData->_messageText = messageText;
    messageData->_stickerLocation = stickerLocation;
    messageData->_artLocation = artLocation;
    messageData->_contentId = contentId;
    messageData->_moderated = moderated;
    return messageData;
}

MessageRef Message::createTextMessage(const std::string& text)
{
    MessageRef messageData(new Message());
    messageData->_messageType = Message::MessageTypeText;
    messageData->_messageText = text;
    return messageData;
}

MessageRef Message::createStickerMessage(const StickerRef& sticker)
{
    MessageRef messageData(new Message());
    messageData->_messageType = Message::MessageTypeSticker;
    messageData->_stickerLocation = sticker->imageURL();
    return messageData;
}

MessageRef Message::createArtMessage(const std::string &artData)
{
    MessageRef messageData(new Message());
    messageData->_messageType = Message::MessageTypeArt;
    messageData->_artData = artData;
    return messageData;
}

MessageRef Message::createContentMessage(const std::string& contentId)
{
    MessageRef messageData(new Message());
    messageData->_messageType = Message::MessageTypeContent;
    messageData->_contentId = contentId;
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

std::string Message::stickerURL() const
{
    return _stickerLocation;
}

std::string Message::artURL() const
{
    return _artLocation;
}

std::string Message::contentId() const
{
    return _contentId;
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
    if(_messageType == Message::MessageTypeText)
    {
        std::map<std::string, std::string> params = {
            { "text", Azoomee::Net::urlEncode(_messageText) }
        };
        json.AddMember("params", ToJson(params), allocator);
    }
    else if(_messageType == Message::MessageTypeSticker)
    {
        std::map<std::string, std::string> params = {
            { "sticker_location", _stickerLocation }
        };
        json.AddMember("params", ToJson(params), allocator);
    }
    else if(_messageType == Message::MessageTypeArt)
    {
        json.AddMember("data",ToJson(_artData),allocator);
    }
    else if(_messageType == Message::MessageTypeContent)
    {
        std::map<std::string, std::string> params = {
            { "contentId", _contentId }
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
