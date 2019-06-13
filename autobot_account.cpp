#include "autobot_account.h"

namespace autobot {

AutobotAccount::AutobotAccount(const QString& username,
                               const QString& password)
  : bot_username_(username),
    bot_password_(password) {
}


QString AutobotAccount::GetUsername() {
  return bot_username_;
}

QString AutobotAccount::GetPassword() {
  return bot_password_;
}


}
