#include "autobot_manager.h"
#include <memory>
#include <QSet>

namespace autobot {

AutobotManager::AutobotManager() :
  target_speech_dict_(*CreateDefaultTargetSpeechSet()) {
  for (auto target_speech : target_speech_dict_) {
    speech_to_account_list_[target_speech->SpeechName()] = QSet<QString>();
  }
}

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

void AutobotManager::AssignSpeechToAccount(const QString& speech_name,
                                           const QString& account_name) {
  account_dict_[account_name]->SetSpeechName(speech_name);
  speech_to_account_list_[speech_name].insert(account_name);
  emit AccountsChanged(QStringList(account_name));
}

void AutobotManager::SetSpeechContent(const QString& speech_name,
                                      const QStringList& speech_content) {
  target_speech_dict_[speech_name]->SetWordsList(speech_content);
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

void AutobotManager::SetSelectedAcountNames(
    const QStringList& selected_autobots) {
  selected_autobot_nicknames_ = selected_autobots;
}

const QStringList& AutobotManager::GetSelectedAcountNames() const {
  return selected_autobot_nicknames_;
}

const TargetSpeechSetMap& AutobotManager::GetSpeechDict() const {
  return target_speech_dict_;
}

void AutobotManager::AddSpeech(
    const std::shared_ptr<TargetSpeech>& speech_ptr) {
  target_speech_dict_[speech_ptr->SpeechName()] = speech_ptr;
  speech_to_account_list_[speech_ptr->SpeechName()] = QSet<QString>();
}


void AutobotManager::RemoveSpeech(const QString& speech_name) {
  // Remove the speech set from the account.
  QSet<QString> associated_accounts = speech_to_account_list_[speech_name];
  for (auto associated_account : associated_accounts) {
    // Set the account speech to default.
    account_dict_[associated_account]->SetSpeechName(kDefaultSpeechName);
  }
  target_speech_dict_.remove(speech_name);
  speech_to_account_list_.remove(speech_name);
  emit AccountsChanged(associated_accounts.toList());
}

void AutobotManager::RebuildSpeechToAccountMap() {
  for (const auto& speech_name : target_speech_dict_) {
    speech_to_account_list_[speech_name->SpeechName()] = QSet<QString>();
  }
  for (const auto& account : account_dict_) {
    const auto& speech_name = account->GetSpeechName();
    if (!speech_to_account_list_.contains(speech_name)) {
      qFatal (("Target speech" + speech_name + "from account " +
               account->GetUsername() +
               "is not found in the speech dictionary.").toStdString().data());
    }
    speech_to_account_list_[speech_name].insert(account->GetUsername());
  }
}

}// namespace
