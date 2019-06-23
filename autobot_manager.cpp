#include "autobot_manager.h"
#include <memory>

namespace autobot {
std::shared_ptr<AutobotAccount> AutobotManager::Find(
    const QString& autobot_name) const {
  auto account_dict_itr = account_dict_.find(autobot_name);
  if (account_dict_itr == account_dict_.end()) {
    return std::shared_ptr<AutobotAccount>();
  } else {
    return account_dict_itr.value();
  }
}

AutobotManager& AutobotManager::GetInstance() {
  static AutobotManager autobot_manager_singleton;
  return autobot_manager_singleton;
}


void AutobotManager::AddAccount(
    const std::shared_ptr<AutobotAccount>& autobot_account) {
  account_dict_[autobot_account->GetUsername()] = autobot_account;
}

void AutobotManager::RemoveAutobot(const QString& autobot_name) {
  account_dict_.remove(autobot_name);
}

const AutobotAccountMap& AutobotManager::GetAccountDict() const {
  return account_dict_;
}

}// namespace
