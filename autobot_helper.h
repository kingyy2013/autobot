#ifndef AUTOBOT_HELPER_H
#define AUTOBOT_HELPER_H

#include "autobot_account.h"
#include "autobot_manager.h"
#include "target_room.h"
#include "target_speech.h"

#include <QPair>
#include <QColor>
#include <QtXml>

namespace autobot {
QPair<QString, QColor> ConvertStatusToQStringAndColor(AccountStatus status);

// XML IO.
QDomElement ConvertTargetRoomToXML(const TargetRoom& target_room,
                                   QDomDocument* parent_doc);
QDomElement ConvertTargetAccountToXML(const AutobotAccount& autobot_account,
                                      QDomDocument* parent_doc);
QDomElement ConvertAutobotManagerToXML(const AutobotManager& account_manager,
                                       QDomDocument* parent_doc);
QDomElement ConvertTargetSpeechToXML(const TargetSpeech& target_speech,
                                     QDomDocument* parent_doc);

bool ParseXMLToTargetRoom(const QDomElement& dom_element,
                          TargetRoom* target_room);
bool ParseXMLToTargetAccount(const QDomElement& dom_element,
                             AutobotAccount* autobot_account);
bool ParseXMLToAutobotManager(const QDomElement& dom_element,
                              AutobotManager* account_manager);
bool ParseXMLToTargetSpeech(const QDomElement& dom_element,
                            TargetSpeech* target_speech);
}

#endif // AUTOBOT_HELPER_H
