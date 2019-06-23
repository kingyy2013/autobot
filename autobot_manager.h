#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <QThreadPool>
#include <QHash>
#include <memory>

#include "autobot_account.h"

namespace autobot {

typedef QHash<QString, std::shared_ptr<AutobotAccount>> AutobotAccountMap;

class AutobotManager {
public:
  AutobotManager() = default;
  /// @brief Added new autobot account to the manager.
  /// @param autobot_account. Autobot account that is being added.
  void AddAccount(const std::shared_ptr<AutobotAccount>& autobot_account);
  /// @brief Find the AutobotAccount instance.
  std::shared_ptr<AutobotAccount> Find(const QString& autobot_name) const;
  void RemoveAutobot(const QString& autobot_name);
  const AutobotAccountMap& GetAccountDict() const;
  static AutobotManager& GetInstance();
private:
  QHash<QString, std::shared_ptr<AutobotAccount>> account_dict_;
  QThreadPool thread_pool_;
};

}// namespace
#endif // ACCOUNT_MANAGER_H
