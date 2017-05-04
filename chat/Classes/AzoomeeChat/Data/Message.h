#ifndef AzoomeeChat_Message_h
#define AzoomeeChat_Message_h

#include "../AzoomeeChat.h"
#include <external/json/document.h>
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
class Message
{
private:
    
    std::string _messageId;
    std::string _messageType;
    std::string _messageText;
    std::string _senderId;
    std::string _recipientId;
    uint64_t _timestamp;
    
    // no direct construction
    Message();
    
public:
    
    static MessageRef createFromJson(const rapidjson::Value& json);
    
    std::string messageId() const;
    std::string messageType() const;
    std::string messageText() const;
    std::string senderId() const;
    std::string recipientId() const;
    uint64_t timestamp() const;
};

NS_AZOOMEE_CHAT_END

#endif
