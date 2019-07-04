#ifndef TARGET_ROOM_H
#define TARGET_ROOM_H

#include <QSet>
#include <QString>
#include "autobot_unit.h"

namespace autobot {
class TargetRoom : public AutobotUnit {
public:
  TargetRoom() = default;
  TargetRoom(const QString& room_number);
  void SetRoomNumber(const QString& room_number);
  const QString& GetRoomNumber() const;

  void AssignTargetSpeech(const std::shared_ptr<AutobotUnit>&);
  void RemoveTargetSpeech(const std::shared_ptr<AutobotUnit>&);
  const AutobotUnitDict& GetSpeechSet() const;
  const AutobotUnitDict& GetAssignedAccountSet() const;

  const QString GetTypeName() const ;
  static const QString GetTypeNameStatic();
private:
};

} // namespace

#endif // TARGET_ROOM_H
