#ifndef BOT_EDIT_WINDOW_H
#define BOT_EDIT_WINDOW_H

#include <QMainWindow>
#include <QDialog>

#include "autobot_account.h"
#include "target_speech_edit_window.h"

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

  void SetTaskConfig();

  void on_pushButton_update_account_clicked();

private:
  Ui::AutobotEditWindow *ui;
  std::shared_ptr<AutobotAccount> autobot_account_ptr_;
};

}
#endif // BOT_EDIT_WINDOW_H
