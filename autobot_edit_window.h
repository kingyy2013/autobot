#ifndef BOT_EDIT_WINDOW_H
#define BOT_EDIT_WINDOW_H

#include <QMainWindow>
#include <QDialog>

#include "autobot_account.h"
#include "target_speech_edit_window.h"

#include <memory>

namespace Ui {
  class AutobotEditWindow;
  class TargetRoomDialog;
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
  void AssignTargetRoomToTarget();

  void SetTaskConfig();

  void on_pushButton_update_account_clicked();

  void on_pushButton_add_target_clicked();

  void on_pushButton_remove_target_clicked();

private:
  Ui::AutobotEditWindow *ui;
  Ui::TargetRoomDialog* target_room_dialog_ui_;
  QDialog* target_room_dialog_;
  std::shared_ptr<AutobotAccount> autobot_account_ptr_;
};

}
#endif // BOT_EDIT_WINDOW_H
