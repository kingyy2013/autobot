#include "autobot_helper.h"

#include <QHash>
#include <QDateTime>

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


QDomElement ConvertTargetRoomToXML(const TargetRoom& target_room,
                                   QDomDocument* parent_doc) {
  QDomElement dom_ele = parent_doc->createElement("TargetRoom");
  dom_ele.setAttribute("Room", target_room.GetRoomNumber());
  return dom_ele;
}

QDomElement ConvertTargetAccountToXML(const AutobotAccount& autobot_account,
                                       QDomDocument* parent_doc) {
  QDomElement dom_account = parent_doc->createElement("AutoBot");
  dom_account.setAttribute("Userame", autobot_account.GetUsername());
  dom_account.setAttribute("Password", autobot_account.GetPassword());
  QDomElement dom_targets = parent_doc->createElement("TagetRooms");
  for (const auto& target_room : autobot_account.GetTargetRoomMap()) {
    QDomElement target_dom = ConvertTargetRoomToXML(*target_room,
                                                   parent_doc);
    dom_targets.appendChild(target_dom);
  }
  dom_account.appendChild(dom_targets);
  return dom_account;
}

QDomElement ConvertAutobotManagerToXML(const AutobotManager& account_manager,
                                       QDomDocument* parent_doc) {
  QDomElement dom_account = parent_doc->createElement("Autobot Accounts");
  dom_account.setAttribute("DateTime",
                           QDateTime::currentDateTime()
                           .toString("yyyy.MM.dd:hh:mm:ss"));
  for (const auto& autobot_account : account_manager.GetAccountDict()) {
    QDomElement target_dom = ConvertTargetAccountToXML(*autobot_account,
                                                   parent_doc);
    dom_account.appendChild(target_dom);
  }
  return dom_account;
}

}
