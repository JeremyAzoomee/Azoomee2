#ifndef AzoomeeChat_Message_h
#define AzoomeeChat_Message_h

#include "../AzoomeeChat.h"
#include <AzoomeeCommon/Data/Json.h>
#include <string>
#include <memory>


USING_NS_TZ

NS_AZ_CHAT_BEGIN

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
    std::string _artLocation;
    std::string _artData;
    std::string _contentId;
    std::string _senderId;
    std::string _recipientId;
    uint64_t _timestamp = 0;
    bool _moderated = false;
    
    // no direct construction
    Message();
    
public:
    
    static const char* const MessageTypeText;
    static const char* const MessageTypeSticker;
    static const char* const MessageTypeArt;
    static const char* const MessageTypeContent;
    
    static const std::map<std::string, std::string> kSentMessageDescription;
    
    /// Create a Message from JSON
    static MessageRef createFromJson(const rapidjson::Value& json);
    /// Create a Text Message
    static MessageRef createTextMessage(const std::string& text);
    /// Create a Sticker Message
    static MessageRef createStickerMessage(const StickerRef& sticker);
    /// Create an Art Message
    static MessageRef createArtMessage(const std::string& artData);
    /// Create Content Message
    static MessageRef createContentMessage(const std::string& contentId);
    
    std::string messageId() const;
    std::string messageType() const;
    std::string messageText() const;
    std::string stickerURL() const;
    std::string artURL() const;
    std::string contentId() const;
    std::string senderId() const;
    std::string senderName() const;
    std::string recipientId() const;
    std::string recipientName() const;
    uint64_t timestamp() const;
    bool moderated() const;
    
    // - JsonObjectRepresentation
    rapidjson::Value toJson() const override;
};

NS_AZ_CHAT_END

#endif
