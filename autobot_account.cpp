#include "autobot_account.h"

namespace autobot {

AutobotAccount::AutobotAccount(const QString& username,
                               const QString& password)
  : bot_username_(username),
    bot_password_(password),
    bot_status_(AccountStatus::UNINITIALIZED) {
}


QString AutobotAccount::GetUsername() {
  return bot_username_;
}

QString AutobotAccount::GetPassword() {
  return bot_password_;
}

AccountStatus AutobotAccount::GetStatus() {
  return bot_status_;
}

const TargtRoomMap& AutobotAccount::GetTargetRoomMap() {
  return assigned_target_rooms_;
}

void AutobotAccount::SetUsername(const QString& username) {
  bot_username_ = username;
}

void AutobotAccount::SetPassword(const QString& password) {
  bot_password_ = password;
}

void AutobotAccount::AssignTargetRoom(
    const std::shared_ptr<TargetRoom>& target_room) {
  assigned_target_rooms_[target_room->GetRoomNumber()] = target_room;
}

void AutobotAccount::RemoveTargetRoom(const QString& room_number) {
  assigned_target_rooms_.remove(room_number);
}

}
