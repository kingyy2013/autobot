#ifndef BOT_ACCOUNT_H
#define BOT_ACCOUNT_H

#include <QString>
#include <QStringList>
#include <QSet>

#include <memory>
#include "task_interface.h"
#include "autobot_unit.h"

namespace autobot {
enum class AccountStatus {
  UNINITIALIZED = 0,
  IDLE,
  WORKING,
  ERROR,
};

// The container for the bot accounts.
class AutobotAccount : public AutobotUnit {
public:
  AutobotAccount() = default;
  AutobotAccount(const QString& username, const QString& password,
                 const QString& nickname = "");
  // Getters.
  const QString& GetUsername() const;
  const QString& GetNickname() const;
  const QString& GetPassword() const;
  const TaskConfig& GetTaskConfig() const;

  // Setters.
  void SetUsername(const QString&);
  void SetNickname(const QString&);
  void SetPassword(const QString&);
  void SetTaskConfig(const TaskConfig& task_config);

  // Target room as lower level connector.
  void AssignTargetRoom(const std::shared_ptr<AutobotUnit>&);
  void RemoveTargetRoom(const std::shared_ptr<AutobotUnit>&);
  const ConnectionUnitList& GetTargetRoomSet() const;

  // Get the status of the account.
  const AccountStatus& GetStatus() const;

  const QString GetTypeName() const ;
  static const QString GetTypeNameStatic();

private:
  QString bot_password_;
  QString bot_nickname_;

  AccountStatus bot_status_;
  TaskConfig task_config_;
}; // AutobotAccount

} // namespace

#endif // BOT_ACCOUNT_H
