#include "autobot_helper.h"

#include <QHash>
namespace autobot {
const QHash<uint, QPair<QString, QColor>> kStatusToQString = {
  {static_cast<uint>(AccountStatus::UNINITIALIZED), {"未登录", QColor(0, 255, 255)}},
  {static_cast<uint>(AccountStatus::IDLE), {"空闲中", QColor(0, 255, 0)}},
  {static_cast<uint>(AccountStatus::WORKING), {"工作中", QColor(0, 0, 255)}},
  {static_cast<uint>(AccountStatus::ERROR), {"错误", QColor(255, 0, 0)}}
};


QPair<QString, QColor> ConvertStatusToQStringAndColor(AccountStatus status) {
  return kStatusToQString[static_cast<uint>(status)];
}

}
