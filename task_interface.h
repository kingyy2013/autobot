#ifndef TASK_BASE_H
#define TASK_BASE_H

#include <QRandomGenerator>

namespace autobot {

struct TaskConfig {
  bool fixed_interval;
  int interval_seconds;
  int min_seconds;
  int max_seconds;
  QRandomGenerator qrand_gen;
  TaskConfig () : fixed_interval(true), interval_seconds(5), min_seconds(1),
    max_seconds(10) {}
  int GetInterval() {
    if (fixed_interval == true) {
      return interval_seconds;
    } else {
      return qrand_gen.bounded(min_seconds, max_seconds);
    }
  }
};

class TaskInterface {
public:
  TaskInterface() = default;
  virtual ~TaskInterface() = default;
  void SetConfig(const TaskConfig& task_config);
  const TaskConfig& GetConfig() const;
  bool virtual LoginTask() = 0;
  bool virtual SpeakTask() = 0;
  bool virtual VisitTask() = 0;
private:
  TaskConfig task_config;
};

} //namespace
#endif // TASK_BASE_H
