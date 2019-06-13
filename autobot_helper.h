#ifndef AUTOBOT_HELPER_H
#define AUTOBOT_HELPER_H

#include "autobot_account.h"
#include "autobot_login_dialog.h"

namespace autobot {
  AutobotAccount ConvertLoginDialogToAccount(const AutobotLoginDialog& login_dialog);
}

#endif // AUTOBOT_HELPER_H
