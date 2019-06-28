#ifndef TASK_BASE_H
#define TASK_BASE_H

#include <QRandomGenerator>

namespace autobot {
/// @brief The task configuratio. Will set the interval and order of the task
/// speech.
struct TaskConfig {
  /// @brief Whether the interval of the speech is fixed.
  bool fixed_interval;
  /// @brief Whether the order of the speech is fixed.
  bool fixed_order;
  /// @brief The interval to follow when the speech has fixed frequence.
  int interval_seconds;
  /// @brief The minimum interval when the interval is random.
  int min_seconds;
  /// @brief The maximum interval when the interval is random.
  int max_seconds;
  /// @brief The randeom number generator.
  QRandomGenerator qrand_gen;
  /// @brief Default consturctor.
  TaskConfig () : fixed_interval(true), fixed_order(false), interval_seconds(5),
    min_seconds(1), max_seconds(10) {}
  /// @brief Get a random interval between min_seconds and max_seconds,
  /// following uniform distribution.
  int GetInterval() {
    if (fixed_interval == true) {
      return interval_seconds;
    } else {
      return qrand_gen.bounded(min_seconds, max_seconds);
    }
  }
};

/// @brief The interface of the tasks.
class TaskInterface {
public:
  TaskInterface() = default;
  virtual ~TaskInterface() = default;
  /// @brief Set the configruation of the task.
  /// @param task_config. The input task configuration.
  void SetConfig(const TaskConfig& task_config);
  /// @brief Get the configuration of the task.
  /// @returns The configuration of the task.
  const TaskConfig& GetConfig() const;
  /// @brief The login task, that will perform the login of the autobot. Must be
  /// implmented by the derived class.
  /// @return True. Succes. False. Failed.
  bool virtual LoginTask() = 0;
  /// @brief The speak task, that will make the autobot to speak in the certain
  /// target room.
  /// @return True. Succes. False. Failed.
  bool virtual SpeakTask() = 0;
  /// @brief The visit task, make the autobot enter a certain target room.
  /// @return True. Succes. False. Failed.
  bool virtual VisitTask() = 0;
private:
  TaskConfig task_config;
};

} //namespace
#endif // TASK_BASE_H
