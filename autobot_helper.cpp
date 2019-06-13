#include "autobot_helper.h"

namespace autobot {
AutobotAccount ConvertLoginDialogToAccount(
    const AutobotLoginDialog& login_dialog) {
  return AutobotAccount(login_dialog.GetUsername(),
                        login_dialog.GetPassword());
}
}
