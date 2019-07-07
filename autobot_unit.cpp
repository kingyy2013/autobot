#include "autobot_unit.h"
#include <QDebug>

namespace autobot {

AutobotUnit::AutobotUnit(const QString& unit_name) : unit_name_(unit_name) {}

AutobotUnit::~AutobotUnit() {
  qDebug() << unit_name_ << "'s destructor called, with upper connection: "
           << upper_connections_.keys() << ", lower connection: "
           << lower_connections_.keys();

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
  for (const auto& upper_unit_name : upper_connections_.keys()) {
    RemoveUpperConnection(upper_unit_name);
  }
}
void AutobotUnit::BreakLowerConnections() {
  for (const auto& lower_unit_name : lower_connections_.keys()) {
    RemoveLowerConnection(lower_unit_name);
  }
}

bool AutobotUnit::AddUpperConnection(
    const std::shared_ptr<AutobotUnit>& parent) {
  if (parent == nullptr) {
    qFatal("Input pointer to AutobotUnit::AddUpperConnection can not be null");
  }
  if(!upper_connections_.contains(parent->GetUnitName())) {
    upper_connections_.insert(parent->GetUnitName(), parent);
    parent->AddLowerConnection(GetPtr());
    return true;
  } else {
    return false;
  }
}

bool AutobotUnit::ReserveUpperConnection(const QString& parent) {
  if(!upper_connections_.contains(parent)) {
    upper_connections_[parent] = nullptr;
    return true;
  } else {
    return false;
  }
}

bool AutobotUnit::RemoveUpperConnection(
    const QString& parent_name) {
  if (upper_connections_.contains(parent_name)) {
    const auto parent_ptr = upper_connections_[parent_name];
    upper_connections_.remove(parent_name);
    if (parent_ptr != nullptr) {
      parent_ptr->RemoveLowerConnection(unit_name_);
    }
    return true;
  } else {
    return false;
  }
}

const AutobotUnitDict& AutobotUnit::GetAllUpperConnections() const {
  return upper_connections_;
}

bool AutobotUnit::AddLowerConnection(
    const std::shared_ptr<AutobotUnit>& child) {
  if (child == nullptr) {
    qFatal("Input pointer to AutobotUnit::AddLowerConnection can not be null");
  }
  if(!lower_connections_.contains(child->GetUnitName())) {
    lower_connections_.insert(child->GetUnitName(), child);
    child->AddUpperConnection(GetPtr());
    return true;
  } else {
    return false;
  }
}

bool AutobotUnit::ReserveLowerConnection(const QString& child) {
  if(!lower_connections_.contains(child)) {
    lower_connections_[child] = nullptr;
    return true;
  } else {
    return false;
 }
}

bool AutobotUnit::RemoveLowerConnection(const QString& child) {
  if (lower_connections_.contains(child)) {
    const auto child_ptr = lower_connections_[child];
    lower_connections_.remove(child);
    if (child_ptr != nullptr) {
      child_ptr->RemoveUpperConnection(unit_name_);
    }
    return true;
  } else {
    return false;
  }
}

const AutobotUnitDict& AutobotUnit::GetAllLowerConnections() const {
  return lower_connections_;
}


void AutobotUnit::ReassignUpperConnection(
    const QString& connection_name,
    const std::shared_ptr<AutobotUnit>& new_ptr) {
  upper_connections_[connection_name] = new_ptr;
}

void AutobotUnit::ReassignLowerConnection(
    const QString& connection_name,
    const std::shared_ptr<AutobotUnit>& new_ptr) {
  lower_connections_[connection_name] = new_ptr;
}

void AutobotUnit::Print() {
  qDebug() << GetTypeName() << " Name: " << unit_name_
           << "\n Upper connections: " << upper_connections_.keys()
           << "\n Lower connections: " << lower_connections_.keys();
}

} // namespace
