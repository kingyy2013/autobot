#include "autobot_manager.h"
#include <memory>
#include <QSet>
#include "target_speech_set.h"
#include <QDebug>

namespace autobot {

AutobotManager::AutobotManager() {
  for (auto default_speech : CreateDefaultTargetSpeechSet()) {
    target_speechs_handler_.Add(default_speech);
  }
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
bool AutobotManager::AutobotSelectionHandler<AutobotUnitType>::Add(
    const std::shared_ptr<AutobotUnitType>& unit_ptr) {
  if (!unit_ptr_map_.contains(unit_ptr->GetUnitName())) {
    unit_ptr_map_[unit_ptr->GetUnitName()] = unit_ptr;
    return true;
  } else {
    return false;
  }
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
const QStringList
AutobotManager::AutobotSelectionHandler<UnitType>::GetAllNames() const {
  QStringList all_names;
  for (const auto& unit : unit_ptr_map_) {
    all_names.append(unit->GetUnitName());
  }
  return all_names;
}

template<class UnitType>
const QHash<QString, std::shared_ptr<UnitType>>&
AutobotManager::AutobotSelectionHandler<UnitType>::GetUnitDict() const {
  return unit_ptr_map_;
}

bool AutobotManager::AssignSelectedRoomsToSelectedAccounts() {
  QStringList selected_accounts = autobot_accounts_handler_.GetSelectedNames();
  for (const auto& selected_account : selected_accounts) {
    const auto& selected_account_ptr
        = autobot_accounts_handler_.GetUnitPtr(selected_account);
    // Account -> contains rooom.
    target_rooms_handler_.AssignedSelectedToUpper(selected_account_ptr);
  }
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
  return true;
}


}// namespace
