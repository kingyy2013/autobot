#include "autobot_manager.h"
#include <memory>
#include <QSet>

#include <QDebug>

namespace autobot {

AutobotManager::AutobotManager() {
}

// Singleton instance.
AutobotManager& AutobotManager::GetInstance() {
  static AutobotManager autobot_manager_singleton;
  return autobot_manager_singleton;
}

AutobotManager::AutobotAccountHandler& AutobotManager::GetAccounts() {
  return GetInstance().MutableAccounts();
}

AutobotManager::TargetRoomHandler& AutobotManager::GetRooms() {
return GetInstance().MutableRooms();
}

AutobotManager::TargetSpeechHandler& AutobotManager::GetSpeechs() {
  return GetInstance().MutableSpeechs();
}

template<class AutobotUnitType>
void AutobotManager::AutobotSelectionHandler<AutobotUnitType>::Add(
    const std::shared_ptr<AutobotUnitType>& unit_ptr) {
  unit_ptr_map_[unit_ptr->GetUnitName()] = unit_ptr;
}

template<class AutobotUnitType>
void AutobotManager::AutobotSelectionHandler<AutobotUnitType>::RemoveSelection(
    ) {
 RemoveMuti(selected_unit_names_);
}

template<class UnitType>
void AutobotManager::AutobotSelectionHandler<UnitType>::RemoveMuti(
    const QStringList& unit_names) {
  for (const auto& unit_name : unit_names) {
    Remove(unit_name);
  }
}

template<class UnitType>
void AutobotManager::AutobotSelectionHandler<UnitType>::Remove(
    const QString& unit_name) {
  unit_ptr_map_[unit_name]->BreakConnections();
  unit_ptr_map_.remove(unit_name);
}

template<class UnitType>
void AutobotManager::AutobotSelectionHandler<UnitType>::BreakUpper(
    const QString& unit_name, const QString& upper_name) {
  unit_ptr_map_[unit_name]->RemoveUpperConnection(upper_name);
}

template<class UnitType>
std::shared_ptr<UnitType> AutobotManager::AutobotSelectionHandler<UnitType>
::GetUnitPtr(
    const QString& dict_name) const {
  const auto& unit_ptr_itr = unit_ptr_map_.find(dict_name);
  if (unit_ptr_itr != unit_ptr_map_.end()) {
    return std::static_pointer_cast<UnitType>(unit_ptr_itr.value());
  } else {
    return nullptr;
  }
}

template<class UnitType>
void AutobotManager::AutobotSelectionHandler<UnitType>::AssignedSelectedToUpper(
    const std::shared_ptr<AutobotUnit> upper_unit_ptr) {
  for (const auto& selected_name : selected_unit_names_) {
    const auto& unit_ptr = unit_ptr_map_[selected_name];
    upper_unit_ptr->AddLowerConnection(unit_ptr);
  }
}

template<class UnitType>
void AutobotManager::AutobotSelectionHandler<UnitType>::SetSelectedNames(
    const QStringList& selected_names) {
  selected_unit_names_ = selected_names;
}

template<class UnitType>
const QStringList&
AutobotManager::AutobotSelectionHandler<UnitType>::GetSelectedNames() const {
  return selected_unit_names_;
}

template<class UnitType>
const QHash<QString, std::shared_ptr<UnitType>>&
AutobotManager::AutobotSelectionHandler<UnitType>::GetUnitDict() const {
  return unit_ptr_map_;
}

//std::shared_ptr<TargetRoom> AutobotManager::GetTargetRoomPtr(
//    const QString& room_name) const {
//  return GetUnitPtr<TargetRoom>(room_name);
//}

//std::shared_ptr<AutobotAccount> AutobotManager::GetAutobotAccountPtr(
//    const QString& account_name) const {
//  return GetUnitPtr<AutobotAccount>(account_name);
//}

//std::shared_ptr<TargetSpeech> AutobotManager::GetTargetSpeech(
//    const QString& speech_name) const {
//  return GetUnitPtr<TargetSpeech>(speech_name);
//}

//// All the operations related to accounts.
//void AutobotManager::AddAccount(
//    const std::shared_ptr<AutobotAccount>& autobot_account) {
//  autobot_accounts_dict_[autobot_account->GetUsername()] = autobot_account;
//}

//void AutobotManager::RemoveSelectedAccounts() {
//  RemoveAccounts(this->selected_account_names_);
//}

//void AutobotManager::RemoveAccounts(const QStringList& account_names) {
//  for (const auto& account_name : account_names) {
//    RemoveAccount(account_name);
//  }
//}

//void AutobotManager::RemoveAccount(const QString& account_name) {
//  autobot_accounts_dict_[account_name]->BreakConnections();
//  autobot_accounts_dict_.remove(account_name);
//}

//std::shared_ptr<AutobotAccount> AutobotManager::FindAccount(
//    const QString& account_name) const {
//  auto account_dict_itr = autobot_accounts_dict_.find(account_name);
//  if (account_dict_itr == autobot_accounts_dict_.end()) {
//    return nullptr;
//  } else {
//    return account_dict_itr.value();
//  }
//}

//// All the operations related to room.
//void AutobotManager::AddRoom(
//    const std::shared_ptr<TargetRoom>& room_ptr) {
//  target_rooms_dict_[room_ptr->GetRoomNumber()] = room_ptr;
//}

//void AutobotManager::RemoveSelectedRooms(const QStringList& account_names) {
//  RemoveRooms(this->selected_room_names_);
//}

//void AutobotManager::RemoveTargetRoom(const QStringList& room_names) {
//  RemoveMuti<TargetRoom>(room_names);
//}

//void AutobotManager::RemoveRoom(const QString& account_name) {
//  Remove<TargetRoom>(room_names);
//}

//std::shared_ptr<TargetRoom> AutobotManager::FindRoom(
//    const QString& room_name) const {
//  auto target_room_itr = target_rooms_dict_.find(room_name);
//  if (target_room_itr == target_rooms_dict_.end()) {
//    return nullptr;
//  } else {
//    return target_room_itr.value();
//  }
//}

//// All the operations related to speech.
//void AutobotManager::AddSpeech(
//    const std::shared_ptr<TargetSpeech>& speech_ptr) {
//  target_speechs_dict_[speech_ptr->SpeechName()] = speech_ptr;
//}

//void AutobotManager::RemoveSpeechs(const QStringList& speech_names) {
//  for (const auto& speech_name : speech_names) {
//    RemoveSpeech(speech_name);
//  }
//}

//void AutobotManager::RemoveSpeech(const QString& speech_name) {
//  target_speechs_dict_[speech_name]->BreakConnections();
//  target_speechs_dict_.remove(speech_name);
//}

//std::shared_ptr<TargetSpeech> AutobotManager::FindSpeech(
//    const QString& speech_name) const {
//  auto target_speech_itr = target_speechs_dict_.find(speech_name);
//  if (target_speech_itr == target_speechs_dict_.end()) {
//    return nullptr;
//  } else {
//    return target_speech_itr.value();
//  }
//}

bool AutobotManager::AssignSelectedRoomsToSelectedAccounts() {
  QStringList selected_accounts = autobot_accounts_handler_.GetSelectedNames();
  for (const auto& selected_account : selected_accounts) {
    const auto& selected_account_ptr
        = autobot_accounts_handler_.GetUnitPtr(selected_account);
    qDebug() << "selected_account: " << selected_account;
    // Account -> contains rooom.
    target_rooms_handler_.AssignedSelectedToUpper(selected_account_ptr);
  }
  emit AccountsChanged(selected_accounts);
  return true;
}

bool AutobotManager::AssignSelectedSpeechsToSelectedRooms() {
  QStringList selected_rooms = target_rooms_handler_.GetSelectedNames();
  for (const auto& selected_room : selected_rooms) {
    const auto& selected_room_ptr
        = target_rooms_handler_.GetUnitPtr(selected_room);
    // Room -> contains speech.
    target_speechs_handler_.AssignedSelectedToUpper(selected_room_ptr);
  }
  emit RoomsChanged(selected_rooms);
  return true;
}

//template <class LowerLayerTypes, class UpperLayerTypes>
//bool AutobotManager::AssignSelectedUnitsToSelectedUnits() {
//  const QStringList& selected_lower_names_
//      = type_to_selection_map_[LowerLayerTypes::GetTypeNameStatic()];
//  const QStringList& selected_upper_names_
//      = type_to_selection_map_[UpperLayerTypes::GetTypeNameStatic()];
//  bool success = true;
//  for (const auto& lower_unit_name : selected_lower_names_) {
//    for (const auto& upper_unit_name : selected_upper_names_) {
//      success &= AssignUnitToUnit<LowerLayerTypes, UpperLayerTypes>
//          (lower_unit_name, upper_unit_name);
//    }
//  }
//  return success;
//}

//template <class LowerUnitTypes, class UpperUnitTypes>
//bool AutobotManager::AssignUnitToUnit(const QString& lower_unit_name,
//                                      const QString& upper_unit_name) {
//  AutobotUnitDict& lower_unit_dict_
//      = type_to_unit_ptr_map_[LowerUnitTypes::GetTypeNameStatic()];
//  AutobotUnitDict& upper_unit_dict_
//      = type_to_unit_ptr_map_[UpperUnitTypes::GetTypeNameStatic()];
//  return lower_unit_dict_[lower_unit_name]->
//      AddUpperConnection(upper_unit_dict_[upper_unit_name]);
//}

//template<class AutobotUnitType>
//void AutobotManager::SetSelectedNames(const QStringList& selected_names) {
//  type_to_selection_map_[AutobotUnitType::GetTypeNameStatic()] = selected_names;
//}

//template<class AutobotUnitType>
//const QStringList& AutobotManager::GetSelectedNames() const {
//  return type_to_selection_map_[AutobotUnitType::GetTypeNameStatic()];
//}

//template<class AutobotUnitType>
//const AutobotUnitDict AutobotManager::GetAutobotUniDict() const {
//  return type_to_unit_ptr_map_[AutobotUnitType::GetTypeNameStatic()];
//}


//// Connections assignment.
//void AutobotManager::AssignSelectedUnitsToSelectedUnits() {
//  const QStringList& selected_upper_names
//      = type_to_selection_map_[UpperLayerTypes::GetTypeNameStatic()];
//  const QStringList& selected_lower_names
//      = type_to_selection_map_[LowerLayerTypes::GetTypeNameStatic()];

//  for (const QString& selected_upper_name : selected_upper_names) {
//    for (const QString& selected_lower_name : selected_lower_names) {
//      AssignSpeechToRoom(selected_lower_name,
//                         selected_upper_name);
//    }
//  }
//}

//void AutobotManager::AssignSpeechToRoom(const QString& speech_name,
//                                        const QString& room_name) {
//  AutobotUnitDict& target_rooms_dict_
//      = type_to_unit_ptr_map_[TargetRoom::GetTypeNameStatic()];
//  AutobotUnitDict& target_speechs_dict_
//      = type_to_unit_ptr_map_[TargetSpeech::GetTypeNameStatic()];
//  std::static_pointer_cast<TargetRoom>(target_rooms_dict_[room_name])->
//      AssignTargetSpeech(target_speechs_dict_[speech_name]);
//}

//void AutobotManager::AssignRoomToAccount(const QString& room_name,
//                                         const QString& account_name) {
//  AutobotUnitDict& target_rooms_dict_
//      = type_to_unit_ptr_map_[TargetRoom::GetTypeNameStatic()];
//  AutobotUnitDict& autobot_accounts_dict_
//      = type_to_unit_ptr_map_[AutobotAccount::GetTypeNameStatic()];
//  std::static_pointer_cast<AutobotAccount>(
//        autobot_accounts_dict_[account_name])->
//      AssignTargetRoom(target_rooms_dict_[room_name]);
//}

//void AutobotManager::SetSpeechContent(const QString& speech_name,
//                                      const QStringList& speech_content) {
//  target_speech_dict_[speech_name]->SetWordsList(speech_content);
//}
//void AutobotManager::SetSelectedAcountNames(
//    const QStringList& selected_names) {
//  selected_account_names_ = selected_names;
//}

//void AutobotManager::SetSelectedRoomNames(
//    const QStringList& selected_names) {
//  selected_room_names_ = selected_names;
//}
//void AutobotManager::SetSelectedSpeechNames(
//    const QStringList& selected_names) {
//  selected_speech_names_ = selected_names;
//}


//const QStringList& AutobotManager::GetSelectedAcountNames() const {
//  return selected_account_names_;
//}
//const QStringList& AutobotManager::GetSelectedRoomNames() const {
//  return selected_room_names_;
//}
//const QStringList& AutobotManager::GetSelectedSpeechNames() const {
//  return selected_speech_names_;
//}


//const AutobotAccountDict& AutobotManager::GetAccountDict() const {
//  return autobot_accounts_dict_;
//}
//const TargetRoomDict& AutobotManager::GetRoomDict() const {
//  return target_rooms_dict_;
//}
//const TargetSpeechSetDict& AutobotManager::GetSpeechDict() const {
//  return target_speechs_dict_;
//}


//void AutobotManager::RebuildSpeechToRoomMap() {
//  for (const auto& speech_name : target_speech_dict_) {
//    speech_to_room_list_[speech_name->SpeechName()] = QSet<QString>();
//  }
//  for (const auto& account : account_dict_) {
//    const auto& speech_name = account->GetSpeechName();
//    if (!speech_to_room_list_.contains(speech_name)) {
//      qFatal (("Target speech" + speech_name + "from account " +
//               account->GetUsername() +
//               "is not found in the speech dictionary.").toStdString().data());
//    }
//    speech_to_room_list_[speech_name].insert(account->GetUsername());
//  }
//}

//template class AutobotManager::AutobotSelectionHandler<AutobotAccount>;
//template class AutobotManager::AutobotSelectionHandler<TargetRoom>;
//template class AutobotManager::AutobotSelectionHandler<TargetSpeech>;

}// namespace
