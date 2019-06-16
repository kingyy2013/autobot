#ifndef AUTOBOT_HELPER_H
#define AUTOBOT_HELPER_H

#include "autobot_account.h"
#include <QPair>
#include <QColor>

namespace autobot {
QPair<QString, QColor> ConvertStatusToQStringAndColor(AccountStatus status);

}

#endif // AUTOBOT_HELPER_H
