#ifndef TARGET_ROOM_H
#define TARGET_ROOM_H

#include <QString>

namespace autobot {
class TargetRoom {
public:
  TargetRoom(const QString& room_number);
  const QString& GetRoomNumber() const;
private:
  QString room_number_;
};

} // namespace

#endif // TARGET_ROOM_H
