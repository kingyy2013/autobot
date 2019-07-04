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
  dom_ele.setAttribute("RoomNumber", target_room.GetRoomNumber());

  QDomElement dom_speech = parent_doc->createElement("AssignedName");
  dom_speech.setAttribute("SpeechNames",
                          target_room.GetSpeechSet().keys().join(" "));
  dom_ele.appendChild(dom_speech);

  QDomElement dom_accounts = parent_doc->createElement("AssignedAccount");
  dom_accounts.setAttribute(
        "AccountNames", target_room.GetAssignedAccountSet().keys().join(" "));
  dom_ele.appendChild(dom_accounts);

  return dom_ele;
}

QDomElement ConvertTargetSpeechToXML(const TargetSpeech& target_speech,
                                     QDomDocument* parent_doc) {
  QDomElement dom_speech = parent_doc->createElement("Speech");
  dom_speech.setAttribute("Speechname", target_speech.SpeechName());

  QDomElement target_dom = parent_doc->createElement("Word");
  target_dom.setAttribute("word", target_speech.GetWordsList().join(" "));
  dom_speech.appendChild(target_dom);

  QDomElement dom_room = parent_doc->createElement("AssignedRooms");
  dom_room.setAttribute(
        "RoomName", target_speech.GetAssignedRooms().keys().join(" "));
  dom_speech.appendChild(dom_room);

  return dom_speech;
}

QDomElement ConvertAutobotAccountToXML(
    const AutobotAccount& autobot_account,
    QDomDocument* parent_doc) {
  QDomElement dom_account = parent_doc->createElement("Account");
  dom_account.setAttribute("Username", autobot_account.GetUsername());
  dom_account.setAttribute("Password", autobot_account.GetPassword());
  dom_account.setAttribute("Nickname", autobot_account.GetNickname());
  dom_account.appendChild(ConvertTaskConfigToXML(
                            autobot_account.GetTaskConfig(),
                            parent_doc));
  QDomElement dom_room = parent_doc->createElement("AttachedRooms");
  dom_room.setAttribute(
        "RoomName", autobot_account.GetTargetRoomSet().keys().join(" "));
  dom_account.appendChild(dom_room);

  return dom_account;
}

QDomElement ConvertTaskConfigToXML(const TaskConfig& task_config,
                                   QDomDocument* parent_doc) {
  QDomElement dom_config = parent_doc->createElement("TaskConfig");
  dom_config.setAttribute("fixed_interval", task_config.fixed_interval);
  dom_config.setAttribute("fixed_order", task_config.fixed_order);
  dom_config.setAttribute("interval_seconds", task_config.interval_seconds);
  dom_config.setAttribute("min_seconds", task_config.min_seconds);
  dom_config.setAttribute("max_seconds", task_config.max_seconds);
  return dom_config;
}

QDomElement ConvertAutobotManagerToXML(const AutobotManager& autobot_manager,
                                       QDomDocument* parent_doc) {
  QDomElement dom_manager = parent_doc->createElement("AutobotManager");
  for (const auto& autobot_account
       : autobot_manager.GetAccounts().GetUnitDict()) {
    dom_manager.appendChild(
        ConvertAutobotAccountToXML(*autobot_account, parent_doc));
  }
  for (const auto& target_room
       : autobot_manager.GetRooms().GetUnitDict()) {
    dom_manager.appendChild(
          ConvertTargetRoomToXML(*target_room, parent_doc));
  }
  for (const auto& target_speech
       : autobot_manager.GetSpeechs().GetUnitDict()) {
    dom_manager.appendChild(
          ConvertTargetSpeechToXML(*target_speech, parent_doc));
  }
  return dom_manager;
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
      !dom_element.hasAttribute("Password") ||
      !dom_element.hasAttribute("Nickname")) {
    qDebug() << "Failed to parse Username and Password";
    return false;
  }
  const QString& username = dom_element.attribute("Username");
  const QString& password = dom_element.attribute("Password");
  const QString& nickname = dom_element.attribute("Nickname");

  autobot_account->SetUsername(username);
  autobot_account->SetPassword(password);
  autobot_account->SetNickname(nickname);

  bool success = true;
  QDomNodeList child_dorms = dom_element.childNodes();
  for (int i = 0; i < child_dorms.size(); ++i) {
    QDomElement child_ele = child_dorms.at(i).toElement();
    if (child_ele.tagName() == "TaskConfig") {
      TaskConfig task_config;
      success &= ParseXMLToTaskConfig(child_ele, &task_config);
      autobot_account->SetTaskConfig(task_config);
    }
    if (child_ele.tagName() == "TagetRooms") {
      QDomNodeList target_dorms = child_ele.childNodes();
      for (int j = 0; j < target_dorms.size(); ++j) {
        auto target_room_ptr = std::make_shared<TargetRoom>();
        success &= ParseXMLToTargetRoom(target_dorms.at(j).toElement(),
                                        target_room_ptr.get());
//        autobot_account->AssignTargetRoom(target_room_ptr->GetRoomNumber());
      }
    }
  }
  return success;
}

