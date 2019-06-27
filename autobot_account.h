#ifndef BOT_ACCOUNT_H
#define BOT_ACCOUNT_H

#include <QString>
#include <QStringList>
#include <QHash>

#include <memory>
#include "target_room.h"
#include "target_speech_set.h"
#include "task_interface.h"

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
                 const QString& nickname = "",
                 const QString& speech_name = kDefaultSpeechName);
  // Work related.
  bool Login();
  bool Speak();
  bool Logoff();

  // Read related.
  const QString& GetUsername() const;
  const QString& GetNickname() const;
  const QString& GetPassword() const;
  const QString& GetSpeechName() const;
  void SetSpeechName(const QString& speech_name);

  void SetTaskConfig(const TaskConfig& task_config);
  const TaskConfig& GetTaskConfig() const;

  const TargtRoomMap& GetTargetRoomMap() const;

  // Write related.
  void SetUsername(const QString&);
  void SetNickname(const QString&);
  void SetPassword(const QString&);
  void AssignTargetRoom(const std::shared_ptr<TargetRoom>& target_room);
  void RemoveTargetRoom(const QString& target_room);

  const AccountStatus& GetStatus() const;
private:
  QString bot_username_;
  QString bot_password_;
  QString bot_nickname_;
  QString speech_name_;
  QHash<QString, std::shared_ptr<TargetRoom>> assigned_target_rooms_;
  AccountStatus bot_status_;
  TaskConfig task_config_;
}; // AutobotAccount

} // namespace

#endif // BOT_ACCOUNT_H
