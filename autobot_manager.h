#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <QThreadPool>
#include <QHash>
#include <QSet>
#include <memory>

#include "autobot_account.h"
#include "target_speech_set.h"

namespace autobot {

typedef QHash<QString, std::shared_ptr<AutobotAccount>> AutobotAccountMap;

class AutobotManager : public QObject {
  Q_OBJECT

public:
  AutobotManager();
  /// @brief Added new autobot account to the manager.
  /// @param autobot_account. Autobot account that is being added.
  void AddAccount(const std::shared_ptr<AutobotAccount>& autobot_account);
  /// @brief Find the AutobotAccount instance.
  std::shared_ptr<AutobotAccount> Find(const QString& autobot_name) const;

  void RemoveAutobot(const QString& autobot_name);
  void AddSpeech(const std::shared_ptr<TargetSpeech>& speech_ptr);
  void SetSpeechContent(
      const QString& speech_name,
      const QStringList& speech_content);

  void AssignSpeechToAccount(const QString& speech_name,
                             const QString& account_name);

  void RemoveSpeech(const QString& speech_name);

  const AutobotAccountMap& GetAccountDict() const;

  const TargetSpeechSetMap& GetSpeechDict() const;

  static AutobotManager& GetInstance();

  void RebuildSpeechToAccountMap();

  // Sets selected nicknames.
  void SetSelectedAcountNames(const QStringList& selected_autobots);
  const QStringList& GetSelectedAcountNames() const;

signals:
  void AccountsChanged(const QStringList&);
  void AccountsChanged();

  void AccountStatusChanged();

private:
  AutobotAccountMap account_dict_;
  TargetSpeechSetMap target_speech_dict_;
  QStringList selected_autobot_nicknames_;
  QHash<QString, QSet<QString>> speech_to_account_list_;
};

}// namespace
#endif // ACCOUNT_MANAGER_H