bool ParseXMLToAutobotManager(const QDomElement& dom_element,
                              AutobotManager* account_manager) {
  QDomNodeList child_dorms = dom_element.childNodes();
  bool success = true;
//  for (int i = 0; i < child_dorms.size(); ++i) {
//    QDomElement child_ele = child_dorms.at(i).toElement();
//    // Parse autobot accounts.
//    if (child_ele.tagName() == "AutobotAccounts") {
//      QDomNodeList accounts_doms = child_ele.childNodes();
//      for (int j = 0; j < accounts_doms.size(); ++j) {
//        auto autobot_ptr = std::make_shared<AutobotAccount>();
//        success &= ParseXMLToTargetAccount(accounts_doms.at(j).toElement(),
//                                           autobot_ptr.get());
//        account_manager->Add(autobot_ptr);
//      }
//    }
//    // Parse target speeches.
//    if (child_ele.tagName() == "TargetSpeechs") {
//      QDomNodeList accounts_doms = child_ele.childNodes();
//      for (int j = 0; j < accounts_doms.size(); ++j) {
//        auto speech_ptr = std::make_shared<TargetSpeech>();
//        success &= ParseXMLToTargetSpeech(accounts_doms.at(j).toElement(),
//                                          speech_ptr.get());
//        account_manager->Add(speech_ptr);
//      }
//    }
//  }
  return success;
}

bool ParseXMLToTargetSpeech(const QDomElement& dom_element,
                            TargetSpeech* target_speech) {
  if (!dom_element.hasAttribute("Speechname")) {
    qDebug() << "Failed to parse Speechname";
    return false;
  }
  target_speech->SetSpeechName(dom_element.attribute("Speechname"));
  QDomNodeList child_dorms = dom_element.childNodes();
  QStringList word_list;
  for (int i = 0; i < child_dorms.size(); ++i) {
    const auto child_ele = child_dorms.at(i).toElement();
    if (!child_ele.hasAttribute("word")) {
      qDebug() << "Failed to parse word";
      return false;
    }
    word_list.append(child_ele.attribute("word"));
  }
  target_speech->SetWordsList(word_list);
  return true;
}

bool ParseXMLToTaskConfig(const QDomElement& dom_element,
                          TaskConfig* task_config) {
  if (!dom_element.hasAttribute("fixed_interval") ||
      !dom_element.hasAttribute("interval_seconds") ||
      !dom_element.hasAttribute("min_seconds") ||
      !dom_element.hasAttribute("max_seconds")) {
    qDebug() << "Failed to parse target config";
    return false;
  }
  task_config->fixed_interval
      = dom_element.attribute("fixed_interval") == "0" ? false : true;
  task_config->fixed_order
      = dom_element.attribute("fixed_order") == "0" ? false : true;
  task_config->interval_seconds
      = dom_element.attribute("interval_seconds").toInt();
  task_config->min_seconds = dom_element.attribute("min_seconds").toInt();
  task_config->max_seconds = dom_element.attribute("max_seconds").toInt();
  return true;
}

} // namespace

