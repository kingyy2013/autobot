#include "autobot_account.h"

namespace autobot {

AutobotAccount::AutobotAccount(const QString& username,
                               const QString& password)
  : bot_username_(username),
    bot_password_(password),
    bot_status_(AccountStatus::UNINITIALIZED) {
}


QString AutobotAccount::GetUsername() {
  return bot_username_;
}

QString AutobotAccount::GetPassword() {
  return bot_password_;
}

AccountStatus AutobotAccount::GetStatus() {
  return bot_status_;
}

}
