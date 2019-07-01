#ifndef AUTOBOT_UNIT_H
#define AUTOBOT_UNIT_H

#include <unordered_set>
#include <QString>
#include <memory>

namespace autobot {
class AutobotUnit;
typedef std::unordered_set<std::shared_ptr<AutobotUnit>> ConnectionUnitList;

class AutobotUnit : public std::enable_shared_from_this<AutobotUnit> {

public:
  AutobotUnit() = default;
  AutobotUnit(const QString& unit_name);

  virtual ~AutobotUnit();
  std::shared_ptr<AutobotUnit> GetPtr();

  const QString& GetUnitName() const;
  void SetUnitName(const QString&);

  void BreakConnections();
  void BreakUpperConnections();
  void BreakLowerConnections();

  virtual bool AddUpperConnection(const std::shared_ptr<AutobotUnit>&);
  virtual bool RemoveUpperConnection(const std::shared_ptr<AutobotUnit>&);
  virtual const ConnectionUnitList& GetAllUpperConnections() const;

  virtual bool AddLowerConnection(const std::shared_ptr<AutobotUnit>&);
  virtual bool RemoveLowerConnection(const std::shared_ptr<AutobotUnit>&);
  virtual const ConnectionUnitList & GetAllLowerConnections() const;

  bool IsUpperConnector(const std::shared_ptr<AutobotUnit>&);
  bool IsLowerConnector(const std::shared_ptr<AutobotUnit>&);

  virtual const QString GetTypeName() const = 0;
private:
  QString unit_name_;
  ConnectionUnitList upper_connections_;
  ConnectionUnitList lower_connections_;
};

} // namespace
#endif // AUTOBOT_UNIT_H
