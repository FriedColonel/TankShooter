#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <nlohmann/json.hpp>

using namespace nlohmann;

class Message {
 public:
  enum class MessageType {
    control,
    createRoom,
    joinRoom,
    leaveRoom,
  };

 private:
  MessageType type;
  int userId;

 public:
  Message(MessageType type);
  ~Message();

  MessageType getType();

  static void to_json(json& j, const Message& value);
};

#endif
