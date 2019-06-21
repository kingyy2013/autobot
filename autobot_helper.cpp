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
  dom_account.setAttribute("Username", autobot_account.GetUsername());
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
  QDomElement dom_account = parent_doc->createElement("AutobotAccounts");
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

bool ParseXMLToTargetRoom(const QDomElement& dom_element,
                          TargetRoom* target_room) {
  if (!dom_element.hasAttribute("Room")) {
    qDebug() << "Failed to parse Room num";
    return false;
  }
  target_room->SetRoomNumber(dom_element.attribute("Room"));
  return true;
}

bool ParseXMLToTargetAccount(const QDomElement& dom_element,
                             AutobotAccount* autobot_account) {
  if (!dom_element.hasAttribute("Username") ||
      !dom_element.hasAttribute("Password")) {
    qDebug() << "Failed to parse Username and Password";
    return false;
  }
  const QString& username = dom_element.attribute("Username");
  const QString& password = dom_element.attribute("Password");
  autobot_account->SetUsername(username);
  autobot_account->SetPassword(password);
  bool success = true;
  QDomNodeList child_dorms = dom_element.childNodes();
  for (int i = 0; i < child_dorms.size(); ++i) {
    QDomElement child_ele = child_dorms.at(i).toElement();
    if (child_ele.tagName() == "TagetRooms") {
      QDomNodeList target_dorms = child_ele.childNodes();
      for (int j = 0; j < target_dorms.size(); ++j) {
        auto target_room_ptr = std::make_shared<TargetRoom>();

        success &= ParseXMLToTargetRoom(target_dorms.at(j).toElement(),
                                        target_room_ptr.get());
        autobot_account->AssignTargetRoom(target_room_ptr);
      }
    }
  }
  return success;
}

bool ParseXMLToAutobotManager(const QDomElement& dom_element,
                              AutobotManager* account_manager) {
  QDomNodeList child_dorms = dom_element.childNodes();
  bool success = true;
  for (int i = 0; i < child_dorms.size(); ++i) {
    QDomElement child_ele = child_dorms.at(i).toElement();
    if (child_ele.tagName() == "AutoBot") {
      auto autobot_ptr = std::make_shared<AutobotAccount>();
      success &= ParseXMLToTargetAccount(child_ele,
                                         autobot_ptr.get());
      account_manager->AddAccount(autobot_ptr);
    }
  }
  return success;
}


}
