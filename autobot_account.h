#ifndef BOT_ACCOUNT_H
#define BOT_ACCOUNT_H

#include <QString>
#include <QStringList>
#include <QHash>

#include <memory>
#include "target_room.h"
#include "target_speech_set.h"

namespace autobot {
enum class AccountStatus {
  UNINITIALIZED = 0,
  IDLE,
  WORKING,
  ERROR,
};

typedef QHash<QString, std::shared_ptr<TargetRoom>> TargtRoomMap;

// The container for the bot accounts.
class AutobotAccount {
public:
  AutobotAccount() = default;
  AutobotAccount(const QString& username, const QString& password,
                 const QString& speech_name = "");
  // Work related.
  bool Login();
  bool Speak();
  bool Logoff();

  // Read related.
  const QString& GetUsername() const;
  const QString& GetPassword() const;
  const QString& GetSpeechName() const;
  void SetSpeechName(const QString& speech_name);

  const TargtRoomMap& GetTargetRoomMap() const;

  // Write related.
  void SetUsername(const QString&);
  void SetPassword(const QString&);
  void AssignTargetRoom(const std::shared_ptr<TargetRoom>& target_room);
  void RemoveTargetRoom(const QString& target_room);

  const AccountStatus& GetStatus() const;
private:
  QString bot_username_;
  QString bot_password_;
  QString speech_name_;
  QHash<QString, std::shared_ptr<TargetRoom>> assigned_target_rooms_;
  AccountStatus bot_status_;
}; // AutobotAccount

} // namespace

#endif // BOT_ACCOUNT_H
