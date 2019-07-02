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
    void Add(const std::shared_ptr<UnitType>& unit_name);
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

//  // Template functions.
//  /// @brief Added new autobot account to the manager.
//  /// @param autobot_account. Autobot account that is being added.
//  template<class AutobotUnitType>
//  void Add(const std::shared_ptr<AutobotUnitType>& unit_name);
//  template<class AutobotUnitType>
//  void RemoveSelection();
//  template<class AutobotUnitType>
//  void RemoveMuti(const QStringList& unit_names);
//  template<class AutobotUnitType>
//  void Remove(const QString& unit_name);
//  template<class AutobotUnitType>
//  void BreakUpper(const QString& unit_name);
//  template<class AutobotUnitType>
//  std::shared_ptr<AutobotUnitType> GetUnitPtr(
//      const QString& dict_name) const;

//  template<class AutobotUnitType>
//  void SetSelectedNames(const QStringList& selected_names);
//  template<class AutobotUnitType>
//  const QStringList& GetSelectedNames() const;
//  template<class AutobotUnitType>
//  const AutobotUnitDict GetAutobotUniDict() const ;
  // Template functions.


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

// typedef template<> void Remove<AutobotAccount>(const QString& unit_name) RemoveAutobotAccount;
//  using RemoveAutobotAccount = decltype(template<> void Remove<AutobotAccount>(const QString& unit_name));

//  typedef template<> void Remove<AutobotAccount>() RemoveAutobotAccount;

//  // Usage functions.
//  std::shared_ptr<TargetRoom> GetTargetRoomPtr(
//      const QString& room_name) const;
//  std::shared_ptr<AutobotAccount> GetAutobotAccountPtr(
//      const QString& account_name) const;
//  std::shared_ptr<TargetSpeech> GetTargetSpeech(
//      const QString& speech_name) const;

//  inline void RemoveAccount(const QString& name) {
//    Remove<AutobotAccount>(name);
//  }
//  inline void RemoveRoom(const QString& name) {
//    Remove<TargetRoom>(name);
//  }
//  inline void RemoveSpeech(const QString& name) {
//    this->Remove<TargetSpeech>(name);
//  }

  // Connection related.
  // Break TargetRoom from AutobotAccount, but still keep the instance.
//  void BreakTargetRoomPtr(const QString& room_name);
  // Break TargetSpeech from TargetRoom, but still keep the instance.
//  void BreakTargetSpeechPtr(const QString& room_name);

//  bool AssignSelectedSpeechsToSelectedRooms();
//  void AssignSpeechToRoom(const QString& speech_name,
//                          const QString& roomt_name);

//  bool AssignSelectedRoomsToSelectedAccounts();
//  void AssignRoomToAccount(const QString& roomt_name,
//                           const QString& account_name);


//  /// @brief Find the AutobotAccount instance.
//  std::shared_ptr<AutobotAccount> FindAccount(const QString& account_name) const;

//  void AddRoom(const std::shared_ptr<TargetRoom>& room_ptr);
//  void RemoveSelectedRooms();
//  void RemoveRooms(const QStringList& room_names);
//  void RemoveRoom(const QString& room_names);
//  std::shared_ptr<TargetRoom> FindRoom(const QString& room_name) const;

//  void AddSpeech(const std::shared_ptr<TargetSpeech>& speech_ptr);
//  void RemoveSelectedSpeechs();
//  void RemoveSpeechs(const QStringList& speech_names);
//  void RemoveSpeech(const QString& speech_name);
//  std::shared_ptr<TargetSpeech> FindSpeech(const QString& speech_name) const;


  const QString& GetErrorMessage() const;

//  // Sets selected nicknames.
//  void SetSelectedAcountNames(const QStringList& selected_names);
//  void SetSelectedRoomNames(const QStringList& selected_names);
//  void SetSelectedSpeechNames(const QStringList& selected_names);

//  const QStringList& GetSelectedAcountNames() const;
//  const QStringList& GetSelectedRoomNames() const;
//  const QStringList& GetSelectedSpeechNames() const;

//  const AutobotAccountDict& GetAccountDict() const;
//  const TargetRoomDict& GetRoomDict() const;
//  const TargetSpeechSetDict& GetSpeechDict() const;

signals:
  /// @brief Signal when the selected accounts have changed.
  /// @param selected_accounts. The name of the accounts that has changed.
  void AccountsChanged(const QStringList& selected_accounts);

  /// @brief Signal when the selected rooms have changed.
  /// @param selected_rooms. The name of the rooms that has changed.
  void RoomsChanged(const QStringList& selected_rooms);

  /// @brief Signal for all the accounts changed.
  void AccountsChanged();

  /// @brief Signal when the accounts selection has changed.
  /// @param selected_accounts. The name of the selected accounts that will be
  /// changed into.
  void AccountSelectionChanged(const QStringList& selected_accounts);

  void AccountStatusChanged();

private:
//  AutobotAccountDict autobot_accounts_dict_;
//  TargetRoomDict target_rooms_dict_;
//  TargetSpeechSetDict target_speechs_dict_;

//  QStringList selected_account_names_;
//  QStringList selected_room_names_;
//  QStringList selected_speech_names_;

//  QHash<QString, AutobotUnitDict> type_to_unit_ptr_map_;
//  QHash<QString, QStringList> type_to_selection_map_;
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

