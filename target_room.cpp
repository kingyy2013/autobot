#include "target_room.h"

namespace autobot {
TargetRoom::TargetRoom(const QString& room_number) : room_number_(room_number) {

}

void TargetRoom::SetRoomNumber(const QString& room_number) {
  room_number_ = room_number;
}

const QString& TargetRoom::GetRoomNumber() const {
  return room_number_;
}

} //namespace