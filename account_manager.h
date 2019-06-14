#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <QThreadPool>
#include <QHash>
#include <memory>

#include "autobot_account.h"

namespace autobot {

class AccountManager {
public:
  AccountManager() = default;
  bool AddAccount(const std::shared_ptr<AutobotAccount>& autobot_account);
  bool RemoveAutobot(const QString& autobot_name);

private:
  QHash<QString, std::shared_ptr<AutobotAccount>> account_dict_;
  QThreadPool thread_pool_;
};

}// namespace
#endif // ACCOUNT_MANAGER_H
