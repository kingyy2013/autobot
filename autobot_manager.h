#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <QThreadPool>
#include <QHash>
#include <QSet>
#include <memory>

#include "autobot_unit.h"
#include "autobot_account.h"
#include "target_speech_set.h"
#include "target_room.h"

namespace autobot {

//typedef QHash<QString, std::shared_ptr<AutobotAccount>> AutobotAccountDict;
//typedef QHash<QString, std::shared_ptr<TargetRoom>> TargetRoomDict;
typedef QHash<QString, std::shared_ptr<AutobotUnit>> AutobotUnitDict;

class AutobotManager : public QObject {
  Q_OBJECT
  /// @brief Added new autobot account to the manager.
  /// @param autobot_account. Autobot account that is being added.
  template<class UnitType>
  class AutobotSelectionHandler {
  public:
    AutobotSelectionHandler() = default;
    bool Add(const std::shared_ptr<UnitType>& unit_name);
    void RemoveSelection();
    void RemoveMuti(const QStringList& unit_names);
    void Remove(const QString& unit_name);
    void BreakUpper(const QString& unit_name, const QString& upper_name);
    std::shared_ptr<UnitType> GetUnitPtr(
        const QString& dict_name) const;

    void AssignedSelectedToUpper(
        const std::shared_ptr<AutobotUnit> upper_unit_ptr);
    void SetSelectedNames(const QStringList& selected_names);
    const QStringList& GetSelectedNames() const;
    const QStringList GetAllNames() const;
    const QHash<QString, std::shared_ptr<UnitType>>& GetUnitDict() const;

    QHash<QString, std::shared_ptr<UnitType>> unit_ptr_map_;
    QStringList selected_unit_names_;
  };

  typedef AutobotSelectionHandler<AutobotAccount> AutobotAccountHandler;
  typedef AutobotSelectionHandler<TargetRoom> TargetRoomHandler;
  typedef AutobotSelectionHandler<TargetSpeech> TargetSpeechHandler;

public:
  AutobotManager();
  /// @brief The global singlton instance.
  static AutobotManager& GetInstance();
  static AutobotAccountHandler& GetAccounts();
  static TargetRoomHandler& GetRooms();
  static TargetSpeechHandler& GetSpeechs();

  bool AssignSelectedRoomsToSelectedAccounts();
  bool AssignSelectedSpeechsToSelectedRooms();

  inline AutobotAccountHandler& MutableAccounts() {
    return autobot_accounts_handler_;
  }
  inline TargetRoomHandler& MutableRooms() {
    return target_rooms_handler_;
  }
  inline TargetSpeechHandler& MutableSpeechs() {
    return target_speechs_handler_;
  }

  const QString& GetErrorMessage() const;

signals:
  /// @brief Signal when the selected accounts have changed.
  /// @param selected_accounts. The name of the accounts that has changed.
  void AccountsChanged(const QStringList& selected_accounts);

  /// @brief Signal when the selected rooms have changed.
  /// @param selected_rooms. The name of the rooms that has changed.
  void RoomsChanged(const QStringList& selected_rooms);

  void RoomsRemoved(const QStringList& selected_rooms);
  void SpeechsRemoved(const QStringList& selected_speechs);

  /// @brief Signal for all the accounts changed.
  void AccountsChanged();

  /// @brief Signal when the accounts selection has changed.
  /// @param selected_accounts. The name of the selected accounts that will be
  /// changed into.
  void AccountSelectionChanged(const QStringList& selected_accounts);

  void AccountStatusChanged();

private:
  AutobotAccountHandler autobot_accounts_handler_;
  TargetRoomHandler target_rooms_handler_;
  TargetSpeechHandler target_speechs_handler_;
  QString error_message_;
};

template class AutobotManager::AutobotSelectionHandler<AutobotAccount>;
template class AutobotManager::AutobotSelectionHandler<TargetRoom>;
template class AutobotManager::AutobotSelectionHandler<TargetSpeech>;

}// namespace
#endif // ACCOUNT_MANAGER_H

