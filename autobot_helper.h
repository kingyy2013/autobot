#ifndef AUTOBOT_HELPER_H
#define AUTOBOT_HELPER_H

#include "autobot_account.h"
#include "autobot_manager.h"
#include "target_room.h"
#include "target_speech.h"
#include "task_interface.h"

#include <QPair>
#include <QColor>
#include <QtXml>

namespace autobot {
QPair<QString, QColor> ConvertStatusToQStringAndColor(AccountStatus status);

// XML IO.
QDomElement ConvertTargetRoomToXML(const TargetRoom& target_room,
                                   QDomDocument* parent_doc);
QDomElement ConvertAutobotAccountToXML(const AutobotAccount& autobot_account,
                                      QDomDocument* parent_doc);
QDomElement ConvertAutobotManagerToXML(const AutobotManager& account_manager,
                                       QDomDocument* parent_doc);
QDomElement ConvertTargetSpeechToXML(const TargetSpeech& target_speech,
                                     QDomDocument* parent_doc);
QDomElement ConvertTaskConfigToXML(const TaskConfig& task_config,
                                   QDomDocument* parent_doc);
bool ParseXMLToTargetRoom(const QDomElement& dom_element,
                          TargetRoom* target_room);
bool ParseXMLToAutobotAccount(const QDomElement& dom_element,
                             AutobotAccount* autobot_account);
bool ParseXMLToAutobotManager(const QDomElement& dom_element,
                              AutobotManager* account_manager);
bool ParseXMLToTargetSpeech(const QDomElement& dom_element,
                            TargetSpeech* target_speech);
bool ParseXMLToTaskConfig(const QDomElement& dom_element,
                          TaskConfig* task_config);
}

#endif // AUTOBOT_HELPER_H
