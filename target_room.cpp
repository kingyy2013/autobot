#include "target_room.h"

namespace autobot {
TargetRoom::TargetRoom(const QString& room_number) : AutobotUnit (room_number) {
}

void TargetRoom::SetRoomNumber(const QString& room_number) {
  this->SetUnitName(room_number);
}

const QString& TargetRoom::GetRoomNumber() const {
  return this->GetUnitName();
}

void TargetRoom::AssignTargetSpeech(
    const std::shared_ptr<AutobotUnit>& target_speech) {
  this->AddLowerConnection(target_speech);
}

void TargetRoom::RemoveTargetSpeech(
    const std::shared_ptr<AutobotUnit>& target_speech) {
  this->RemoveLowerConnection(target_speech);
}

const ConnectionUnitList& TargetRoom::GetSpeechNameSet() const {
  return this->GetAllLowerConnections();
}

const ConnectionUnitList& TargetRoom::GetAssignedAccountSet() const{
  return this->GetAllUpperConnections();
}

const QString TargetRoom::GetTypeName() const {
  return GetTypeNameStatic();
}

const QString TargetRoom::GetTypeNameStatic() {
  return "target_room";
}

} //namespace
