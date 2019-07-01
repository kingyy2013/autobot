#include "autobot_unit.h"

namespace autobot {

AutobotUnit::AutobotUnit(const QString& unit_name) : unit_name_(unit_name) {}

AutobotUnit::~AutobotUnit() {
  for (const auto& upper_unit : upper_connections_) {
    upper_unit->RemoveLowerConnection(shared_from_this());
  }
  for (const auto& lower_unit_ : lower_connections_) {
    lower_unit_->RemoveUpperConnection(shared_from_this());
  }
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
  if(!upper_connections_.count(parent)) {
    upper_connections_.insert(parent);
    parent->AddLowerConnection(GetPtr());
    return true;
  } else {
    return false;
  }
}

bool AutobotUnit::RemoveUpperConnection(
    const std::shared_ptr<AutobotUnit>& parent) {
  if (upper_connections_.count(parent)) {
    upper_connections_.erase(parent);
    parent->RemoveLowerConnection(GetPtr());
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
  if(!lower_connections_.count(child)) {
    lower_connections_.insert(child);
    child->AddUpperConnection(GetPtr());
    return true;
  } else {
    return false;
  }
}

bool AutobotUnit::RemoveLowerConnection(
    const std::shared_ptr<AutobotUnit>& child) {
  if (lower_connections_.count(child)) {
    lower_connections_.erase(child);
    child->RemoveUpperConnection(GetPtr());
    return true;
  } else {
    return false;
  }
}

const ConnectionUnitList& AutobotUnit::GetAllLowerConnections() const {
  return upper_connections_;
}
} // namespace
