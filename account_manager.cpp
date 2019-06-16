#include "account_manager.h"

namespace autobot {
std::shared_ptr<AutobotAccount> AccountManager::Find(
    const QString& autobot_name) const {
  auto account_dict_itr = account_dict_.find(autobot_name);
  if (account_dict_itr == account_dict_.end()) {
    return std::shared_ptr<AutobotAccount>();
  } else {
    return account_dict_itr.value();
  }
}

void AccountManager::AddAccount(
    const std::shared_ptr<AutobotAccount>& autobot_account) {
  account_dict_[autobot_account->GetUsername()] = autobot_account;
}

void AccountManager::RemoveAutobot(const QString& autobot_name) {
  account_dict_.remove(autobot_name);
}

const QHash<QString, std::shared_ptr<AutobotAccount>>&
AccountManager::GetAccountDict() {
  return account_dict_;
}

}// namespace
