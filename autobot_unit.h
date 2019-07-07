#ifndef AUTOBOT_UNIT_H
#define AUTOBOT_UNIT_H

#include <unordered_set>
#include <QHash>
#include <QString>
#include <memory>

namespace autobot {
class AutobotUnit;
typedef QHash<QString, std::shared_ptr<AutobotUnit>> AutobotUnitDict;

class AutobotUnit : public std::enable_shared_from_this<AutobotUnit> {

public:
  AutobotUnit() = default;
  AutobotUnit(const QString& unit_name);
  void Print();
  virtual ~AutobotUnit();
  std::shared_ptr<AutobotUnit> GetPtr();

  const QString& GetUnitName() const;
  void SetUnitName(const QString&);

  void BreakConnections();
  void BreakUpperConnections();
  void BreakLowerConnections();

  virtual bool AddUpperConnection(const std::shared_ptr<AutobotUnit>&);
  virtual bool ReserveUpperConnection(const QString& );

  virtual bool RemoveUpperConnection(const QString&);
  virtual const AutobotUnitDict& GetAllUpperConnections() const;

  virtual bool AddLowerConnection(const std::shared_ptr<AutobotUnit>&);
  virtual bool ReserveLowerConnection(const QString& );

  virtual bool RemoveLowerConnection(const QString&);
  virtual const AutobotUnitDict & GetAllLowerConnections() const;

  // Reassign the connection_name in the upper connection to the input ptr.
  void ReassignUpperConnection(const QString& connection_name,
                               const std::shared_ptr<AutobotUnit>& new_ptr);
  // Reassign the conneciton_name in the lower connection to the input ptr.
  void ReassignLowerConnection(const QString& conneciton_name,
                               const std::shared_ptr<AutobotUnit>& new_ptr);

  virtual const QString GetTypeName() const = 0;
private:
  QString unit_name_;
  AutobotUnitDict upper_connections_;
  AutobotUnitDict lower_connections_;
};

} // namespace
#endif // AUTOBOT_UNIT_H
