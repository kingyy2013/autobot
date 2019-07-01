#include "autobot_account.h"

namespace autobot {

AutobotAccount::AutobotAccount(const QString& username,
                               const QString& password,
                               const QString& nickname)
  : AutobotUnit (username),
    bot_password_(password),
    bot_nickname_(nickname),
    bot_status_(AccountStatus::UNINITIALIZED) {
}


const QString& AutobotAccount::GetUsername() const {
  return this->GetUnitName();
}

const QString& AutobotAccount::GetNickname() const {
  return bot_nickname_;
}

const QString& AutobotAccount::GetPassword() const {
  return bot_password_;
}

const AccountStatus& AutobotAccount::GetStatus() const {
  return bot_status_;
}


void AutobotAccount::SetUsername(const QString& username) {
  this->SetUnitName(username);
}

void AutobotAccount::SetNickname(const QString& nickname) {
  bot_nickname_ = nickname;
}

void AutobotAccount::SetPassword(const QString& password) {
  bot_password_ = password;
}

void AutobotAccount::AssignTargetRoom(
    const std::shared_ptr<AutobotUnit>& target_room) {
  this->AddLowerConnection(target_room);
}

void AutobotAccount::RemoveTargetRoom(
    const std::shared_ptr<AutobotUnit>& target_room) {
  this->RemoveLowerConnection(target_room);
}

const ConnectionUnitList& AutobotAccount::GetTargetRoomSet() const {
  return this->GetAllLowerConnections();
}

void AutobotAccount::SetTaskConfig(const TaskConfig& task_config) {
  task_config_ = task_config;
}

const TaskConfig& AutobotAccount::GetTaskConfig() const {
  return task_config_;
}

const QString AutobotAccount::GetTypeName() const {
  return GetTypeNameStatic();
}

const QString AutobotAccount::GetTypeNameStatic() {
  return "autobot_account";
}

}
