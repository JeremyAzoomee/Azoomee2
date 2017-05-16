#ifndef AzoomeeChat_Message_h
#define AzoomeeChat_Message_h

#include "../AzoomeeChat.h"
#include <AzoomeeCommon/Data/Json.h>
#include <string>
#include <memory>


NS_AZOOMEE_CHAT_BEGIN

// forward decleration
class Message;
typedef std::shared_ptr<Message> MessageRef;
typedef std::vector<MessageRef> MessageList;

/**
 * A Message is a contact in the chat list.
 */
class Message : public JsonObjectRepresentation
{
private:
    
    std::string _messageId;
    std::string _messageType;
    std::string _messageText;
    std::string _senderId;
    std::string _recipientId;
    uint64_t _timestamp = 0;
    
    // no direct construction
    Message();
    
public:
    
    /// Create a Message from JSON
    static MessageRef createFromJson(const rapidjson::Value& json);
    /// Create a Text Message
    static MessageRef createTextMessage(const std::string& text);
    
    std::string messageId() const;
    std::string messageType() const;
    std::string messageText() const;
    std::string senderId() const;
    std::string recipientId() const;
    uint64_t timestamp() const;
    
    // - JsonObjectRepresentation
    rapidjson::Value toJson() const override;
};

NS_AZOOMEE_CHAT_END

#endif
