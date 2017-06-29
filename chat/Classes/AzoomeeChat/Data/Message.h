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

class Sticker;
typedef std::shared_ptr<Sticker> StickerRef;

/**
 * A Message is a contact in the chat list.
 */
class Message : public JsonObjectRepresentation
{
private:
    
    std::string _messageId;
    std::string _messageType;
    std::string _messageText;
    std::string _stickerLocation;
    std::string _senderId;
    std::string _recipientId;
    uint64_t _timestamp = 0;
    bool _moderated = false;
    
    // no direct construction
    Message();
    
public:
    
    static const char* const MessageTypeText;
    static const char* const MessageTypeSticker;
    
    /// Create a Message from JSON
    static MessageRef createFromJson(const rapidjson::Value& json);
    /// Create a Text Message
    static MessageRef createTextMessage(const std::string& text);
    /// Create a Sticker Message
    static MessageRef createStickerMessage(const StickerRef& sticker);
    
    const std::string& messageId() const;
    const std::string& messageType() const;
    const std::string& messageText() const;
    const std::string& stickerURL() const;
    const std::string& senderId() const;
    const std::string& senderName() const;
    const std::string& recipientId() const;
    const std::string& recipientName() const;
    uint64_t timestamp() const;
    bool moderated() const;
    
    // - JsonObjectRepresentation
    rapidjson::Value toJson() const override;
};

NS_AZOOMEE_CHAT_END

#endif
