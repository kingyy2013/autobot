#include "autobot_unit.h"

namespace autobot {

AutobotUnit::AutobotUnit(const QString& unit_name) : unit_name_(unit_name) {}

AutobotUnit::~AutobotUnit() {
  BreakConnections();
}

const QString& AutobotUnit::GetUnitName() const {
  return this->unit_name_;
}

void AutobotUnit::SetUnitName(const QString& unit_name) {
  this->unit_name_ = unit_name;
}


std::shared_ptr<AutobotUnit> AutobotUnit::GetPtr() {
  return shared_from_this();
}

void AutobotUnit::BreakConnections() {
  BreakUpperConnections();
  BreakLowerConnections();
}

void AutobotUnit::BreakUpperConnections() {
  for (const auto& upper_unit : upper_connections_) {
    RemoveUpperConnection(upper_unit);
  }
}
void AutobotUnit::BreakLowerConnections() {
  for (const auto& lower_unit : lower_connections_) {
    RemoveLowerConnection(lower_unit);
  }
}

bool AutobotUnit::AddUpperConnection(
    const std::shared_ptr<AutobotUnit>& parent) {
  if(!upper_connections_.contains(parent->GetUnitName())) {
    upper_connections_.insert(parent->GetUnitName(), parent);
    parent->AddLowerConnection(GetPtr());
    return true;
  } else {
    return false;
  }
}

bool AutobotUnit::RemoveUpperConnection(
    const std::shared_ptr<AutobotUnit>& parent) {
  return RemoveUpperConnection(parent->GetUnitName());
}


bool AutobotUnit::RemoveUpperConnection(
    const QString& parent_name) {
  if (upper_connections_.contains(parent_name)) {
    const auto parent_ptr = upper_connections_[parent_name];
    upper_connections_.remove(parent_name);
    parent_ptr->RemoveLowerConnection(GetPtr());
    return true;
  } else {
    return false;
  }
}

const ConnectionUnitList& AutobotUnit::GetAllUpperConnections() const {
  return upper_connections_;
}

bool AutobotUnit::AddLowerConnection(
    const std::shared_ptr<AutobotUnit>& child) {
  if(!lower_connections_.contains(child->GetUnitName())) {
    lower_connections_.insert(child->GetUnitName(), child);
    child->AddUpperConnection(GetPtr());
    return true;
  } else {
    return false;
  }
}

bool AutobotUnit::RemoveLowerConnection(const QString& child) {
  if (lower_connections_.contains(child)) {
    const auto child_ptr = lower_connections_[child];
    lower_connections_.remove(child);
    child_ptr->RemoveUpperConnection(GetPtr());
    return true;
  } else {
    return false;
  }
}

bool AutobotUnit::RemoveLowerConnection(
    const std::shared_ptr<AutobotUnit>& child) {
  return RemoveLowerConnection(child->GetUnitName());
}

const ConnectionUnitList& AutobotUnit::GetAllLowerConnections() const {
  return lower_connections_;
}
} // namespace
