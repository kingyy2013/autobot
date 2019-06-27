#include "autobot_account.h"

namespace autobot {

AutobotAccount::AutobotAccount(const QString& username,
                               const QString& password,
                               const QString& speech_name)
  : bot_username_(username),
    bot_password_(password),
    speech_name_(speech_name),
    bot_status_(AccountStatus::UNINITIALIZED) {
}


const QString& AutobotAccount::GetUsername() const {
  return bot_username_;
}

const QString& AutobotAccount::GetPassword() const {
  return bot_password_;
}

const AccountStatus& AutobotAccount::GetStatus() const {
  return bot_status_;
}

const TargtRoomMap& AutobotAccount::GetTargetRoomMap() const {
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

void AutobotAccount::SetSpeechName(const QString& speech_name) {
  speech_name_ = speech_name;
}

const QString&  AutobotAccount::GetSpeechName() const {
  return speech_name_;
}

void AutobotAccount::SetTaskConfig(const TaskConfig& task_config) {
  task_config_ = task_config;
}

const TaskConfig& AutobotAccount::GetTaskConfig() const {
  return task_config_;
}

}
