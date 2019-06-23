#ifndef BOT_EDIT_WINDOW_H
#define BOT_EDIT_WINDOW_H

#include <QMainWindow>
#include "autobot_account.h"
#include "target_room_dialog.h"
#include <memory>

namespace Ui {
  class AutobotEditWindow;
}

namespace autobot {

class AutobotEditWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit AutobotEditWindow(QWidget *parent = nullptr);
  void CombineAutobotAccount(const std::shared_ptr<AutobotAccount>& account);
  ~AutobotEditWindow();
  void UpdateUI();

private slots:
  void AssignTargetRoomToTarget(const QString& target_room_str);

  void on_pushButton_update_account_clicked();

  void on_pushButton_add_target_clicked();

  void on_pushButton_remove_target_clicked();

  void on_pushButton_set_speech_clicked();

private:
  Ui::AutobotEditWindow *ui;
  TargetRoomDialog* target_room_dialog_;
  std::shared_ptr<AutobotAccount> autobot_account_ptr_;
};

}
#endif // BOT_EDIT_WINDOW_H
